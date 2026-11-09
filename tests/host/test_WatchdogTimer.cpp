/**
 * @file test_WatchdogTimer.cpp
 * @brief Host test of the WatchdogTimer WDTCSR encodings and timeout rule
 *
 * Register-level host test: the device registers used here (WDTCSR, MCUSR)
 * are defined as plain globals in this file; the stub headers under
 * stubs/ supply the ATmega328P bit constants.
 */

#include <stdint.h>
#include <stdio.h>

#include "host_registers.h"

#include "WatchdogTimer.h"

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
    core::WatchdogTimer &wdt = core::WatchdogTimer::getInstance();

    // constructor stops the watchdog
    CHECK(WDTCSR == 0x00);

    printf("operation mode -> WDTCSR (timeout = 16 ms):\n");
    struct ModeCase {
        const char *name;
        core::watchdogOperationMode mode;
        uint8_t expected;
    };
    const ModeCase cases[] = {
        { "interrupt",        core::watchdogOperationMode::interrupt,        0x40 },
        { "reset",            core::watchdogOperationMode::reset,            0x08 },
        { "interrupt+reset",  core::watchdogOperationMode::interrupt_reset,  0x48 },
    };
    for (uint8_t i = 0; i < 3; i++)
    {
        wdt.stop();
        wdt.selectTimeOut(core::timeOut::to_16ms);
        wdt.start(cases[i].mode);
        printf("  %-16s WDTCSR = 0x%02X (expected 0x%02X)\n",
               cases[i].name, WDTCSR, cases[i].expected);
        CHECK(WDTCSR == (cases[i].expected | 0x00));
        // reset and interrupt+reset must set WDE (bit 3)
        if (cases[i].mode != core::watchdogOperationMode::interrupt)
        {
            CHECK((WDTCSR & (1 << WDE)) != 0);
        }
    }

    // interrupt-only mode must leave WDE clear
    wdt.stop();
    wdt.start(core::watchdogOperationMode::interrupt);
    CHECK((WDTCSR & (1 << WDE)) == 0);
    CHECK((WDTCSR & (1 << WDIE)) != 0);

    // selectTimeOut while running keeps WDE set
    wdt.stop();
    wdt.start(core::watchdogOperationMode::reset, core::timeOut::to_8s);
    printf("  running, timeout 8 s:        WDTCSR = 0x%02X\n", WDTCSR);
    CHECK(WDTCSR == 0x29); // WDE | WDP3 | WDP0
    wdt.selectTimeOut(core::timeOut::to_16ms);
    printf("  running, timeout -> 16 ms:   WDTCSR = 0x%02X\n", WDTCSR);
    CHECK(WDTCSR == 0x08); // WDE preserved, timeout bits replaced

    // selectTimeOut while running keeps WDIE set in interrupt mode
    wdt.stop();
    wdt.start(core::watchdogOperationMode::interrupt);
    wdt.selectTimeOut(core::timeOut::to_2s);
    printf("  running, timeout -> 2 s:     WDTCSR = 0x%02X\n", WDTCSR);
    CHECK(WDTCSR == 0x47); // WDIE | WDP2 | WDP1 | WDP0

    // selectTimeOut while stopped only stages the timeout bits
    wdt.stop();
    wdt.selectTimeOut(core::timeOut::to_8s);
    printf("  stopped, timeout 8 s:        WDTCSR = 0x%02X\n", WDTCSR);
    CHECK(WDTCSR == 0x21); // no WDE, no WDIE, WDP3 | WDP0

    // start() clears the watchdog reset flag
    MCUSR = 0xFF;
    wdt.start(core::watchdogOperationMode::reset);
    CHECK((MCUSR & (1 << WDRF)) == 0);

    printf("%s: %u failure(s)\n", failures ? "FAILED" : "PASSED", failures);
    return failures ? 1 : 0;
}