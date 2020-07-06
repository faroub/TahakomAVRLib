/**
 * @file PushButton.h
 * @brief Header file of the Push Button class
 *
 * class to monitor a Push Button
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/
#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H
#include "Pin.h"

namespace component
{
class PushButton
{    
public:
    /** Constructor. Initializes pushbutton object
         *
         *  @param ar_pin pin object
         *  @param ar_useInternalPullUp indicates if internal pull up resistor used

         */
    PushButton(const io::Pin &ar_pin, const uint8_t &ar_useInternalPullUp=1, const uint8_t &ar_isActiveLow=1);
    /** Destructor.
        */
    ~PushButton();
    /** Is pushbutton pressed.
         */
    uint8_t isPressed();
    /** Get pushbutton pressed count.
         */
    uint8_t getPressedCount() const;
    /** Reset pushbutton pressed count.
         */
    void resetPressedCount();


protected:

private:
    io::Pin mr_pin; /**< pin object */
    const uint8_t &mr_isActiveLow; /**< indicates led active state */
    const uint8_t &mr_useInternalPullUp; /**< indicates if internal pullup resistor used  */
    uint8_t m_buttonPressed;    /**< pushbutton pressed count */



};
}




#endif
