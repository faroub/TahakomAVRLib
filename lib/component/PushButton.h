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
 * The class implements PushButton operations
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
     *  The configuration is copied into the object: storing references
     *  to the constructor arguments would leave them dangling after the
     *  call returns.
     *
     *  @param ar_pin Defines a Pin object
     *  @param ar_useInternalPullUp Defines if internal pull up resistor is used
     *  @param ar_isActiveLow Defines if input is active low or active high
     */
    PushButton(const io::Pin &ar_pin, uint8_t ar_useInternalPullUp=1, uint8_t ar_isActiveLow=1);
    /**
     *  Destructor
    */
    ~PushButton();
    /**
     *  Is pushbutton pressed
     *
     *  The level is selected by the active-low flag alone: active low
     *  reads the pin low as pressed, active high reads it high as
     *  pressed.  The pin is sampled, and after the debounce time
     *  (PUSHBUTTON_DEBOUNCE_TIME_US, 1 ms) sampled again; the button
     *  counts as pressed only if both samples agree.  The pressed count
     *  is incremented once per press, on the transition from released to
     *  pressed, so holding the button does not count as new presses.
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
    uint8_t m_isActiveLow; /**< Input active state */
    uint8_t m_useInternalPullUp; /**< Internal pullup resistor use  */
    uint8_t m_lastState; /**< Pressed state of the previous isPressed() call */
    uint8_t m_buttonPressed;    /**< PushButton pressed count */



};
}




#endif
