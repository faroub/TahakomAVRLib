/**
 * @file SPI.h
 * @brief Header file of the SPI class
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/

#ifndef SPI_H
#define SPI_H
#include "ha_base.h"
#include "Pin.h"

namespace io
{

enum class operationMode : uint8_t {
    master=0,
    slave,
    submaster,
    disable,
};

enum class clockPrescaler : uint8_t {
    PS_4 = 0,
    PS_16,
    PS_64,
    PS_128,
    PS_2,
    PS_8,
    PS_32
};

enum class dataMode : uint8_t {
    mode_0 = 0,
    mode_1,
    mode_2,
    mode_3,
};

enum class dataOrder : uint8_t {
    first_MSB = 0,
    first_LSB
};

class SPI
{
public:
    static SPI& getInstance(const io::Pin &ar_pinSCK,
                            const io::Pin &ar_pinMISO,
                            const io::Pin &ar_pinMOSI,
                            const io::Pin &ar_pinSS);

    void selectDataMode(const dataMode& ar_dataMode);
    void selectDataOrder(const dataOrder& ar_dataOrder);
    void selectOperationMode(const operationMode& ar_operationMode);
    void selectClockPrescaler(const clockPrescaler& ar_clockPrescaler);
    void selectSlave(const uint8_t a_select);

    uint8_t writeCollision();
    uint8_t transferComplete();

    static void enableTransferCompleteInterrupt(const uint8_t a_enable);

    /**
     *  Send a byte to the slave as SPI master.
     *
     *  @param ar_byte Defines byte to be sent
     *  @return Status flag (1 = success, 0 = timeout/error)
     */
    uint8_t masterSendByte(const uint8_t &ar_byte);

    /**
     *  Receive a byte from the slave as SPI master.
     *
     *  @param ar_byte Defines byte to be received
     *  @return Status flag (1 = success, 0 = timeout/error)
     */
    uint8_t masterReceiveByte(uint8_t &ar_byte);

    void slaveReceiveByte(uint8_t &ar_byte);

    static void transferCompleteServiceRoutine() __asm__(STR(SPI_TRANSFER_COMPLETE_INTERRUPT)) __attribute__((__signal__, __used__, __externally_visible__));

private:
    SPI(const io::Pin &ar_pinSCK,
        const io::Pin &ar_pinMISO,
        const io::Pin &ar_pinMOSI,
        const io::Pin &ar_pinSS);

    ~SPI();
    SPI(const SPI&);
    const SPI& operator=(const SPI&);

    static volatile uint8_t m_data;
    static volatile uint8_t m_transferComplete;

    io::Pin m_pinSCK;
    io::Pin m_pinMISO;
    io::Pin m_pinMOSI;
    io::Pin m_pinSS;
};

}
#endif // SPI_H
