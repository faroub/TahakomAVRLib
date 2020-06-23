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

enum transMode : uint8_t {
    Async=0,
    AsyncDouble,
    sync
};

enum comMode : uint8_t {
    duplex=0,
    send,
    receive,

};

enum parityMode : uint8_t {
    noParity=0,
    evenParity,
    oddParity
};

enum frameSize : uint8_t {
    eightBits=0,
    fiveBits,
    sixBits,
    sevenBits,
    neineBits
};

enum frameSync : uint8_t{
    oneStopBit=0,
    twoStopBits
};

class USART0
{

public:



    /** Constructor. Initalizes the USART serial communication device
         *
         *  @param ar_port IO port of the avr chip
         *  @param ar_pin IO pin of the avr chip
         */

    // initialize USART:
    //  - setting the baud rate
    //  - setting frame format (frame size and stop bits)
    //  - enabling the Transmitter or/and the Receiver depending on the usage


    USART0(const transMode& transMode = transMode::Async,
           const comMode& ar_comMode = comMode::duplex,
           const frameSize& ar_frameSize = frameSize::eightBits,
           const frameSync& ar_frameSync = frameSync::oneStopBit,
           const parityMode& ar_parityMode = parityMode::noParity);
    /** Destructor.
        */
    ~USART0();

    void sendFrame();

    void receiveFrame();




protected:




private:

    const transMode &mr_transMode; /**< constant reference to operation mode */

    const comMode &mr_comMode; /**< constant reference to communication mode */

    const parityMode &mr_parityMode; /**< constant reference to frame sync bits */

    const frameSize &mr_frameSize; /**< constant reference to frame size */

    const frameSync &mr_frameSync; /**< constant reference to frame sync bits */



};


}











#endif
