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
#include <stdint.h>
#include <avr/io.h>
#include <util/setbaud.h>


namespace io
{

enum opMode : uint8_t {
    Async=0,
    AsyncDouble,
    sync
};

enum comMode : uint8_t {
    send=0,
    receive,
    duplex
};

enum frameSize : uint8_t{
    fiveBits=0,
    sixBits,
    sevenBits,
    eightBits,
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


    USART0(const opMode& ar_opMode = opMode::Async,
           const comMode& ar_comMode = comMode::duplex,
           const frameSize& ar_frameSize = frameSize::eightBits,
           const frameSync& ar_frameSync = frameSync::oneStopBit);
    /** Destructor.
        */
    ~USART0();

    void sendFrame();

    void receiveFrame();




protected:




private:

    volatile uint8_t* mp_baudRateRegH = &UBRR0H; /**< pointer to the USART baud rate register high byte */

    volatile uint8_t* mp_baudRateRegL = &UBRR0L; /**< pointer to the USART baud rate register low byte */

    volatile uint8_t* mp_controlStatusRegA = &UCSR0A; /**< pointer to the USART control and status register A */

    volatile uint8_t* mp_controlStatusRegB = &UCSR0B; /**< pointer to the USART control and status register B */

    volatile uint8_t* mp_controlStatusRegC = &UCSR0C; /**< pointer to the USART control and status register C */

    volatile uint8_t* mp_dataReg = &UDR0; /**< pointer to the USART data register */

    const opMode &mr_opMode; /**< constant reference to operation mode */

    const comMode &mr_comMode; /**< constant reference to communication mode */

    const frameSize &mr_frameSize; /**< constant reference to frame size */

    const frameSync &mr_frameSync; /**< constant reference to frame sync bits */



};


}











#endif
