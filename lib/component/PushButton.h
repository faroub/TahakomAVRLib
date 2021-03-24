#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H
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
 * @brief Class for handling a PushButton component
 *
 * The class implements PushButton component operations
 *
 * @author Farid Oubbati (farid.oubbati@outlook.com)
 * @date March 2018
*/

class PushButton
{    
public:
    /** Constructor.
     *
     *  Initializes The PushButton object
     *
     *  @param ar_pin Defines a Pin object
     *  @param ar_useInternalPullUp Defines if internal pull up resistor is used
     *  @param ar_isActiveLow Defines if input is active low or active high
     */
    PushButton(const io::Pin &ar_pin, const uint8_t &ar_useInternalPullUp=1, const uint8_t &ar_isActiveLow=1);
    /**
     *  Destructor
    */
    ~PushButton();
    /**
     *  Is pushbutton pressed
     *
     *  @return  PushButton status
     */
    uint8_t isPressed();
    /**
     *  Get pushbutton pressed count
     *
     *  @return  PushButton press count
     */
    uint8_t getPressedCount() const;
    /**
     *  Reset pushbutton pressed count
     *
     */
    void resetPressedCount();


protected:

private:
    io::Pin m_pin; /**< Pin object */
    const uint8_t &mr_isActiveLow; /**< Input active state */
    const uint8_t &mr_useInternalPullUp; /**< Internal pullup resistor use  */
    uint8_t m_buttonPressed;    /**< PushButton pressed count */



};
}




#endif
