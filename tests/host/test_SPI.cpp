/**
 * @file test_SPI.cpp
 * @brief Host test of the SPI master transfer sequence
 *
 * Register-level host test: the device registers used here (DDRB, PORTB,
 * SPDR, SPSR, SPCR) are defined as plain globals in this file; the stub
 * headers under stubs/ supply the ATmega328P bit constants.
 *
 * masterSendByte busy-waits for the transfer-complete flag, which on the
 * wire is set by the SPI_STC ISR.  The ISR is simulated here by running
 * the transfer on a helper thread and invoking the ISR from the main
 * thread once SPDR has been written, the way the hardware would.
 */

#include <stdint.h>
#include <stdio.h>
#include <thread>

#include "host_registers.h"

#ifndef SPI_TIMEOUT_CYCLES
#define SPI_TIMEOUT_CYCLES 200000000U
#endif

#include "SPI.h"

static uint8_t failures = 0;

#define CHECK(cond) \
    do { \
        if (!(cond)) { \
            printf("  FAIL: %s (line %d)\n", #cond, __LINE__); \
            failures++; \
        } \
    } while (0)

static io::SPI &getSPI()
{
    return io::SPI::getInstance(io::Pin(5, io::PortB),
                                io::Pin(4, io::PortB),
                                io::Pin(3, io::PortB),
                                io::Pin(2, io::PortB));
}

int main(void)
{
    io::SPI &spi = getSPI();

    printf("master mode configures the pins and enables the SPI:\n");
    spi.selectClockPrescaler(io::clockPrescaler::PS_128);
    spi.selectDataMode(io::dataMode::mode_0);
    spi.selectDataOrder(io::dataOrder::first_MSB);
    spi.selectOperationMode(io::operationMode::master);
    CHECK((DDRB & (1 << 5)) != 0);          // SCK output
    CHECK((DDRB & (1 << 3)) != 0);          // MOSI output
    CHECK((DDRB & (1 << 2)) != 0);          // SS output
    CHECK((DDRB & (1 << 4)) == 0);          // MISO input
    CHECK((PORTB & (1 << 2)) != 0);         // SS idles high
    CHECK((SPCR & (1 << MSTR)) != 0);       // master
    CHECK((SPCR & (1 << SPE)) != 0);        // SPI enabled
    CHECK((SPCR & (1 << SPIE)) != 0);       // transfer-complete interrupt armed

    printf("masterSendByte: SS low before the SPDR write, SS high after the ISR:\n");
    SPDR = 0xA5;   // sentinel so we can observe the library's write
    std::thread l_send([]() {
        getSPI().masterSendByte(0x03);
    });
    {
        // spin until the library wrote SPDR (SS was asserted before that)
        for (uint32_t i = 0; (SPDR != 0x03) && (i < 100000000UL); i++) {}
        if (SPDR != 0x03)
        {
            printf("  FAIL: SPDR was never written with 0x03\n");
            failures++;
        }
        CHECK((PORTB & (1 << 2)) == 0);      // SS asserted while transferring
        SPDR = 0x5A;                         // the slave shifts its answer in
        SPSR |= (1 << SPIF);                 // transfer complete
        io::SPI::transferCompleteServiceRoutine();   // the interrupt fires
    }
    l_send.join();
    CHECK((PORTB & (1 << 2)) != 0);          // SS released after completion

    printf("masterReceiveByte: dummy write clocks the slave answer in:\n");
    SPDR = 0xA5;
    uint8_t l_received = 0;
    std::thread l_receive([&l_received]() {
        getSPI().masterReceiveByte(l_received);
    });
    {
        for (uint32_t i = 0; (SPDR != 0x00) && (i < 100000000UL); i++) {}
        if (SPDR != 0x00)
        {
            printf("  FAIL: SPDR was never written with 0x00\n");
            failures++;
        }
        CHECK((PORTB & (1 << 2)) == 0);      // SS asserted while transferring
        SPDR = 0x77;                         // the slave shifts its answer in
        SPSR |= (1 << SPIF);
        io::SPI::transferCompleteServiceRoutine();
    }
    l_receive.join();
    CHECK((PORTB & (1 << 2)) != 0);
    CHECK(l_received == 0x77);

    printf("%s: %u failure(s)\n", failures ? "FAILED" : "PASSED", failures);
    return failures ? 1 : 0;
}
