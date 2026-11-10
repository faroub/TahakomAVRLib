/**
 * @file test_RotaryEncoder.cpp
 * @brief Host unit test for Quadrature RotaryEncoder logic
 */

#include <stdint.h>
#include <stdio.h>

#include "host_registers.h"
#include "RotaryEncoder.h"

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
    printf("RotaryEncoder quadrature tracking:\n");
    // Pin PB0 (Channel A) and PB1 (Channel B)
    PINB = 0x03; // Initially both high
    component::RotaryEncoder encoder(io::Pin(0, io::PortB), io::Pin(1, io::PortB));

    CHECK(encoder.getPosition() == 0);

    // Transition A low while B high (CW turn)
    PINB = 0x02;
    encoder.update();
    printf("  CW step: position = %ld (expected 1)\n", (long)encoder.getPosition());
    CHECK(encoder.getPosition() == 1);
    CHECK(encoder.getDirection() == 1);

    // Reset position
    encoder.resetPosition();
    CHECK(encoder.getPosition() == 0);

    printf("%s: %u failure(s)\n", failures ? "FAILED" : "PASSED", failures);
    return failures ? 1 : 0;
}
