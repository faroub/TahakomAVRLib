/**
 * @file test_TWI.cpp
 * @brief Host unit test for TWI hardware register setup
 */

#include <stdint.h>
#include <stdio.h>

#include "host_registers.h"
#include "TWI.h"

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
    printf("TWI bitrate initialization:\n");
    core::TWI &twi = core::TWI::getInstance(100000UL);
    twi.setClock(100000UL);
    printf("  TWBR = %u (expected 72)\n", TWBR);
    CHECK(TWBR == 72);

    twi.setClock(400000UL);
    printf("  TWBR = %u (expected 12)\n", TWBR);
    CHECK(TWBR == 12);

    printf("TWI start condition sets TWSTA & TWEN:\n");
    TWSR = 0x08; // Simulate START transmitted
    TWCR = 0;
    uint8_t res = twi.start();
    CHECK(res == 1);
    CHECK((TWCR & (1 << TWSTA)) != 0);
    CHECK((TWCR & (1 << TWEN)) != 0);

    printf("%s: %u failure(s)\n", failures ? "FAILED" : "PASSED", failures);
    return failures ? 1 : 0;
}
