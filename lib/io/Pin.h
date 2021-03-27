#ifndef PIN_H
#define PIN_H
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
 *  @brief Contains defintions of pointers to the AVR port's registers
 *
*/
struct Port
{
  volatile uint8_t* mp_ddrReg; /**< Pointer to the data direction register */

  volatile uint8_t* mp_portReg; /**< Pointer to the port register */

  volatile uint8_t* mp_pinReg; /**< Pointer to the pin register */
};
/**
 * @brief Class for handling a Pin
 *
 * The class implements Pin operations
 *
 * @author Farid Oubbati (farid.oubbati@outlook.com)
 * @date March 2018
*/
class Pin
{

public:
    /** Constructor
     *
     *  Initalizes the pin object
     *
     *  @param ar_portName Defines the Port name in the AVR chip
     *  @param a_pinNumber Defines the Pin number in the AVR chip
     */
    Pin(const uint8_t a_pinNumber, const Port &ar_portName);
    /** Destructor
        */
    ~Pin();
    /** Configures Pin to output
         */
    void toOutput();
    /** Configures Pin to input
        *
        * @param ar_useInternalPullUp Indicates if internal pull up resistor is used
        */
    void toInput(const uint8_t &ar_useInternalPullUp);
    /** Set Pin to logic low
         */
    void setLow();
    /** Set Pin to logic high
         */
    void setHigh();

    /** Toggle Pin state
         */
    void toggle();
    /**
     *  Check if Pin is logic high
     *
     *  @return  Pin status
     */
    uint8_t isHigh();
    /**
     *  Check if Pin is logic low
     *
     *  @return  Pin status
     */
    uint8_t isLow();
    /**
     *  Get Pin number
     *
     *  @return  Pin number
     */
    uint8_t getPinNumber();

protected:


private:

     const Port &mr_portName; /**< Port object reference */
     const uint8_t m_pinNumber; /**< Pin number */




};

static io::Port PortB = { &DDRB, &PORTB, &PINB }; /**< global static Port B object */
static io::Port PortC = { &DDRC, &PORTC, &PINC }; /**< global static Port C object */
static io::Port PortD = { &DDRD, &PORTD, &PIND }; /**< global static Port D object */

}


#endif
