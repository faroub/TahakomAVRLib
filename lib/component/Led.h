#ifndef LED_H
#define LED_H
#include "Pin.h"

/**  
 *
 * @brief AVR chip external components
 *
 * The namespace englobes all external components that, when hooked up,
 * allows the AVR MCU to interact with the external World
 *
*/

namespace component
{

/**
 * @brief Class for handling a Led component
 *
 * The class implements Led component operations
 *
 * @author Farid Oubbati (farid.oubbati@outlook.com)
 * @date March 2018
*/

class Led
{

public:
    /**
     *  Constructor
     *
     *  Initalizes the Led object
     *
     *  @param ar_pin Defines a Pin object
     */
    Led(const io::Pin &ar_pin);
    /**
     *  Destructor
     */
    ~Led();
    /** Turn Led On
         */
    void on();
    /** Turn Led Off
         */
    void off();
    /** Toggle Led state
         */
    void toggle();
    /**
     * Is Led On
     *
     * @return  Led status
     */
    uint8_t isOn();
    /**
     *  Is Led Off
     *
     *  @return  Led status
     */
    uint8_t isOff();

protected:

private:

    io::Pin m_pin; /**< Pin object */



};

}


#endif
