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
 * @brief Class for handling USART0 component
 *
 * The class implements USART0 component operations
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
     *  @param ap_dataBuffer Defines pointer to transmitter buffer
     *  @param a_size Defines size of transmitter buffer
     */
    void sendFrame(const uint8_t *ap_dataBuffer, const uint8_t a_size);

    /**
     *  Transmit string
     *
     *  @param ap_string Defines pointer to string
     */
    void sendString(const char *ap_string);

    /**
     *  Receive string
     *
     *  @param ap_string Defines pointer to string
     */
    void receiveString(const char *ap_string);

    /**
     *  Transmit character
     *
     *  @param ar_char Defines character to be sent
     */
    void sendChar(const uint8_t &ar_char);

    /**
     *  Transmit byte
     *
     *  @param ar_byte Defines byte to be sent
     */
    void sendByte(const uint8_t &ar_byte);

    /**
     *  Transmit word
     *
     *  @param ar_word Defines word to be sent
     */
    void sendWord(const uint16_t &ar_word);

    /**
     *  Transmit long word
     *
     *  @param ar_long Defines long word to be sent
     */
    void sendLong(const uint32_t &ar_long);

    /**
     *  Receive character
     *
     *  @param ar_char Defines charcter to be received
     */
    void receiveChar(uint8_t &ar_char);

    /**
     *  Receive data frame
     *
     *  @param ap_dataBuffer Defines pointer to receiver buffer
     *  @param a_size Defines size of receiver buffer
     *  @param a_ready2Receive Indicates if chip ready to receive data
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
     *  @return  Number of received bytes
     */
    uint16_t getNumberBytesReceived();
    /**
     *  Get number of bytes sent
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
     *  @param ar_transMode defines transmission mode
     *  @param ar_comMode defines communication mode
     *  @param ar_frameSize defines data frame size
     *  @param ar_stopBit defines number of stop bits
     *  @param ar_parityMode defines parity mode
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

    static uint8_t *mp_data2Receive; /**< Pointer to receiver buffer */

    static uint16_t m_sizeData2Send;  /**< Size of data to be transmitted */

    static uint16_t m_sizeData2Receive;  /**< Size of data to be received */

    static volatile uint16_t m_numberBytesReceived;   /**< Nnumber of received bytes */

    static volatile uint16_t m_numberBytesSent;   /**< Number of trasnmitted bytes */

    static volatile uint8_t m_ready2Send;   /**< Ready to send flag */


};



}











#endif
