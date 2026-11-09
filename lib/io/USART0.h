#ifndef USART_H
#define USART_H
#include "ha_base.h"

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


/**
 * @brief USART0 transmission mode
 *
 * @enum io::transmissionMode
 *
 * @author Farid Oubbati (farid.oubbati@outlook.com)
 * @date March 2018
*/
enum class transmissionMode : uint8_t {
    async=0,    /**< asynchronous mode */
    sync,       /**< synchronous mode */
    masterSPI   /**< masterSPI mode */
};

/**
 * @brief USART0 communication mode
 *
 * @enum io::communicationMode
 *
 * @author Farid Oubbati (farid.oubbati@outlook.com)
 * @date March 2018
*/
enum class communicationMode : uint8_t {
    duplex=0,   /**< full duplex mode */
    transmit,   /**< transmit mode */
    receive,    /**< receive mode */

};

/**
 * @brief USART0 parity mode
 *
 * @enum io::parityMode
 *
 * @author Farid Oubbati (farid.oubbati@outlook.com)
 * @date March 2018
*/
enum class parityMode : uint8_t {
    noParity=0, /**< no parity check mode */
    evenParity, /**< even parity check mode */
    oddParity /**< odd parity check mode */
};

/**
 * @brief USART0 frame size
 *
 * @enum io::frameSize
 *
 * @author Farid Oubbati (farid.oubbati@outlook.com)
 * @date March 2018
*/
enum class frameSize : uint8_t {
    eightBits=0, /**< 8 bits frame size */
    fiveBits,   /**< 5 bits frame size */
    sixBits, /**< 6 bits frame size */
    sevenBits, /**< 7 bits frame size */
    neineBits /**< 9 bits frame size */
};

/**
 * @brief USART0 stop bit
 *
 * @enum io::stopBit
 *
 * @author Farid Oubbati (farid.oubbati@outlook.com)
 * @date March 2018
*/
enum class stopBit : uint8_t{
    oneStopBit=0, /**< 1 stop bit */
    twoStopBits /**< 2 stop bits */
};

/**
 * @brief Class for handling USART0
 *
 * The class implements USART0 operations
 *
 * @author Farid Oubbati (farid.oubbati@outlook.com)
 * @date March 2018
*/

class USART0
{

public:


    /**
     *  Create a single instance of the USART0 object
     *
     *  @param ar_transMode Defines transmission mode
     *  @param ar_comMode Defines communication mode
     *  @param ar_frameSize Defines data frame size
     *  @param ar_stopBit Defines number of stop bits
     *  @param ar_parityMode Defines parity mode
     *
     *  @note The parameters are only honored by the first call.  Every later
     *  call returns the existing instance and ignores its arguments.
     */
    static USART0& getInstance(const transmissionMode& ar_transMode = transmissionMode::async,
                               const communicationMode& ar_comMode = communicationMode::duplex,
                               const frameSize& ar_frameSize = frameSize::eightBits,
                               const stopBit& ar_stopBit = stopBit::oneStopBit,
                               const parityMode& ar_parityMode = parityMode::noParity);
    /**
     *  Set baud rate.
     *
     */
    void setBaudRate();
    /**
     *  Set transnmission mode
     *
     *  @param ar_transMode Defines transmission mode
     */
    void setTransmissionMode(const transmissionMode& ar_transMode);
    /**
     *  Set communication mode
     *
     *  @param ar_comMode Defines communication mode
     */
    void setCommunicationMode(const communicationMode& ar_comMode);
    /**
     *  Set parity mode in data frame
     *
     *  @param ar_parityMode Defines parity mode
     */
    void setParityMode(const parityMode& ar_parityMode);
    /**
     *  Set data frame size
     *
     *  @param ar_frameSize Defines data frame size
     */
    void setFrameSize(const frameSize& ar_frameSize);
    /**
     *  Set number of stop bits in data frame
     *
     *  @param ar_stopBit Defines number of stop bits
     */
    void setStopBit(const stopBit& ar_stopBit);
    /**
     *  Transmit data frame
     *
     *  Claims the transmitter and enables the data-register-empty
     *  interrupt; ready2Send() is cleared synchronously before the
     *  interrupt is enabled, and getNumberBytesSent() is restarted
     *  at 0.
     *
     *  @param ap_dataBuffer Defines pointer to transmitter buffer
     *  @param a_size Defines size of transmitter buffer
     *
     *  @note ap_dataBuffer must outlive the transmission: the transmit
     *  ISR reads from it until all a_size bytes have been sent.
     */
    void sendFrame(const uint8_t *ap_dataBuffer, const uint8_t a_size);

    /**
     *  Transmit string
     *
     *  @param ap_string Defines pointer to string
     *
     *  @note ap_string must outlive the transmission (string literals
     *  are fine, they live in program storage for the whole program).
     */
    void sendString(const char *ap_string);

    /**
     *  Receive string
     *
     *  Arms the receiver for a NUL-terminated string.  Reception stops when
     *  a NUL byte is stored, or after 255 bytes if no NUL arrives.
     *
     *  @param ap_string Defines pointer to string
     *
     *  @note The signature keeps the const qualifier of the original API,
     *  but the ISR writes received bytes into the buffer, so ap_string must
     *  point to modifiable storage and must never be a string literal.  The
     *  buffer must outlive the reception.
     */
    void receiveString(const char *ap_string);

    /**
     *  Transmit character
     *
     *  The byte is copied into internal storage, so ar_char does not
     *  have to outlive the call.
     *
     *  @param ar_char Defines character to be sent
     */
    void sendChar(const uint8_t &ar_char);

    /**
     *  Transmit byte
     *
     *  Always transmits 3 characters, zero-padded: sendByte(5) sends
     *  "005", sendByte(255) sends "255".
     *
     *  @param ar_byte Defines byte to be sent
     */
    void sendByte(const uint8_t &ar_byte);

    /**
     *  Transmit word
     *
     *  Always transmits 5 characters, zero-padded: sendWord(5) sends
     *  "00005".
     *
     *  @param ar_word Defines word to be sent
     */
    void sendWord(const uint16_t &ar_word);

    /**
     *  Transmit long word
     *
     *  Always transmits 10 characters, zero-padded: sendLong(5) sends
     *  "0000000005".
     *
     *  @param ar_long Defines long word to be sent
     */
    void sendLong(const uint32_t &ar_long);

    /**
     *  Receive character
     *
     *  Arms the receiver for a single byte.  The byte is delivered by the
     *  receive-complete ISR; poll getNumberBytesReceived() to know when it
     *  has arrived.
     *
     *  @param ar_char Defines character to be received
     *
     *  @note ar_char must outlive the reception: the ISR stores into it
     *  until a byte has been received.
     */
    void receiveChar(uint8_t &ar_char);

    /**
     *  Receive data frame
     *
     *  Arms the receiver for a_size bytes.  Bytes are delivered by the
     *  receive-complete ISR; poll getNumberBytesReceived() for the number
     *  of bytes stored so far.
     *
     *  @param ap_dataBuffer Defines pointer to receiver buffer
     *  @param a_size Defines size of receiver buffer
     *
     *  @note ap_dataBuffer must outlive the reception: the ISR keeps
     *  writing into it until all a_size bytes have been received.
     */
    void receiveFrame(uint8_t *ap_dataBuffer, const uint8_t a_size);

    /**
     *  Enable transmit complete interrupt
     *
     *  @param ar_enable Indicates if interrupt is enabled
     */
    void enableTransmitCompleteInterrupt(const uint8_t a_enable);

    /**
     *  Enable receive complete interrupt
     *
     *  @param ar_enable Indicates if interrupt is enabled
     */
    void enableReceiveCompleteInterrupt(const uint8_t a_enable);

    /**
     *  Enable data register empty interrupt
     *
     *  @param ar_enable Indicates if interrupt is enabled
     */
    static void enableDataRegisterEmptyInterrupt(const uint8_t a_enable);

    /**
     * Is there frame error in received data
     *
     * @return  Status of received data
     */
    uint8_t frameError();

    /**
     * Is there data overrun in received data
     *
     * @return  Status of received data
     */
    uint8_t dataOverrun();

    /**
     *  Is there partity error in received data
     *
     *  @return  Status of received data
     */
    uint8_t parityError();
    /**
     *  Get number of bytes received
     *
     *  Counts the bytes stored by the receive-complete ISR, not the bytes
     *  that arrived on the wire: bytes received while no buffer is armed,
     *  or after the armed buffer is full, are dropped and not counted.
     *
     *  @return  Number of received bytes
     */
    uint16_t getNumberBytesReceived();
    /**
     *  Get number of bytes sent
     *
     *  Bytes sent by the current transmission; restarted at 0 at the
     *  start of each send.
     *
     *  @return  Number of transmitted bytes
     */
    uint16_t getNumberBytesSent();
    /**
     *  Is ready to send
     *
     *  @return  Sender status
     */
    uint8_t ready2Send();
    /** Reset number of bytes received
         */
    void resetNumberBytesReceived();
    /** Receive complete ISR
         */
    static void receiveCompleteServiceRoutine() __asm__(STR(USART0_RECEIVE_COMPLETE_INTERRUPT)) __attribute__((__signal__, __used__, __externally_visible__));
    /** Data register empty ISR
         */
    static void dataRegisterEmptyServiceRoutine() __asm__(STR(USART0_DATA_REGISTER_EMPTY_INTERRUPT)) __attribute__((__signal__,__used__, __externally_visible__));
    /** Transmit complete ISR
         */
    static void transmitCompleteServiceRoutine() __asm__(STR(USART0_TRANSMIT_COMPLETE_INTERRUPT)) __attribute__((__signal__, __used__, __externally_visible__));


protected:




private:
    /** Constructor.
     *
     *  Initalizes the USART0 object
     *
     *  @param ar_transMode Defines transmission mode
     *  @param ar_comMode Defines communication mode
     *  @param ar_frameSize Defines data frame size
     *  @param ar_stopBit Defines number of stop bits
     *  @param ar_parityMode Defines parity mode
     */
    USART0(const transmissionMode& ar_transMode,
           const communicationMode& ar_comMode,
           const frameSize& ar_frameSize,
           const stopBit& ar_stopBit,
           const parityMode& ar_parityMode);

    /** Destructor.
        */
    ~USART0();

    /**
     *  Overried Copy constructor.
     */
    USART0(const USART0&);

    /**
     *  Override assign operator.
     */
    const USART0& operator=(const USART0&);

    static volatile uint8_t m_status;   /**< Status of received data */

    static const uint8_t *mp_data2Send; /**< Pointer to transmitter buffer */

    static uint8_t *mp_data2Receive; /**< Pointer to the next free byte of the receiver buffer */

    static uint16_t m_sizeData2Send;  /**< Size of data to be transmitted */

    static volatile uint16_t m_sizeData2Receive;  /**< Number of bytes the ISR still stores into mp_data2Receive */

    static volatile uint8_t m_stringReceive;  /**< Receiving a NUL-terminated string: ISR disarms on NUL */

    static volatile uint16_t m_numberBytesReceived;   /**< Nnumber of received bytes */

    static volatile uint16_t m_numberBytesSent;   /**< Number of trasnmitted bytes */

    static volatile uint8_t m_ready2Send;   /**< Ready to send flag */


};



}











#endif
