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
    /** Constructor. Initalizes the pushbutton object
         *
         *  @param ar_pin Reference to pin object
         */
    PushButton(const io::Pin &ar_pin, const uint8_t &ar_useInternalPullUp=1, const uint8_t &ar_isActiveLow=1);
    /** Destructor.
        */
    ~PushButton();
    /** Is pushbutton pressed.
         */
    uint8_t isPressed();
    uint8_t getPressedCount() const;
    void resetPressedCount();


protected:

private:
    io::Pin mr_pin; /**< constant reference to a pin object */
    const uint8_t &mr_isActiveLow; /**< constant reference to active state */
    const uint8_t &mr_useInternalPullUp; /**< constant reference to usage of internal pullup */
    uint8_t m_buttonPressed;



};
}




#endif
