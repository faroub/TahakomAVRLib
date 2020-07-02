/**
 * @file Pin.h
 * @brief Header file of the Pin class
 *
 * Basic class for IO abstraction of the Atmega328p Pin and Port
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/
#ifndef PIN_H
#define PIN_H
#include "ha_base.h"

namespace io
{

/** Structure. Contains pointers to the Port's registers
     *
     */
struct Port
{
  volatile uint8_t* mp_ddrReg; /**< pointer to the data direction register */

  volatile uint8_t* mp_portReg; /**< pointer to the port register */

  volatile uint8_t* mp_pinReg; /**< pointer to the pin register */
};

class Pin
{

public:
    /** Constructor. Initalizes the pin number and port name
         *
         *  @param ar_port IO port of the avr chip
         *  @param ar_pin IO pin of the avr chip
         */
    Pin(const uint8_t &ar_pinNumber, const Port &ar_portName);
    /** Destructor.
        */
    ~Pin();
    /** Configure pin to output.
         */
    void toOutput();
    /** Configure pin to input.
         */
    void toInput(const uint8_t &ar_useInternalPullUp);
    /** Set pin to logic low.
         */
    void setLow();
    /** Set pin to logic high.
         */
    void setHigh();
    /** Toggle pin state.
         */
    void toggle();
    /** Check if pin is logic high.
         */
    uint8_t isHigh();
    /** Check if pin is logic low.
         */
    uint8_t isLow();
    /** Get pin number.
         */
    uint8_t getPinNumber();

protected:


private:

     const Port &mr_portName; /**< constant reference to a port object */
     const uint8_t &mr_pinNumber; /**< constant reference to pin number */



};

static io::Port PortB = { &DDRB, &PORTB, &PINB }; /**< global static Port B object */
static io::Port PortC = { &DDRC, &PORTC, &PINC }; /**< global static Port C object */
static io::Port PortD = { &DDRD, &PORTD, &PIND }; /**< global static Port D object */

}


#endif
