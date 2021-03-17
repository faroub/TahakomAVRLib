/**
 * @file Pin.h
 * @brief Header file of the Pin class
 *
 * Basic class for Pin and Port abstraction
 *
 * - Compiler:           gcc-avr
 * - Supported devices:  The example compiles on the ATmega48P/88P/168P/328P AVR family
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2018
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
    /** Constructor. Initalizes the pin object
         *
         *  @param ar_portName defines the port name of the avr chip
         *  @param ar_pinNumber defines the pin number of the avr chip
         */
    Pin(const uint8_t a_pinNumber, const Port &ar_portName);
    /** Destructor.
        */
    ~Pin();
    /** Configures pin to output.
         */
    void toOutput();
    /** Configures pin to input.
        *
        * @param ar_useInternalPullUp indicates if internal pull up resistor is used
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

     const Port &mr_portName; /**< constant reference to port object */
     const uint8_t m_pinNumber; /**< pin number */



};

static io::Port PortB = { &DDRB, &PORTB, &PINB }; /**< global static Port B object */
static io::Port PortC = { &DDRC, &PORTC, &PINC }; /**< global static Port C object */
static io::Port PortD = { &DDRD, &PORTD, &PIND }; /**< global static Port D object */

}


#endif
