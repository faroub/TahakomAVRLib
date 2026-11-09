/**
 * @file test_PushButton.cpp
 * @brief Host test of the PushButton press detection
 *
 * Register-level host test: the pin register PINB is a plain global
 * (host_registers.h); feeding it a press/release sequence checks that a
 * press is counted exactly once (on the edge), that the level is chosen
 * by the active-low flag alone, and that the constructor configures the
 * pull-up.  The delay stub makes the 1 ms debounce wait a no-op.
 */

#include <stdint.h>
#include <stdio.h>

#include "host_registers.h"

#include "PushButton.h"

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
    // active-low button with the internal pull-up, as in the MusicBox app:
    // idle reads high, a press pulls the pin low
    printf("active-low button with the internal pull-up (pin PB1):\n");
    component::PushButton button(io::Pin(1, io::PortB));
    CHECK((PORTB & (1 << 1)) != 0);   // pull-up resistor enabled
    CHECK((DDRB & (1 << 1)) == 0);    // pin configured as an input
    CHECK(button.getPressedCount() == 0);

    printf("  idle reads as not pressed:\n");
    PINB = 0x02;
    for (uint8_t i = 0; i < 5; i++)
    {
        CHECK(button.isPressed() == 0);
    }
    CHECK(button.getPressedCount() == 0);

    printf("  a press counts once, holding does not count again:\n");
    PINB = 0x00;
    CHECK(button.isPressed() == 1);
    CHECK(button.getPressedCount() == 1);
    for (uint8_t i = 0; i < 10; i++)
    {
        CHECK(button.isPressed() == 1);
    }
    CHECK(button.getPressedCount() == 1);   // still the same single press

    printf("  release then press again:\n");
    PINB = 0x02;
    CHECK(button.isPressed() == 0);
    CHECK(button.getPressedCount() == 1);
    PINB = 0x00;
    CHECK(button.isPressed() == 1);
    CHECK(button.getPressedCount() == 2);
    PINB = 0x02;
    CHECK(button.isPressed() == 0);
    PINB = 0x00;
    CHECK(button.isPressed() == 1);
    CHECK(button.getPressedCount() == 3);

    printf("  resetPressedCount clears the counter:\n");
    button.resetPressedCount();
    CHECK(button.getPressedCount() == 0);
    PINB = 0x02;
    CHECK(button.isPressed() == 0);
    PINB = 0x00;
    CHECK(button.isPressed() == 1);
    CHECK(button.getPressedCount() == 1);

    printf("active-high button without a pull-up (pin PB2):\n");
    component::PushButton activeHigh(io::Pin(2, io::PortB), 0, 0);
    CHECK((PORTB & (1 << 2)) == 0);   // no pull-up resistor
    CHECK((DDRB & (1 << 2)) == 0);
    PINB = 0x04;                      // pin high means pressed
    CHECK(activeHigh.isPressed() == 1);
    CHECK(activeHigh.getPressedCount() == 1);
    for (uint8_t i = 0; i < 5; i++)
    {
        CHECK(activeHigh.isPressed() == 1);
    }
    CHECK(activeHigh.getPressedCount() == 1);
    PINB = 0x00;
    CHECK(activeHigh.isPressed() == 0);
    CHECK(activeHigh.getPressedCount() == 1);

    printf("active-low button without a pull-up (pin PB3):\n");
    // the level must be selected by the active-low flag alone, not by the
    // pull-up flag: this reads low as pressed even without a pull-up
    component::PushButton noPullUp(io::Pin(3, io::PortB), 0, 1);
    CHECK((PORTB & (1 << 3)) == 0);
    PINB = 0x00;
    CHECK(noPullUp.isPressed() == 1);
    CHECK(noPullUp.getPressedCount() == 1);
    PINB = 0x08;
    CHECK(noPullUp.isPressed() == 0);
    CHECK(noPullUp.getPressedCount() == 1);

    printf("%s: %u failure(s)\n", failures ? "FAILED" : "PASSED", failures);
    return failures ? 1 : 0;
}
