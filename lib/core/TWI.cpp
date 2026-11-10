/**
 * @file TWI.cpp
 * @brief Implementation of the Two-Wire Interface (I2C) class
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2026
*/

#include "TWI.h"
#include "MCU.h"

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
    // TWBR = ((F_CPU / clock) - 16) / 2
    if (a_clockFreq_hz == 0) a_clockFreq_hz = 100000UL;
    uint32_t twbr_val = ((F_CPU / a_clockFreq_hz) - 16UL) / 2UL;
    TWSR = 0; // Prescaler = 1
    TWBR = static_cast<uint8_t>(twbr_val);
}

uint8_t core::TWI::start()
{
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT))) {}
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
    while (!(TWCR & (1 << TWINT))) {}
    uint8_t status = TWSR & 0xF8;
    return (status == 0x18 || status == 0x28 || status == 0x40) ? 1 : 0;
}

uint8_t core::TWI::readAck()
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    while (!(TWCR & (1 << TWINT))) {}
    return TWDR;
}

uint8_t core::TWI::readNack()
{
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT))) {}
    return TWDR;
}
