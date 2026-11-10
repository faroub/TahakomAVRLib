/**
 * @file test_PIDController.cpp
 * @brief Host unit test for PIDController math
 */

#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "PIDController.h"

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
    printf("PID controller response:\n");
    utils::PIDController pid(2.0f, 0.5f, 0.1f, -100.0f, 100.0f);

    // Step error = 10 (setpoint 100, input 90), dt = 0.1s
    // P = 2 * 10 = 20
    // I = 0.5 * (10 * 0.1) = 0.5
    // D = 0.1 * (10 / 0.1) = 10
    // Total = 30.5
    float out1 = pid.compute(100.0f, 90.0f, 0.1f);
    printf("  step 1 output = %.2f (expected ~30.50)\n", out1);
    CHECK(fabsf(out1 - 30.50f) < 0.1f);

    // Output clamping check
    float out_large = pid.compute(1000.0f, 0.0f, 0.1f);
    printf("  clamped output = %.2f (expected 100.00)\n", out_large);
    CHECK(out_large == 100.0f);

    printf("%s: %u failure(s)\n", failures ? "FAILED" : "PASSED", failures);
    return failures ? 1 : 0;
}
