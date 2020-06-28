/**
 * @file USART0.h
 * @brief Header file of the USART0 class
 *
 * Basic class for IO abstraction of the Universal
 * Synchronous and Asynchronous serial Receiver and Transmitter
 * serial communication device
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/
#ifndef USART_H
#define USART_H
#include "ha_base.h"


namespace io
{

enum transmissionMode : uint8_t {
    Async=0,    /**< asynchronous mode */
    Sync,       /**< synchronous mode */
    MasterSPI   /**< masterSPI mode */
};

enum communicationMode : uint8_t {
    duplex=0,   /**< full duplex mode */
    transmit,   /**< transmit mode */
    receive,    /**< receive mode */

};

enum parityMode : uint8_t {
    noParity=0, /**< no parity check mode */
    evenParity, /**< even parity check mode */
    oddParity /**< odd parity check mode */
};

enum frameSize : uint8_t {
    eightBits=0, /**< 8 bits frame size */
    fiveBits,   /**< 5 bits frame size */
    sixBits, /**< 6 bits frame size */
    sevenBits, /**< 7 bits frame size */
    neineBits /**< 9 bits frame size */
};

enum stopBit : uint8_t{
    oneStopBit=0, /**< 1 stop bit */
    twoStopBits /**< 2 stop bits */
};

class USART0
{

public:

    static USART0& getInstance(const transmissionMode& ar_transMode = transmissionMode::Async,
                               const communicationMode& ar_comMode = communicationMode::duplex,
                               const frameSize& ar_frameSize = frameSize::eightBits,
                               const stopBit& ar_stopBit = stopBit::oneStopBit,
                               const parityMode& ar_parityMode = parityMode::noParity)
    {
        static USART0 l_instance(ar_transMode,
                                 ar_comMode,
                                 ar_frameSize,
                                 ar_stopBit,
                                 ar_parityMode);

        return l_instance;
    }



    static void setBaudRate();

    static void setTransmissionMode(const transmissionMode& ar_transMode);

    static void setCommunicationMode(const communicationMode& ar_comMode);

    static void setParityMode(const parityMode& ar_parityMode);

    static void setFrameSize(const frameSize& ar_frameSize);

    static void setStopBit(const stopBit& ar_stopBit);

    static void sendFrame(uint8_t *ap_dataBuffer, const int16_t& a_size);

    static void receiveFrame(uint8_t* ap_dataBuffer, const int16_t& a_size);

    static void enableTransmitterInterrupt(const bool &ar_enable);

    static void enableReceiverInterrupt(const bool &ar_enable);

    static void enableDataRegisterEmptyInterrupt(const bool &ar_enable);

    static bool isFrameError();

    static bool isDataOverrun();

    static bool isParityError();

    static void receiveCompleteServiceRoutine() __asm__(STR(USART0_RECEIVE_COMPLETE_INTERRUPT)) __attribute__((__signal__, __used__, __externally_visible__));

    static void dataRegisterEmptyServiceRoutine() __asm__(STR(USART0_DATA_REGISTER_EMPTY_INTERRUPT)) __attribute__((__signal__,__used__, __externally_visible__));

    static void transmitCompleteServiceRoutine() __asm__(STR(USART0_TRANSMIT_COMPLETE_INTERRUPT)) __attribute__((__signal__, __used__, __externally_visible__));



protected:




private:
    /** Constructor. Initalizes the USART serial communication device
         *
         *  @param ar_port IO port of the avr chip
         *  @param ar_pin IO pin of the avr chip
         */
    USART0(const transmissionMode& ar_transMode = transmissionMode::Async,
           const communicationMode& ar_comMode = communicationMode::duplex,
           const frameSize& ar_frameSize = frameSize::eightBits,
           const stopBit& ar_stopBit = stopBit::oneStopBit,
           const parityMode& ar_parityMode = parityMode::noParity);

    /** Destructor.
        */
    ~USART0();

    USART0(const USART0&);


    const USART0& operator=(const USART0&);


     /**< constant reference to operation mode */

     /**< constant reference to communication mode */

     /**< constant reference to frame sync bits */

     /**< constant reference to frame size */

     /**< constant reference to frame sync bits */

    static uint8_t m_status;

    static uint8_t m_data2Send;

    static uint8_t m_dataReceived;

    static int16_t m_numberBytesSent;

    static int16_t m_numberBytesReceived;


};



}











#endif
