/**
 * @file USART0.h
 * @brief Header file of the USART0 class
 *
 * Basic class for IO abstraction of the Universal
 * Synchronous and Asynchronous serial Receiver and Transmitter
 * serial communication device.
 *
 * Usage example:
 *
 #include "USART0.h"
 #define BUFFER_SIZE 1

 int main(void) {
  // Init

  char l_receiverBuffer[BUFFER_SIZE];
  char l_transmitterBuffer[BUFFER_SIZE];

  uint8_t l_ready2Send = 0;
  uint8_t l_ready2Receive = 1;
  // instantiate the USART0 object
  io::USART0 &myUSART0 = io::USART0::getInstance();
  // Mainloop
  while (1) {
    if (l_ready2Send && !myUSART0.getNumberBytesSent())
    {
        myUSART0.sendFrame(reinterpret_cast<uint8_t*>(l_receiverBuffer),BUFFER_SIZE);
        l_ready2Send = 0;
    }
    myUSART0.receiveFrame(reinterpret_cast<uint8_t*>(l_receiverBuffer),BUFFER_SIZE,l_ready2Receive);
    if (myUSART0.getNumberBytesReceived())
    {
        // extract data received
        // .....
        // send back received data
        l_ready2Send = 1;
        // reset number of bytes after extracting the received data
        myUSART0.resetNumberBytesReceived();
    }
  }
  return 0;
 }
 *
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/
#ifndef USART_H
#define USART_H
#include "ha_base.h"


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
    oddParity /**< odd parity check mode */
};

enum class frameSize : uint8_t {
    eightBits=0, /**< 8 bits frame size */
    fiveBits,   /**< 5 bits frame size */
    sixBits, /**< 6 bits frame size */
    sevenBits, /**< 7 bits frame size */
    neineBits /**< 9 bits frame size */
};

enum class stopBit : uint8_t{
    oneStopBit=0, /**< 1 stop bit */
    twoStopBits /**< 2 stop bits */
};


class USART0
{

public:


    /** Create a single instance of the USART0 object
         *
         *  @param ar_transMode defines transmission mode
         *  @param ar_comMode defines communication mode
         *  @param ar_frameSize defines data frame size
         *  @param ar_stopBit defines number of stop bits
         *  @param ar_parityMode defines parity mode
         */
    static USART0& getInstance(const transmissionMode& ar_transMode = transmissionMode::async,
                               const communicationMode& ar_comMode = communicationMode::duplex,
                               const frameSize& ar_frameSize = frameSize::eightBits,
                               const stopBit& ar_stopBit = stopBit::oneStopBit,
                               const parityMode& ar_parityMode = parityMode::noParity);
    /** Set baud rate
         *
         */
    static void setBaudRate();
    /** Set transnmission mode
         *
         *  @param ar_transMode defines transmission mode
         */
    static void setTransmissionMode(const transmissionMode& ar_transMode);
    /** Set communication mode
         *
         *  @param ar_comMode defines communication mode
         */
    static void setCommunicationMode(const communicationMode& ar_comMode);
    /** Set parity mode in data frames
         *
         *  @param ar_parityMode defines parity mode
         */
    static void setParityMode(const parityMode& ar_parityMode);
    /** Set data frame size
         *
         *  @param ar_frameSize defines data frame size
         */
    static void setFrameSize(const frameSize& ar_frameSize);
    /** Set number of stop bits in data frames
         *
         *  @param ar_stopBit defines number of stop bits
         */
    static void setStopBit(const stopBit& ar_stopBit);
    /** Transmit data frames.
         *
         *  @param ap_dataBuffer defines pointer to transmitter buffer
         *  @param a_size defines size of transmitter buffer
         */
    static void sendFrame(const uint8_t *ap_dataBuffer, const uint8_t a_size);
    /** Receive data frames.
         *
         *  @param ap_dataBuffer defines pointer to receiver buffer
         *  @param a_size defines size of receiver buffer
         *  @param a_ready2Receive indicates if chip ready to receive data
         */
    static void receiveFrame(uint8_t *ap_dataBuffer, const uint8_t a_size, const uint8_t a_ready2Receive);
    /** Enable transmit complete interrupt.
         *
         *  @param ar_enable indicates if interrupt is enabled
         */
    static void enableTransmitCompleteInterrupt(const uint8_t ar_enable);
    /** Enable receive complete interrupt.
         *
         *  @param ar_enable indicates if interrupt is enabled
         */
    static void enableReceiveCompleteInterrupt(const uint8_t ar_enable);
    /** Enable data register empty interrupt.
         *
         *  @param ar_enable indicates if interrupt is enabled
         */
    static void enableDataRegisterEmptyInterrupt(const uint8_t ar_enable);
    /** Is there frame error in received data.
         */
    static uint8_t isFrameError();
    /** Is there data overrun in received data.
         */
    static uint8_t isDataOverrun();
    /** Is there partity error in received data.
         */
    static uint8_t isParityError();
    /** Receive complete ISR.
         */
    static void receiveCompleteServiceRoutine() __asm__(STR(USART0_RECEIVE_COMPLETE_INTERRUPT)) __attribute__((__signal__, __used__, __externally_visible__));
    /** Data register empty ISR.
         */
    static void dataRegisterEmptyServiceRoutine() __asm__(STR(USART0_DATA_REGISTER_EMPTY_INTERRUPT)) __attribute__((__signal__,__used__, __externally_visible__));
    /** Transmit complete ISR.
         */
    static void transmitCompleteServiceRoutine() __asm__(STR(USART0_TRANSMIT_COMPLETE_INTERRUPT)) __attribute__((__signal__, __used__, __externally_visible__));
    /** Get number of bytes received.
         */
    static uint8_t getNumberBytesReceived();
    /** Get number of bytes sent.
         */
    static uint8_t getNumberBytesSent();
    /** Reset number of bytes received.
         */
    static void resetNumberBytesReceived();

protected:




private:
    /** Constructor. Initalizes the USART0 object
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

    /** Overried Copy constructor.
        */
    USART0(const USART0&);

    /** Override assign operator.
        */
    const USART0& operator=(const USART0&);

    static volatile uint8_t m_status;   /**< received data status */

    static volatile const uint8_t *mp_data2Send; /**< pointer to transmitter buffer */

    static volatile uint8_t *mp_dataReceived; /**< pointer to receiver buffer */

    static uint8_t m_sizeData2Send;  /**< size of data to be transmitted */

    static uint8_t m_sizeData2Receive;  /**< size of data to be received */

    static uint8_t m_ready2Receive; /**< ready to receive flag */

    static uint8_t m_numberBytesReceived;   /**< number of bytes received */

    static uint8_t m_numberBytesSent;   /**< number of bytes sent */


};



}











#endif
