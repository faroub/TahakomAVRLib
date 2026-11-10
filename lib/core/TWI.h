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

namespace core
{

class TWI
{
public:
    /** Get singleton instance of the TWI hardware manager
     *
     *  @param a_clockFreq_hz Requested I2C clock rate (default 100 kHz)
     */
    static TWI& getInstance(uint32_t a_clockFreq_hz = 100000UL);

    /** Initialize or change I2C clock frequency
     *
     *  @param a_clockFreq_hz Requested I2C clock rate in Hz
     */
    void setClock(uint32_t a_clockFreq_hz);

    /** Transmit START condition on I2C bus
     *
     *  @return Status flag (1 = success, 0 = error)
     */
    uint8_t start();

    /** Transmit STOP condition on I2C bus
     */
    void stop();

    /** Write byte to I2C bus
     *
     *  @param a_data Byte to send
     *  @return Status flag (1 = ACK received, 0 = NACK/error)
     */
    uint8_t write(uint8_t a_data);

    /** Read byte from I2C bus and send ACK
     *
     *  @return Received byte
     */
    uint8_t readAck();

    /** Read byte from I2C bus and send NACK
     *
     *  @return Received byte
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
