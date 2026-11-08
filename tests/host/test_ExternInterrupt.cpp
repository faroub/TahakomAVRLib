/**
 * @file test_ExternInterrupt.cpp
 * @brief Host test of the external interrupt sense controls and masks
 *
 * Register-level host test: the device registers used here (EICRA, EIMSK,
 * PCICR, PCMSK0/1/2) are defined as plain globals in this file; the stub
 * headers under stubs/ supply the ATmega328P bit constants.
 */

#include <stdint.h>
#include <stdio.h>

// device registers as plain globals
volatile uint8_t EICRA;
volatile uint8_t EIMSK;
volatile uint8_t PCICR;
volatile uint8_t PCMSK0;
volatile uint8_t PCMSK1;
volatile uint8_t PCMSK2;

#include "ExternInterrupt.h"

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
    core::ExternInterrupt &ext = core::ExternInterrupt::getInstance();

    printf("INT1 sense control (falling edge) changes only EICRA bits 3:2:\n");
    EICRA = 0x00;
    ext.setInt1SenseControl(core::senseControl::fallingEdge);
    printf("  EICRA = 0x%02X\n", EICRA);
    CHECK(EICRA == 0x08);       // ISC1 = 0b10, everything else untouched
    CHECK((EICRA & 0x0C) == 0x08);
    CHECK((EICRA & 0xF3) == 0x00);

    printf("INT1 sense control (rising edge) preserves the other bits:\n");
    EICRA = 0x55;
    ext.setInt1SenseControl(core::senseControl::risingEdge);
    printf("  EICRA = 0x%02X\n", EICRA);
    CHECK((EICRA & 0x0C) == 0x0C);   // ISC1 = 0b11
    CHECK((EICRA & 0xF3) == 0x51);   // all other bits unchanged

    printf("INT0 sense control (falling edge) changes only EICRA bits 1:0:\n");
    EICRA = 0x00;
    ext.setInt0SenseControl(core::senseControl::fallingEdge);
    printf("  EICRA = 0x%02X\n", EICRA);
    CHECK(EICRA == 0x02);       // ISC0 = 0b10
    CHECK((EICRA & 0x03) == 0x02);
    CHECK((EICRA & 0xFC) == 0x00);

    printf("INT1 falling edge while INT0 is set (bits must not bleed):\n");
    EICRA = 0x00;
    ext.setInt0SenseControl(core::senseControl::risingEdge);   // ISC0 = 0b11
    ext.setInt1SenseControl(core::senseControl::fallingEdge);  // ISC1 = 0b10
    printf("  EICRA = 0x%02X\n", EICRA);
    CHECK(EICRA == 0x0B);

    printf("pin-change masks toggle per pin:\n");
    PCMSK0 = 0x00;
    ext.enablePinChangeMaskPortB(3, 1);
    CHECK(PCMSK0 == (1 << 3));
    ext.enablePinChangeMaskPortB(3, 0);   // must disable, not re-enable
    CHECK(PCMSK0 == 0x00);

    PCMSK1 = 0x00;
    ext.enablePinChangeMaskPortC(4, 1);
    CHECK(PCMSK1 == (1 << 4));
    ext.enablePinChangeMaskPortC(4, 0);
    CHECK(PCMSK1 == 0x00);

    PCMSK2 = 0xFF;
    ext.enablePinChangeMaskPortD(5, 1);
    CHECK(PCMSK2 == 0xFF);
    ext.enablePinChangeMaskPortD(5, 0);
    CHECK(PCMSK2 == (uint8_t)(~(1 << 5)));

    printf("%s: %u failure(s)\n", failures ? "FAILED" : "PASSED", failures);
    return failures ? 1 : 0;
}
