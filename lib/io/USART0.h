#ifndef USART_H
#define USART_H
#include "ha_base.h"

#if defined(TAHAKOM_HAS_USART0) && (TAHAKOM_HAS_USART0 == 0)
    #error "Hardware USART0 is not supported on the target MCU (e.g. ATtiny85)."
#endif

/**
 *
 * @brief AVR chip internal i/o components
 *
 * The namespace englobes all internal input/output components that, when configured,
 * allows the AVR MCU to communicate with the external World
 *
*/
namespace io
{

enum class transmissionMode : uint8_t {
    async=0,    /**< asynchronous mode */
    sync,       /**< synchronous mode */
    masterSPI   /**< masterSPI mode */
};

enum class communicationMode : uint8_t {
    duplex=0,   /**< full duplex mode */
    transmit,   /**< transmit mode */
    receive,    /**< receive mode */
};

enum class parityMode : uint8_t {
    noParity=0, /**< no parity check mode */
    evenParity, /**< even parity check mode */
    oddParity   /**< odd parity check mode */
};

enum class frameSize : uint8_t {
    eightBits=0, /**< 8 bits frame size */
    fiveBits,   /**< 5 bits frame size */
    sixBits,    /**< 6 bits frame size */
    sevenBits,  /**< 7 bits frame size */
    neineBits   /**< 9 bits frame size */
};

enum class stopBit : uint8_t {
    oneStopBit=0, /**< 1 stop bit */
    twoStopBits   /**< 2 stop bits */
};

class USART0
{
public:
    static USART0& getInstance(const transmissionMode& ar_transMode = transmissionMode::async,
                               const communicationMode& ar_comMode = communicationMode::duplex,
                               const frameSize& ar_frameSize = frameSize::eightBits,
                               const stopBit& ar_stopBit = stopBit::oneStopBit,
                               const parityMode& ar_parityMode = parityMode::noParity);

    void setBaudRate();
    void setTransmissionMode(const transmissionMode& ar_transMode);
    void setCommunicationMode(const communicationMode& ar_comMode);
    void setParityMode(const parityMode& ar_parityMode);
    void setFrameSize(const frameSize& ar_frameSize);
    void setStopBit(const stopBit& ar_stopBit);

    void sendFrame(const uint8_t *ap_dataBuffer, const uint8_t a_size);
    void sendString(const char *ap_string);
    void sendChar(const uint8_t &ar_char);
    void sendByte(const uint8_t &ar_byte);
    void sendWord(const uint16_t &ar_word);
    void sendLong(const uint32_t &ar_long);

    /**
     *  Capacity-aware String Reception (Safe API)
     *
     *  @param ap_buffer Destination modifiable character buffer
     *  @param a_capacity Maximum capacity of buffer including NUL terminator
     */
    void receiveString(char *ap_buffer, uint8_t a_capacity);

    /**
     *  Legacy String Reception (Deprecated)
     */
    void receiveString(const char *ap_string);

    void receiveChar(uint8_t &ar_char);
    void receiveFrame(uint8_t *ap_dataBuffer, const uint8_t a_size);

    void enableTransmitCompleteInterrupt(const uint8_t a_enable);
    void enableReceiveCompleteInterrupt(const uint8_t a_enable);
    static void enableDataRegisterEmptyInterrupt(const uint8_t a_enable);

    uint8_t frameError();
    uint8_t dataOverrun();
    uint8_t parityError();
    uint8_t hasOverflowed();

    uint16_t getNumberBytesReceived();
    uint16_t getNumberBytesSent();
    uint8_t ready2Send();
    void resetNumberBytesReceived();

    static void receiveCompleteServiceRoutine() __asm__(STR(USART0_RECEIVE_COMPLETE_INTERRUPT)) __attribute__((__signal__, __used__, __externally_visible__));
    static void dataRegisterEmptyServiceRoutine() __asm__(STR(USART0_DATA_REGISTER_EMPTY_INTERRUPT)) __attribute__((__signal__,__used__, __externally_visible__));
    static void transmitCompleteServiceRoutine() __asm__(STR(USART0_TRANSMIT_COMPLETE_INTERRUPT)) __attribute__((__signal__, __used__, __externally_visible__));

private:
    USART0(const transmissionMode& ar_transMode,
           const communicationMode& ar_comMode,
           const frameSize& ar_frameSize,
           const stopBit& ar_stopBit,
           const parityMode& ar_parityMode);

    ~USART0();
    USART0(const USART0&);
    const USART0& operator=(const USART0&);

    static volatile uint8_t m_status;
    static const uint8_t *mp_data2Send;
    static uint8_t *mp_data2Receive;
    static uint16_t m_sizeData2Send;
    static volatile uint16_t m_sizeData2Receive;
    static volatile uint8_t m_stringReceive;
    static volatile uint8_t m_overflow;
    static volatile uint16_t m_numberBytesReceived;
    static volatile uint16_t m_numberBytesSent;
    static volatile uint8_t m_ready2Send;
};

}

#endif
