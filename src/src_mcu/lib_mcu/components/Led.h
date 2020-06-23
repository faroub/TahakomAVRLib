/**
 * @file Led.h
 * @brief Header file of the Led class
 *
 * class to control a Led
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/
#ifndef LED_H
#define LED_H
#include <stdint.h>
#include "Pin.h"


namespace component
{
class Led
{

public:
    /** Constructor. Initalizes the pin object
         *
         *  @param ar_pin Reference to pin object
         */
    Led(const io::Pin &ar_pin);
    /** Destructor.
        */
    ~Led();
    /** Turn led On.
         */
    void on();
    /** Turn led Off.
         */
    void off();
    /** Toggle led state.
         */
    void toggle();
    /** Toggle led state.
         */
    uint8_t isOn();
    /** Toggle led state.
         */
    uint8_t isOff();

protected:

private:

    const io::Pin &mr_pin; /**< constant reference to a pin object */



};

}


#endif
