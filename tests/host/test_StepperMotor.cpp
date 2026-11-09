/**
 * @file test_StepperMotor.cpp
 * @brief Characterization tests of the StepperMotor speed profile
 *
 * Register-level host test: the coil outputs are pins 0..3 of PORTB,
 * defined as plain globals in this file (host_registers.h); the stub
 * headers under stubs/ supply the ATmega328P bit constants.  The delay
 * stub makes _delay_ms a no-op, so timing is measured in call counts.
 *
 * These tests characterize the CURRENT behavior of the four-argument
 * step() over a 200-step move: how many coil pulses it emits, how the
 * position advances, whether the goal flag ever becomes 1, and how the
 * static profile state carries over into a second move.  Cases marked
 * XFAIL are known-wrong behaviors that P22/P23 rework; an XFAIL that
 * starts to pass shows the fix landed.
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

int main(void)
{
    component::StepperMotor &stepper = *new component::StepperMotor(
        component::mode::fullStep,
        io::Pin(0, io::PortB), io::Pin(1, io::PortB),
        io::Pin(2, io::PortB), io::Pin(3, io::PortB));

    // The motor only advances on the step() calls for which
    // computeStepDelay() returns a nonzero delay: early in the ramp the
    // speed integrator needs many calls before the first 1 ms delay
    // fires, so a 200-step move needs far more than 200 calls.
    printf("200-step move with speed/accel/decel = 1000/5000/5000:\n");
    const uint32_t l_callCap = 100000UL;
    uint32_t l_calls = 0;
    uint16_t l_pulses = 0;
    uint8_t l_lastPattern = PORTB;
    while (!stepper.goalReached() && (l_calls < l_callCap))
    {
        stepper.step(-200, 1000, 5000, 5000);
        l_calls++;
        if (PORTB != l_lastPattern)
        {
            l_pulses++;
            l_lastPattern = PORTB;
        }
    }
    printf("  calls=%lu pulses=%u currentPos=%u goal=%u\n",
           (unsigned long)l_calls, l_pulses, stepper.currentPos(),
           stepper.goalReached());
    // invariants of the move: one coil pulse per step on pins 0..3
    CHECK((PORTB & 0xF0) == 0);
    CHECK(l_pulses > 196);      // ~200 coil outputs, 1% slack either side
    CHECK(l_pulses < 206);
    CHECK(stepper.currentPos() == 200);
    // known-wrong: when the ramp ends the profile speed keeps its final
    // value (zero), so the one extra fire that would set the goal flag
    // never comes and polling goalReached() spins forever; the position
    // and pulse count of the move itself are correct
    XFAIL(stepper.goalReached() == 1);
    XFAIL(l_calls < l_callCap);

    printf("a consecutive move with a new target replays the profile:\n");
    // move 2: 200 steps the other way; beginMove() reset the profile
    // state of the previous move, so this move emits its own pulses and
    // walks the position back (the static state used to leak between
    // moves and the second move never moved at all)
    uint32_t l_calls2 = 0;
    uint16_t l_pulses2 = 0;
    uint8_t l_lastPattern2 = PORTB;
    while (!stepper.goalReached() && (l_calls2 < l_callCap))
    {
        stepper.step(200, 1000, 5000, 5000);
        l_calls2++;
        if (PORTB != l_lastPattern2)
        {
            l_pulses2++;
            l_lastPattern2 = PORTB;
        }
    }
    printf("  calls=%lu pulses=%u currentPos=%u goal=%u\n",
           (unsigned long)l_calls2, l_pulses2, stepper.currentPos(),
           stepper.goalReached());
    CHECK(l_pulses2 > 196);
    CHECK(l_pulses2 < 206);
    CHECK(stepper.currentPos() == 0);
    XFAIL(stepper.goalReached() == 1);
    XFAIL(l_calls2 < l_callCap);

    printf("and a third move back to the first target:\n");
    uint32_t l_calls3 = 0;
    uint16_t l_pulses3 = 0;
    uint8_t l_lastPattern3 = PORTB;
    while (!stepper.goalReached() && (l_calls3 < l_callCap))
    {
        stepper.step(-200, 1000, 5000, 5000);
        l_calls3++;
        if (PORTB != l_lastPattern3)
        {
            l_pulses3++;
            l_lastPattern3 = PORTB;
        }
    }
    printf("  calls=%lu pulses=%u currentPos=%u goal=%u\n",
           (unsigned long)l_calls3, l_pulses3, stepper.currentPos(),
           stepper.goalReached());
    CHECK(l_pulses3 > 196);
    CHECK(l_pulses3 < 206);
    CHECK(stepper.currentPos() == 200);
    XFAIL(stepper.goalReached() == 1);
    XFAIL(l_calls3 < l_callCap);

    printf("accel/decel larger than the step count scales the profile out of reach:\n");
    component::StepperMotor &stepper2 = *new component::StepperMotor(
        component::mode::fullStep,
        io::Pin(4, io::PortB), io::Pin(5, io::PortB),
        io::Pin(6, io::PortB), io::Pin(7, io::PortB));
    for (uint16_t i = 0; i < 200; i++)
    {
        stepper2.step(-200, 1000, 500, 500);
    }
    printf("  200 calls: currentPos=%u goal=%u PORTB=0x%02X\n",
           stepper2.currentPos(), stepper2.goalReached(), PORTB);
    CHECK((PORTB & 0xF0) == 0);
    XFAIL(stepper2.currentPos() > 0);   // the motor never moves at all
    XFAIL(stepper2.goalReached() == 1);

    printf("%s: %u failure(s), %u xfail(s), %u xpass(es)\n",
           failures ? "FAILED" : "PASSED", failures, xfailures, xpasses);
    return failures ? 1 : 0;
}
