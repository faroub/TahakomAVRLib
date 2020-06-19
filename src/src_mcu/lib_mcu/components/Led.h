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
namespace io
{
class Pin;
}

namespace components
{
class Led
{

public:
    /** Constructor. Initalizes the pin object
         *
         *  @param ar_pin pin object
         */
    Led(io::Pin& ar_pin);
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
    /** Constant reference to a pin object
         */
    io::Pin &mr_pin;



};

}


#endif
