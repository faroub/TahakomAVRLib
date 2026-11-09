/**
 *
 * @author Farid Oubbati (farid.oubbati@outlook.com)
 * @date March 2018
 *
 *
 * See USART0.h for a description of this code
*/

#include "USART0.h"
#include "MCU.h"
#include <util/atomic.h>
#include <string.h>

volatile uint8_t io::USART0::m_status = 0;
const uint8_t* io::USART0::mp_data2Send = nullptr;
uint8_t* io::USART0::mp_data2Receive = nullptr;
uint16_t io::USART0::m_sizeData2Send = 0;
volatile uint16_t io::USART0::m_sizeData2Receive = 0;
volatile uint8_t io::USART0::m_stringReceive = 0;
volatile uint16_t io::USART0::m_numberBytesReceived = 0;
volatile uint16_t io::USART0::m_numberBytesSent = 0;
volatile uint8_t io::USART0::m_ready2Send = 1;




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
    core::MCU::enableUSART0(1);
    setBaudRate();
    setTransmissionMode(ar_transMode);
    setCommunicationMode(ar_comMode);
    setParityMode(ar_parityMode);
    setFrameSize(ar_frameSize);
    setStopBit(ar_stopBit);
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


uint8_t io::USART0::frameError()
{
    return (m_status & (1 << USART0_FRAME_ERROR));

}

uint8_t io::USART0::dataOverrun()
{
    return (m_status & (1 << USART0_DATA_OVERRUN));

}

uint8_t io::USART0::parityError()
{
    return (m_status & (1 << USART0_PARITY_ERROR));

}

void io::USART0::sendFrame(const uint8_t* ap_dataBuffer, const uint8_t a_size)
{
    while (!ready2Send()){};
    // claimed here, before the interrupt is enabled, so a second send cannot
    // observe a ready transmitter while this one is still running
    m_numberBytesSent = 0;
    m_sizeData2Send = a_size;
    mp_data2Send = ap_dataBuffer;
    m_ready2Send = 0;
    enableDataRegisterEmptyInterrupt(1);
}


void io::USART0::sendString(const char* ap_string)
{
    while (!ready2Send()){};
    m_numberBytesSent = 0;
    m_sizeData2Send = strlen(ap_string);
    mp_data2Send = reinterpret_cast<const uint8_t*>(ap_string);
    m_ready2Send = 0;
    enableDataRegisterEmptyInterrupt(1);

}


void io::USART0::sendByte(const uint8_t &ar_byte)
{
    static uint8_t l_byte2Send[3];
    l_byte2Send[0] = '0' + (ar_byte / 100);
    l_byte2Send[1] = '0' + ((ar_byte / 10) % 10);
    l_byte2Send[2] = '0' + (ar_byte % 10);
    sendFrame(l_byte2Send, 3);

}
void io::USART0::sendLong(const uint32_t &ar_long)
{
    static uint8_t l_word2Send[10];
    l_word2Send[0] = '0' + (ar_long / 1000000000);
    l_word2Send[1] = '0' + ((ar_long / 100000000) % 10);
    l_word2Send[2] = '0' + ((ar_long /  10000000) % 10);
    l_word2Send[3] = '0' + ((ar_long /   1000000) % 10);
    l_word2Send[4] = '0' + ((ar_long /    100000) % 10);
    l_word2Send[5] = '0' + ((ar_long /     10000) % 10);
    l_word2Send[6] = '0' + ((ar_long /      1000) % 10);
    l_word2Send[7] = '0' + ((ar_long /       100) % 10);
    l_word2Send[8] = '0' + ((ar_long /        10) % 10);
    l_word2Send[9] = '0' + (ar_long % 10);
    sendFrame(l_word2Send, 10);

}
void io::USART0::sendWord(const uint16_t &ar_word)
{
    static uint8_t l_word2Send[5];
    l_word2Send[0] = '0' + (ar_word / 10000);
    l_word2Send[1] = '0' + ((ar_word / 1000) % 10);
    l_word2Send[2] = '0' + ((ar_word /  100) % 10);
    l_word2Send[3] = '0' + ((ar_word /   10) % 10);
    l_word2Send[4] = '0' + (ar_word %    10);
    sendFrame(l_word2Send, 5);

}

void io::USART0::sendChar(const uint8_t &ar_char)
{
    // copied into internal storage: ar_char does not have to outlive the call
    static uint8_t l_char2Send = 0;
    l_char2Send = ar_char;
    sendFrame(&l_char2Send, 1);

}

void io::USART0::receiveChar(uint8_t &ar_char)
{
    receiveFrame(&ar_char, 1);
}


void io::USART0::receiveFrame(uint8_t *ap_dataBuffer, const uint8_t a_size)
{
    // arming is shared with the receive ISR, which runs at any priority
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        mp_data2Receive = ap_dataBuffer;
        m_sizeData2Receive = a_size;
        m_stringReceive = 0;
    }
}

void io::USART0::receiveString(const char *ap_string)
{
    // the const signature is kept for compatibility, but the receive ISR
    // stores into the buffer, so ap_string must be modifiable storage
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        mp_data2Receive = const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(ap_string));
        // no length parameter: bound the write by the receive counter and
        // stop after a NUL byte
        m_sizeData2Receive = 255;
        m_stringReceive = 1;
    }
}


void io::USART0::receiveCompleteServiceRoutine()
{
    m_status = USART0_CONTROL_STATUS_REGISTER;

    // always read UDR0 first, even with no buffer armed, so the hardware
    // receive register is drained and RXC is cleared
    const uint8_t l_data = USART0_DATA_REGISTER;

    if ((mp_data2Receive != nullptr) && (m_sizeData2Receive != 0))
    {
        *mp_data2Receive++ = l_data;
        m_sizeData2Receive--;
        m_numberBytesReceived++;

        if ((m_stringReceive != 0) && (l_data == 0))
        {
            // NUL terminator stored: string reception complete
            m_stringReceive = 0;
            m_sizeData2Receive = 0;
        }
    }

}

void io::USART0::dataRegisterEmptyServiceRoutine()
{

    if (m_sizeData2Send)
    {
        USART0_DATA_REGISTER = *mp_data2Send++;
        m_sizeData2Send--;
        m_numberBytesSent++;

    }
    else
    {
        enableDataRegisterEmptyInterrupt(0);
        m_ready2Send = 1;

    }

}


void io::USART0::enableTransmitCompleteInterrupt(const uint8_t a_enable)
{
    if (a_enable) {
        USART0_ENABLE_TRANSMIT_COMPLETE_INTERRUPT;
    } else {
        USART0_DISABLE_TRANSMIT_COMPLETE_INTERRUPT;
    }

}

void io::USART0::enableReceiveCompleteInterrupt(const uint8_t a_enable)
{
    if (a_enable) {
        USART0_ENABLE_RECEIVE_COMPLETE_INTERRUPT;

    } else {
        USART0_DISABLE_RECEIVE_COMPLETE_INTERRUPT;
    }

}

void io::USART0::enableDataRegisterEmptyInterrupt(const uint8_t a_enable)
{
    if (a_enable) {
        USART0_ENABLE_DATA_REGISTER_EMPTY_INTERRUPT;

    } else {
        USART0_DISABLE_DATA_REGISTER_EMPTY_INTERRUPT;
    }

}

void io::USART0::transmitCompleteServiceRoutine()
{

}

uint16_t io::USART0::getNumberBytesSent()
{
    return m_numberBytesSent;
}

uint16_t io::USART0::getNumberBytesReceived()
{
    return m_numberBytesReceived;
}

void io::USART0::resetNumberBytesReceived()
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        m_numberBytesReceived = 0;
    }
}

uint8_t io::USART0::ready2Send()
{
    return m_ready2Send;

}

