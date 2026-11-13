/**
 * @file TWI.h
 * @brief Header file for the Two-Wire Interface (I2C) class
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2026
*/

#ifndef TWI_H
#define TWI_H

#include "ha_base.h"

#if defined(TAHAKOM_HAS_TWI) && (TAHAKOM_HAS_TWI == 0)
    #error "Hardware TWI/I2C is not supported on the target MCU (e.g. ATtiny85)."
#endif

namespace core
{

class TWI
{
public:
    static TWI& getInstance(uint32_t a_clockFreq_hz = 100000UL);

    void setClock(uint32_t a_clockFreq_hz);

    /** Transmit START condition on I2C bus
     *  @return Status flag (1 = success, 0 = error/timeout)
     */
    uint8_t start();

    /** Transmit STOP condition on I2C bus
     *  @return Status flag (1 = success, 0 = error/timeout)
     */
    uint8_t stop();

    /** Write byte to I2C bus
     *  @param a_data Byte to send
     *  @return Status flag (1 = ACK received, 0 = NACK/error/timeout)
     */
    uint8_t write(uint8_t a_data);

    /** Read byte from I2C bus and send ACK (Status separated)
     *  @param ar_data Reference to output byte
     *  @return Status flag (1 = success, 0 = error/timeout)
     */
    uint8_t readAck(uint8_t &ar_data);

    /** Read byte from I2C bus and send NACK (Status separated)
     *  @param ar_data Reference to output byte
     *  @return Status flag (1 = success, 0 = error/timeout)
     */
    uint8_t readNack(uint8_t &ar_data);

    /** Legacy Read byte from I2C bus and send ACK
     *  @return Received byte (or 0xFF on timeout)
     */
    uint8_t readAck();

    /** Legacy Read byte from I2C bus and send NACK
     *  @return Received byte (or 0xFF on timeout)
     */
    uint8_t readNack();

private:
    TWI(uint32_t a_clockFreq_hz);
    ~TWI();
    TWI(const TWI&);
    const TWI& operator=(const TWI&);
};

}

#endif
