/**
 * @file TWI.cpp
 * @brief Implementation of the Two-Wire Interface (I2C) class
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2026
*/

#include "TWI.h"
#include "MCU.h"

#define TWI_TIMEOUT_CYCLES 10000U

core::TWI& core::TWI::getInstance(uint32_t a_clockFreq_hz)
{
    static TWI l_instance(a_clockFreq_hz);
    return l_instance;
}

core::TWI::TWI(uint32_t a_clockFreq_hz)
{
    core::MCU::enableTWI(1);
    setClock(a_clockFreq_hz);
}

core::TWI::~TWI()
{
}

void core::TWI::setClock(uint32_t a_clockFreq_hz)
{
    // Clamp clock frequency to valid range [10 kHz, 400 kHz]
    if (a_clockFreq_hz < 10000UL) a_clockFreq_hz = 10000UL;
    if (a_clockFreq_hz > 400000UL) a_clockFreq_hz = 400000UL;

    if (F_CPU < 16UL * a_clockFreq_hz) {
        TWBR = 0;
        TWSR = 0;
        return;
    }

    uint32_t twbr_val = ((F_CPU / a_clockFreq_hz) - 16UL) / 2UL;
    if (twbr_val > 255UL) twbr_val = 255UL;

    TWSR = 0; // Prescaler = 1
    TWBR = static_cast<uint8_t>(twbr_val);
}

uint8_t core::TWI::start()
{
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    uint16_t timeout = TWI_TIMEOUT_CYCLES;
    while (!(TWCR & (1 << TWINT)) && --timeout) {}
    if (timeout == 0) return 0; // Bus timeout

    uint8_t status = TWSR & 0xF8;
    return (status == 0x08 || status == 0x10) ? 1 : 0;
}

void core::TWI::stop()
{
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

uint8_t core::TWI::write(uint8_t a_data)
{
    TWDR = a_data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    uint16_t timeout = TWI_TIMEOUT_CYCLES;
    while (!(TWCR & (1 << TWINT)) && --timeout) {}
    if (timeout == 0) return 0; // Bus timeout

    uint8_t status = TWSR & 0xF8;
    return (status == 0x18 || status == 0x28 || status == 0x40) ? 1 : 0;
}

uint8_t core::TWI::readAck()
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    uint16_t timeout = TWI_TIMEOUT_CYCLES;
    while (!(TWCR & (1 << TWINT)) && --timeout) {}
    if (timeout == 0) return 0xFF;
    return TWDR;
}

uint8_t core::TWI::readNack()
{
    TWCR = (1 << TWINT) | (1 << TWEN);
    uint16_t timeout = TWI_TIMEOUT_CYCLES;
    while (!(TWCR & (1 << TWINT)) && --timeout) {}
    if (timeout == 0) return 0xFF;
    return TWDR;
}
