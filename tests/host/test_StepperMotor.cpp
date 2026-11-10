/**
 * @file test_StepperMotor.cpp
 * @brief Characterization tests of the StepperMotor speed profile
 *
 * Register-level host test: the coil outputs are pins 0..3 of PORTB,
 * defined as plain globals in this file (host_registers.h); the stub
 * headers under stubs/ supply the ATmega328P bit constants.  The delay
 * stub makes _delay_ms a no-op, so timing is measured in step() call
 * counts: with the 1 ms polling cadence of step(), one call is 1 ms.
 *
 * These tests characterize the four-argument step() over 200-step moves
 * after the P23 math rework: the move must emit one coil pulse per step
 * and must span the analytic trapezoid time
 *     accelTime + constSpeedTime + decelTime [ms]
 * within 1%, for the (1000, 5000, 5000) profile that is 200+0+200 = 400.
 * Cases marked XFAIL are known-wrong behaviors that remain; an XFAIL
 * that starts to pass shows a further fix landed.
 */

#include <stdint.h>
#include <stdio.h>

#include "host_registers.h"

#include "StepperMotor.h"

static uint8_t failures = 0;
static uint16_t xfailures = 0;
static uint16_t xpasses = 0;

#define CHECK(cond) \
    do { \
        if (!(cond)) { \
            printf("  FAIL: %s (line %d)\n", #cond, __LINE__); \
            failures++; \
        } \
    } while (0)

// expected-to-fail: known-wrong current behavior; must not fail the suite
#define XFAIL(cond) \
    do { \
        if (!(cond)) { \
            printf("  XFAIL (known-wrong): %s (line %d)\n", #cond, __LINE__); \
            xfailures++; \
        } \
        else { \
            printf("  XPASS (was expected to fail): %s (line %d)\n", #cond, __LINE__); \
            xpasses++; \
        } \
    } while (0)

// Drive one move for at most a_callCap calls, measuring the emitted
// coil pulses, the call index of the last pulse (the move duration in
// ms), and whether the position is still advancing afterwards.
static void runMove(component::StepperMotor &ar_stepper,
                    const int16_t a_step,
                    const uint16_t a_speed,
                    const uint16_t a_accel,
                    const uint16_t a_decel,
                    const uint32_t a_callCap,
                    uint16_t &ar_pulses,
                    uint32_t &ar_lastPulseCall,
                    uint32_t &ar_calls)
{
    uint8_t l_lastPattern = PORTB;
    ar_pulses = 0;
    ar_lastPulseCall = 0;
    ar_calls = 0;
    do
    {
        ar_stepper.step(a_step, a_speed, a_accel, a_decel);
        ar_calls++;
        if (PORTB != l_lastPattern)
        {
            ar_pulses++;
            ar_lastPulseCall = ar_calls;
            l_lastPattern = PORTB;
        }
    } while ((ar_calls < a_callCap) && !ar_stepper.goalReached());
}

int main(void)
{
    component::StepperMotor &stepper = *new component::StepperMotor(
        component::mode::fullStep,
        io::Pin(0, io::PortB), io::Pin(1, io::PortB),
        io::Pin(2, io::PortB), io::Pin(3, io::PortB));

    // 200-step moves with speed/accel/decel = 1000/5000/5000:
    // analytic trapezoid = 200 ms accel + 0 const + 200 ms decel = 400 ms,
    // 200 coil pulses, one per step
    uint16_t l_pulses = 0;
    uint32_t l_lastPulseCall = 0;
    uint32_t l_calls = 0;

    printf("200-step move with speed/accel/decel = 1000/5000/5000:\n");
    runMove(stepper, -200, 1000, 5000, 5000, 1000UL,
            l_pulses, l_lastPulseCall, l_calls);
    printf("  calls=%lu pulses=%u lastPulseCall=%lu currentPos=%u goal=%u\n",
           (unsigned long)l_calls, l_pulses, (unsigned long)l_lastPulseCall,
           stepper.currentPos(), stepper.goalReached());
    // invariants of the move: one coil pulse per step on pins 0..3
    CHECK((PORTB & 0xF0) == 0);
    CHECK(l_pulses > 196);      // ~200 coil outputs, 1% slack either side
    CHECK(l_pulses < 206);
    CHECK(l_lastPulseCall > 396);   // ~400 ms, 1% of the analytic 400 ms
    CHECK(l_lastPulseCall < 404);
    CHECK(stepper.currentPos() == 200);
    CHECK(stepper.goalReached() == 1);
    CHECK(l_calls < 1000UL);

    printf("a consecutive move with a new target replays the profile:\n");
    // move 2: 200 steps the other way; beginMove() reset the profile
    // state of the previous move, so this move emits its own pulses and
    // walks the position back
    runMove(stepper, 200, 1000, 5000, 5000, 1000UL,
            l_pulses, l_lastPulseCall, l_calls);
    printf("  calls=%lu pulses=%u lastPulseCall=%lu currentPos=%u goal=%u\n",
           (unsigned long)l_calls, l_pulses, (unsigned long)l_lastPulseCall,
           stepper.currentPos(), stepper.goalReached());
    CHECK(l_pulses > 196);
    CHECK(l_pulses < 206);
    CHECK(l_lastPulseCall > 396);
    CHECK(l_lastPulseCall < 404);
    CHECK(stepper.currentPos() == 0);
    CHECK(stepper.goalReached() == 1);
    CHECK(l_calls < 1000UL);

    printf("and a third move back to the first target:\n");
    runMove(stepper, -200, 1000, 5000, 5000, 1000UL,
            l_pulses, l_lastPulseCall, l_calls);
    printf("  calls=%lu pulses=%u lastPulseCall=%lu currentPos=%u goal=%u\n",
           (unsigned long)l_calls, l_pulses, (unsigned long)l_lastPulseCall,
           stepper.currentPos(), stepper.goalReached());
    CHECK(l_pulses > 196);
    CHECK(l_pulses < 206);
    CHECK(l_lastPulseCall > 396);
    CHECK(l_lastPulseCall < 404);
    CHECK(stepper.currentPos() == 200);
    CHECK(stepper.goalReached() == 1);
    CHECK(l_calls < 1000UL);

    // accel/decel smaller than what the step count needs: the profile is
    // rescaled so the motor still moves (the sqrtf fix; the old integer
    // division collapsed the ramp to zero and nothing moved at all).
    // The rescaled trapezoid is 632 ms accel + 632 ms decel at 316 steps/s,
    // which covers 199 of the 200 requested steps; the discrete speed
    // integrator fires its last step a few ticks before the ramp ends
    printf("accel/decel too small for the speed scales the profile down:\n");
    component::StepperMotor &stepper2 = *new component::StepperMotor(
        component::mode::fullStep,
        io::Pin(4, io::PortB), io::Pin(5, io::PortB),
        io::Pin(6, io::PortB), io::Pin(7, io::PortB));
    runMove(stepper2, -200, 1000, 500, 500, 2000UL,
            l_pulses, l_lastPulseCall, l_calls);
    printf("  calls=%lu pulses=%u lastPulseCall=%lu currentPos=%u goal=%u\n",
           (unsigned long)l_calls, l_pulses, (unsigned long)l_lastPulseCall,
           stepper2.currentPos(), stepper2.goalReached());
    CHECK(l_pulses > 196);
    CHECK(l_pulses < 206);
    CHECK(stepper2.currentPos() == l_pulses);
    CHECK(l_lastPulseCall > 1200);   // ~1264 ms rescaled ramp, discrete lag
    CHECK(l_lastPulseCall < 1330);

    // a zero speed, acceleration or deceleration cannot drive a
    // profile: no step fires, no crash, no division by zero
    printf("a zero speed/accel/decel fires no step and divides by nothing:\n");
    component::StepperMotor &stepper3 = *new component::StepperMotor(
        component::mode::fullStep,
        io::Pin(0, io::PortD), io::Pin(1, io::PortD),
        io::Pin(2, io::PortD), io::Pin(3, io::PortD));
    PORTD = 0;
    for (uint16_t i = 0; i < 100; i++)
    {
        stepper3.step(-200, 0, 5000, 5000);
        stepper3.step(-200, 1000, 0, 5000);
        stepper3.step(-200, 1000, 5000, 0);
    }
    printf("  300 calls: currentPos=%u goal=%u PORTD=0x%02X\n",
           stepper3.currentPos(), stepper3.goalReached(), PORTD);
    CHECK(stepper3.currentPos() == 0);
    CHECK(PORTD == 0);
    CHECK(stepper3.goalReached() == 0);

    printf("%s: %u failure(s), %u xfail(s), %u xpass(es)\n",
           failures ? "FAILED" : "PASSED", failures, xfailures, xpasses);
    return failures ? 1 : 0;
}
