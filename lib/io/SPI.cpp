/**
 * @file SPI.cpp
 * @brief Implementation of the SPI hardware driver
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/

#include "SPI.h"
#include "MCU.h"

volatile uint8_t io::SPI::m_data = 0;
volatile uint8_t io::SPI::m_transferComplete = 0;

#define SPI_TIMEOUT_CYCLES 10000U

io::SPI& io::SPI::getInstance(const Pin &ar_pinSCK,
                              const Pin &ar_pinMISO,
                              const Pin &ar_pinMOSI,
                              const Pin &ar_pinSS)
{
    static SPI l_instance(ar_pinSCK,
                          ar_pinMISO,
                          ar_pinMOSI,
                          ar_pinSS);
    return l_instance;
}

io::SPI::SPI(const Pin &ar_pinSCK,
             const Pin &ar_pinMISO,
             const Pin &ar_pinMOSI,
             const Pin &ar_pinSS)
        : m_pinSCK(ar_pinSCK),
          m_pinMISO(ar_pinMISO),
          m_pinMOSI(ar_pinMOSI),
          m_pinSS(ar_pinSS)
{
    core::MCU::enableSPI(1);
    enableTransferCompleteInterrupt(1);
}

io::SPI::~SPI()
{
}

uint8_t io::SPI::writeCollision()
{
    return (SPI_STATUS_REGISTER & (1 << SPI_WRITE_COLLISION)) ? 1 : 0;
}

uint8_t io::SPI::transferComplete()
{
    return (SPI_STATUS_REGISTER & (1 << SPI_TRANSFER_COMPLETE)) ? 1 : 0;
}

void io::SPI::selectClockPrescaler(const clockPrescaler& ar_clockPrescaler)
{
    SPI_SELECT_CLOCK_PRESCALER(static_cast<uint8_t>(ar_clockPrescaler));
}

void io::SPI::selectOperationMode(const operationMode& ar_operationMode)
{
    switch (ar_operationMode)
    {
        case operationMode::master:
            m_pinMOSI.toOutput();
            m_pinSCK.toOutput();
            m_pinMISO.toInput(1);
            m_pinSS.toOutput();
            m_pinSS.setHigh();
            SPI_SELECT_MASTER_MODE;
            SPI_ENABLE;
            break;
        case operationMode::slave:
            m_pinMISO.toOutput();
            SPI_SELECT_SLAVE_MODE;
            SPI_ENABLE;
            break;
        case operationMode::submaster:
            m_pinMOSI.toOutput();
            m_pinSCK.toOutput();
            m_pinMISO.toInput(1);
            m_pinSS.toInput(1);
            SPI_SELECT_MASTER_MODE;
            SPI_ENABLE;
            break;
        case operationMode::disable:
            SPI_DISABLE;
            break;
    }
}

void io::SPI::selectDataMode(const dataMode& ar_dataMode)
{
    SPI_SELECT_DATA_MODE(static_cast<uint8_t>(ar_dataMode));
}

void io::SPI::selectDataOrder(const dataOrder& ar_dataOrder)
{
    SPI_SELECT_DATA_ORDER(static_cast<uint8_t>(ar_dataOrder));
}

void io::SPI::enableTransferCompleteInterrupt(const uint8_t a_enable)
{
    if (a_enable) {
        SPI_ENABLE_TRANSFER_COMPLETE_INTERRUPT;
    } else {
        SPI_DISABLE_TRANSFER_COMPLETE_INTERRUPT;
    }
}

void io::SPI::selectSlave(const uint8_t a_select)
{
    if (a_select) {
        m_pinSS.setLow();
    } else {
        m_pinSS.setHigh();
    }
}

void io::SPI::transferCompleteServiceRoutine()
{
    m_data = SPI_DATA_REGISTER;
    m_transferComplete = 1;
}

uint8_t io::SPI::masterSendByte(const uint8_t &ar_byte)
{
    selectSlave(1);
    m_transferComplete = 0;
    SPI_DATA_REGISTER = ar_byte;

    uint16_t timeout = SPI_TIMEOUT_CYCLES;
    while (m_transferComplete == 0 && --timeout) {}

    // ALWAYS deassert SS on exit path
    selectSlave(0);

    return (timeout > 0) ? 1 : 0;
}

uint8_t io::SPI::masterReceiveByte(uint8_t &ar_byte)
{
    uint8_t status = masterSendByte(0x00);
    if (status) {
        ar_byte = m_data;
        return 1;
    }
    ar_byte = 0;
    return 0;
}

void io::SPI::slaveReceiveByte(uint8_t &ar_byte)
{
    ar_byte = m_data;
}
