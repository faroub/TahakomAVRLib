/**
 * @file test_ServoMotor.cpp
 * @brief Host test of the servo pulse-width timer counts
 *
 * Register-level host test: the device registers used by the io::Pin
 * abstraction are defined as plain globals in this file; the stub headers
 * under stubs/ supply the ATmega328P bit constants.
 */

#include <stdint.h>
#include <stdio.h>

#include "host_registers.h"

#include "ServoMotor.h"

static uint8_t failures = 0;

#define CHECK(cond) \
    do { \
        if (!(cond)) { \
            printf("  FAIL: %s (line %d)\n", #cond, __LINE__); \
            failures++; \
        } \
    } while (0)

int main(void)
{
    // common servo timings: 500..3000 us pulse width, 20 ms cycle
    component::ServoMotor servo(io::Pin(2, io::PortB),
                                20000, 500, 1520, 3000);

    printf("pulse width -> timer count (prescaler 8, F_CPU = 16 MHz):\n");
    struct PulseCase { uint16_t pulseWidth_us; uint32_t expected; };
    const PulseCase pulses[] = {
        { 500,  1000 },
        { 1000, 2000 },
        { 1500, 3000 },   // required case: 1500 us at prescaler 8 = 3000
        { 2000, 4000 },
        { 2500, 5000 },
        { 3000, 6000 },
    };
    for (uint8_t i = 0; i < 6; i++)
    {
        // reuse a throwaway servo with the pulse width under test
        component::ServoMotor s(io::Pin(2, io::PortB), 20000,
                                pulses[i].pulseWidth_us, 1520, 3000);
        uint16_t count = s.computePulseWidthMinCount(8);
        printf("  %4u us -> %5u (expected %lu)\n",
               pulses[i].pulseWidth_us, count, (unsigned long)pulses[i].expected);
        CHECK(count == pulses[i].expected);
    }

    printf("pulse cycle 20 ms at prescaler 8 = 40000:\n");
    uint16_t cycle = servo.computePulseCycleCount(8);
    printf("  %u\n", cycle);
    CHECK(cycle == 40000);

    printf("range check: 40 ms cycle at prescaler 1 clamps to 65535:\n");
    component::ServoMotor big(io::Pin(2, io::PortB), 40000, 500, 1520, 3000);
    uint16_t clamped = big.computePulseCycleCount(1);
    printf("  %u\n", clamped);
    CHECK(clamped == 65535);

    printf("angle endpoints map to the min/max counts:\n");
    uint16_t atMin = servo.computeRotationAngleCount(0, 8);
    uint16_t atMax = servo.computeRotationAngleCount(180, 8);
    printf("  0 deg -> %u (min count %u), 180 deg -> %u (max count %u)\n",
           atMin, servo.computePulseWidthMinCount(8),
           atMax, servo.computePulseWidthMaxCount(8));
    CHECK(atMin == servo.computePulseWidthMinCount(8));
    CHECK(atMax == servo.computePulseWidthMaxCount(8));

    printf("%s: %u failure(s)\n", failures ? "FAILED" : "PASSED", failures);
    return failures ? 1 : 0;
}