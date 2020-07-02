#include "USART0.h"


volatile uint8_t io::USART0::m_status = 0;
volatile const uint8_t* io::USART0::mp_data2Send = nullptr;
volatile uint8_t* io::USART0::mp_dataReceived = nullptr;
volatile int16_t io::USART0::m_numberBytesSent = 0;
volatile int16_t io::USART0::m_numberBytesReceived = 0;
volatile int16_t io::USART0::m_DataSize = 0;







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
        case transmissionMode::Async:
        {
            USART0_ENABLE_ASYNC_TRANSMISSION_MODE;
            break;
        }
        case transmissionMode::Sync:
        {
            USART0_DISABLE_DOUBLE_SPEED_MODE;
            USART0_ENABLE_SYNC_TRANSMISSION_MODE;
            break;
        }
    case transmissionMode::MasterSPI:
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


bool io::USART0::isFrameError()
{
    return (m_status & (1 << USART0_FRAME_ERROR));

}

bool io::USART0::isDataOverrun()
{
    return (m_status & (1 << USART0_DATA_OVERRUN));

}

bool io::USART0::isParityError()
{
    return (m_status & (1 << USART0_PARITY_ERROR));

}

void io::USART0::sendFrame(volatile const uint8_t* ap_dataBuffer, volatile int16_t &ar_size)
{
    m_DataSize = ar_size;
    mp_data2Send = ap_dataBuffer;
    enableDataRegisterEmptyInterrupt(1);
}


void io::USART0::receiveFrame(volatile uint8_t *ap_dataBuffer, volatile int16_t &ar_size)
{
    m_DataSize = ar_size;
    mp_dataReceived = ap_dataBuffer;
    enableReceiverInterrupt(1);

}

void io::USART0::receiveCompleteServiceRoutine()
{
    m_status = USART0_CONTROL_STATUS_REGISTER;

    if (mp_dataReceived && (m_numberBytesReceived < m_DataSize))
    {
        *mp_dataReceived++ = USART0_DATA_REGISTER;
        m_numberBytesReceived++;

    }
    else
    {
        m_numberBytesReceived = 0;
        m_DataSize = 0;
        mp_dataReceived = nullptr;
        enableReceiverInterrupt(0);

    }


}

void io::USART0::dataRegisterEmptyServiceRoutine()
{

    if (mp_data2Send && (m_numberBytesSent < m_DataSize))
    {
        USART0_DATA_REGISTER = *mp_data2Send++;
        m_numberBytesSent++;

    }
    else
    {
        m_numberBytesSent = 0;
        m_DataSize = 0;
        mp_data2Send = nullptr;
        enableDataRegisterEmptyInterrupt(0);
    }
}


void io::USART0::enableTransmitterInterrupt(const uint8_t &ar_enable)
{
    if (ar_enable) {
        USART0_ENABLE_TRANSMITTER_INTERRUPT;
    } else {
        USART0_DISABLE_TRANSMITTER_INTERRUPT;
    }

}

void io::USART0::enableReceiverInterrupt(const uint8_t &ar_enable)
{
    if (ar_enable) {
        USART0_ENABLE_RECEIVER_INTERRUPT;

    } else {
        USART0_DISABLE_RECEIVER_INTERRUPT;
    }

}

void io::USART0::enableDataRegisterEmptyInterrupt(const uint8_t &ar_enable)
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

bool io::USART0::ready2Send()
{
    return mp_data2Send == nullptr;

}

bool io::USART0::ready2Receive()
{
    return mp_dataReceived == nullptr;
}
