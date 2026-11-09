/**
 * @file test_Buzzer.cpp
 * @brief Host test of the Buzzer edge cases
 *
 * Register-level host test: the Buzzer output and the TimerCounter0
 * registers are plain globals (host_registers.h).  The delay stub makes
 * the busy-wait loops no-ops, so the zero-period and zero-duration edge
 * cases are checked by the call returning (without the guards the pitch
 * loop would spin by steps of 0 and this test would hang) and by the pin
 * state staying untouched.
 */

#include <stdint.h>
#include <stdio.h>

#include "host_registers.h"

#include "Buzzer.h"
#include "TimerCounter1.h"

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
    component::Buzzer buzzer(io::Pin(0, io::PortB));
    CHECK((DDRB & (1 << 0)) != 0);   // pin configured as an output

    printf("buzz with a zero period returns immediately:\n");
    PORTB = 0x00;
    buzzer.buzz(0, 1000);   // would spin forever without the guard
    CHECK(PORTB == 0x00);   // the pin is left untouched

    printf("buzz with a zero duration:\n");
    buzzer.buzz(1000, 0);
    CHECK(PORTB == 0x00);   // no toggles, no pulse

    printf("a normal buzz ends with the pin low:\n");
    PORTB = 0x01;
    buzzer.buzz(1000, 4000);   // 4 toggles, then setLow()
    CHECK(PORTB == 0x00);

    printf("pause(0) returns immediately:\n");
    buzzer.pause(0);   // the decrement loop would otherwise wrap to 65535
    buzzer.pause(1);   // the smallest non-zero duration still returns

    printf("timer-driven buzz loads the period as timer ticks:\n");
    // the 16-bit TimerCounter1 keeps the full tick value (Timer0/2 are
    // 8-bit and would truncate 568 to 56)
    core::TimerCounter1 &timer = core::TimerCounter1::getInstance();
    OCR1A = 0;
    uint16_t l_duration = 200;
    buzzer.buzz(timer, 568, l_duration);   // 568 ticks at PS_64 ~= 220 Hz
    printf("  OCR1A = %u (expected 568)\n", OCR1A);
    CHECK(OCR1A == 568);
    // the duration is passed by value: the caller's variable survives
    printf("  duration after the call = %u (expected 200)\n", l_duration);
    CHECK(l_duration == 200);

    printf("%s: %u failure(s)\n", failures ? "FAILED" : "PASSED", failures);
    return failures ? 1 : 0;
}
