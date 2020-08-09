#include "USART0.h"
#include <string.h>

volatile uint8_t io::USART0::m_status = 0;
volatile const uint8_t* io::USART0::mp_data2Send = nullptr;
volatile uint8_t* io::USART0::mp_dataReceived = nullptr;
uint8_t io::USART0::m_sizeData2Send = 0;
uint8_t io::USART0::m_sizeData2Receive = 0;
uint8_t io::USART0::m_numberBytesReceived = 0;
uint8_t io::USART0::m_numberBytesSent = 0;
uint8_t io::USART0::m_ready2Send = 1;




io::USART0& io::USART0::getInstance(const transmissionMode& ar_transMode,
                           const communicationMode& ar_comMode,
                           const frameSize& ar_frameSize,
                           const stopBit& ar_stopBit,
                           const parityMode& ar_parityMode)
{
    static USART0 l_instance(ar_transMode,
                             ar_comMode,
                             ar_frameSize,
                             ar_stopBit,
                             ar_parityMode);

    return l_instance;
}



io::USART0::USART0(const transmissionMode& ar_transMode,
                   const communicationMode& ar_comMode,
                   const frameSize& ar_frameSize,
                   const stopBit& ar_stopBit,
                   const parityMode& ar_parityMode)

{

    setBaudRate();
    setTransmissionMode(ar_transMode);
    setCommunicationMode(ar_comMode);
    setParityMode(ar_parityMode);
    setFrameSize(ar_frameSize);
    setStopBit(ar_stopBit);
    sei();
    enableReceiveCompleteInterrupt(1);
}

io::USART0::~USART0()
{

}

void io::USART0::setBaudRate()
{
    USART0_SET_BAUDRATE_HIGH_REGISTER;
    USART0_SET_BAUDRATE_LOW_REGISTER;

}

void io::USART0::setTransmissionMode(const transmissionMode& ar_transMode)
{
    switch (ar_transMode)
    {
        case transmissionMode::async:
        {
            USART0_ENABLE_ASYNC_TRANSMISSION_MODE;
            break;
        }
        case transmissionMode::sync:
        {
            USART0_DISABLE_DOUBLE_SPEED_MODE;
            USART0_ENABLE_SYNC_TRANSMISSION_MODE;
            break;
        }
    case transmissionMode::masterSPI:
        {
            USART0_ENABLE_MASTER_SPI_MODE;
            break;
    }
    }


}

void io::USART0::setCommunicationMode(const communicationMode &ar_comMode)
{
    switch (ar_comMode)
    {
        case communicationMode::duplex:
        {
            USART0_ENABLE_TRANSMITTER;
            USART0_ENABLE_RECEIVER;
            break;
        }
        case communicationMode::receive:
        {
            USART0_ENABLE_RECEIVER;
            USART0_DISABLE_TRANSMITTER;
            break;
        }
        case communicationMode::transmit:
        {
            USART0_ENABLE_TRANSMITTER;
            USART0_DISABLE_RECEIVER;            
            break;
        }
    }

}

void io::USART0::setParityMode(const parityMode& ar_parityMode)
{
    switch (ar_parityMode)
    {
        case parityMode::noParity:
        {
            USART0_DISABLE_PARITY_MODE;
            break;
        }
        case parityMode::evenParity:
        {
            USART0_ENABLE_EVEN_PARITY_MODE;
            break;
        }
        case parityMode::oddParity:
        {
            USART0_ENABLE_ODD_PARITY_MODE;
            break;
        }
    }

}

void io::USART0::setFrameSize(const frameSize& ar_frameSize)
{
    switch (ar_frameSize)
    {
        case frameSize::eightBits:
        {
            USART0_SET_8BIT_FRAME_SIZE;
            break;
        }
        case frameSize::sevenBits:
        {
            USART0_SET_7BIT_FRAME_SIZE;
            break;
        }
        case frameSize::sixBits:
        {
            USART0_SET_6BIT_FRAME_SIZE;
            break;
        }
        case frameSize::fiveBits:
        {
            USART0_SET_5BIT_FRAME_SIZE;
            break;
        }
        case frameSize::neineBits:
        {
            USART0_SET_9BIT_FRAME_SIZE;
            break;
        }
    }

}
void io::USART0::setStopBit(const stopBit& ar_stopBit)
{
    switch (ar_stopBit)
    {
        case stopBit::oneStopBit:
        {
            USART0_SET_ONE_STOP_BIT;
            break;
        }
        case stopBit::twoStopBits:
        {
            USART0_SET_TWO_STOP_BITS;
            break;
        }
    }

}


uint8_t io::USART0::isFrameError()
{
    return (m_status & (1 << USART0_FRAME_ERROR));

}

uint8_t io::USART0::isDataOverrun()
{
    return (m_status & (1 << USART0_DATA_OVERRUN));

}

uint8_t io::USART0::isParityError()
{
    return (m_status & (1 << USART0_PARITY_ERROR));

}

void io::USART0::sendFrame(const uint8_t* ap_dataBuffer, const uint8_t a_size)
{
    m_sizeData2Send = a_size;
    mp_data2Send = ap_dataBuffer;
    enableDataRegisterEmptyInterrupt(1);
}

void io::USART0::sendString(const char *ap_string)
{
    m_sizeData2Send = static_cast<uint8_t>(strlen(ap_string));
    mp_data2Send = reinterpret_cast<const uint8_t*>(ap_string);
    enableDataRegisterEmptyInterrupt(1);

}

void io::USART0::sendByte(uint8_t a_byte)
{
    m_sizeData2Send = 1;
    mp_data2Send = &a_byte;
    enableDataRegisterEmptyInterrupt(1);

}

void io::USART0::
receiveFrame(uint8_t *ap_dataBuffer, const uint8_t a_size)
{
    m_sizeData2Receive = a_size;
    mp_dataReceived = ap_dataBuffer;



}

void io::USART0::receiveCompleteServiceRoutine()
{
    static volatile uint8_t *lp_dataReceived = mp_dataReceived;
    static uint8_t l_dataSize = m_sizeData2Receive;

    m_status = USART0_CONTROL_STATUS_REGISTER;



        if (l_dataSize)
        {

            *lp_dataReceived++ = USART0_DATA_REGISTER;
            l_dataSize--;
            m_numberBytesReceived++;

        }
        else
        {
            l_dataSize = m_sizeData2Receive;
            lp_dataReceived = mp_dataReceived;
        }

}

void io::USART0::dataRegisterEmptyServiceRoutine()
{

    if (m_sizeData2Send)
    {
        m_ready2Send = 0;
        USART0_DATA_REGISTER = *mp_data2Send++;
        m_sizeData2Send--;
        m_numberBytesSent++;

    }
    else
    {
        enableDataRegisterEmptyInterrupt(0);
        m_numberBytesSent = 0;
        m_ready2Send = 1;

    }

}


void io::USART0::enableTransmitCompleteInterrupt(const uint8_t ar_enable)
{
    if (ar_enable) {
        USART0_ENABLE_TRANSMIT_COMPLETE_INTERRUPT;
    } else {
        USART0_DISABLE_TRANSMIT_COMPLETE_INTERRUPT;
    }

}

void io::USART0::enableReceiveCompleteInterrupt(const uint8_t ar_enable)
{
    if (ar_enable) {
        USART0_ENABLE_RECEIVE_COMPLETE_INTERRUPT;

    } else {
        USART0_DISABLE_RECEIVE_COMPLETE_INTERRUPT;
    }

}

void io::USART0::enableDataRegisterEmptyInterrupt(const uint8_t ar_enable)
{
    if (ar_enable) {
        USART0_ENABLE_DATA_REGISTER_EMPTY_INTERRUPT;

    } else {
        USART0_DISABLE_DATA_REGISTER_EMPTY_INTERRUPT;
    }

}

void io::USART0::transmitCompleteServiceRoutine()
{

}

uint8_t io::USART0::getNumberBytesSent()
{
    return m_numberBytesSent;
}

uint8_t io::USART0::getNumberBytesReceived()
{
    return m_numberBytesReceived;
}

void io::USART0::resetNumberBytesReceived()
{
    m_numberBytesReceived = 0;
}

uint8_t io::USART0::ready2Send()
{
    return m_ready2Send;

}

