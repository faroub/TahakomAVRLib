/**
 * @file test_USART0.cpp
 * @brief Host test of the USART0 receive machinery
 *
 * Register-level host test: the device registers used here (UDR0, UCSR0A,
 * UCSR0B, UCSR0C, UBRR0H/0L, PRR) are defined as plain globals in this
 * file; the stub headers under stubs/ supply the ATmega328P bit
 * constants.  The receive-complete ISR is driven by calling it directly
 * with UDR0 preloaded, as the hardware would.
 */

#include <stdint.h>
#include <stdio.h>

#include "host_registers.h"

#include "USART0.h"

static uint8_t failures = 0;

#define CHECK(cond) \
    do { \
        if (!(cond)) { \
            printf("  FAIL: %s (line %d)\n", #cond, __LINE__); \
            failures++; \
        } \
    } while (0)

// feed one byte through the receive-complete ISR, like the RXC flag would
static void feedByte(io::USART0 &ar_usart, uint8_t a_byte)
{
    UCSR0A = (1 << RXC0);       // receive-complete flag set
    UDR0 = a_byte;              // the byte is in the data register
    ar_usart.receiveCompleteServiceRoutine();
}

int main(void)
{
    io::USART0 &usart = io::USART0::getInstance();

    printf("receive frame of 4 bytes lands in the buffer in order:\n");
    uint8_t l_buffer[4] = {0xFF, 0xFF, 0xFF, 0xFF};
    usart.resetNumberBytesReceived();
    usart.receiveFrame(l_buffer, 4);
    feedByte(usart, 0x41);
    feedByte(usart, 0x42);
    feedByte(usart, 0x43);
    feedByte(usart, 0x44);
    printf("  buffer = 0x%02X 0x%02X 0x%02X 0x%02X\n",
           l_buffer[0], l_buffer[1], l_buffer[2], l_buffer[3]);
    CHECK(l_buffer[0] == 0x41);
    CHECK(l_buffer[1] == 0x42);
    CHECK(l_buffer[2] == 0x43);
    CHECK(l_buffer[3] == 0x44);
    CHECK(usart.getNumberBytesReceived() == 4);

    printf("a fifth byte is dropped once the armed buffer is full:\n");
    uint8_t l_dropped[4] = {0, 0, 0, 0};
    feedByte(usart, 0x45);
    CHECK(l_dropped[0] == 0);
    CHECK(usart.getNumberBytesReceived() == 4);

    printf("UDR0 is drained even with no buffer armed:\n");
    UDR0 = 0x00;    // would leave garbage feeding the next armed receive
    feedByte(usart, 0x46);
    CHECK(usart.getNumberBytesReceived() == 4);

    printf("re-arming a frame before the old one is done restarts at the new buffer:\n");
    uint8_t l_second[2] = {0, 0};
    usart.receiveFrame(l_second, 2);
    feedByte(usart, 0x31);
    feedByte(usart, 0x32);
    CHECK(l_second[0] == 0x31);
    CHECK(l_second[1] == 0x32);
    CHECK(usart.getNumberBytesReceived() == 6);

    printf("receiveChar delivers one byte:\n");
    uint8_t l_char = 0;
    usart.resetNumberBytesReceived();
    usart.receiveChar(l_char);
    feedByte(usart, 0x5A);
    CHECK(l_char == 0x5A);
    CHECK(usart.getNumberBytesReceived() == 1);

    printf("receiveString stops at the NUL terminator:\n");
    char l_string[8] = {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11};
    usart.resetNumberBytesReceived();
    usart.receiveString(l_string);
    feedByte(usart, 'h');
    feedByte(usart, 'i');
    feedByte(usart, 0x00);
    CHECK(l_string[0] == 'h');
    CHECK(l_string[1] == 'i');
    CHECK(l_string[2] == 0x00);
    CHECK(usart.getNumberBytesReceived() == 3);
    // frame was disarmed: following bytes must not overflow the buffer
    feedByte(usart, 'x');
    CHECK(l_string[3] == 0x11);
    CHECK(usart.getNumberBytesReceived() == 3);

    printf("%s: %u failure(s)\n", failures ? "FAILED" : "PASSED", failures);
    return failures ? 1 : 0;
}
