#ifndef LED_H
#define LED_H
#include "Pin.h"

/**
 * @file Led.h
 * @brief Header file of the Led class
 *
 * class to control a Led.
 *
 * Usage example (simple on/off example):
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/

namespace component
{

/**
 * @file Led.h
 * @brief Header file of the Led class
 *
 * class to control a Led.
 *
 * Usage example (simple on/off example):
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/

class Led
{

public:
    /**
     *  Constructor.
     *  Initalizes the led object
     *
     *  @param ar_pin constant reference to pin object
     */
    Led(const io::Pin &ar_pin);
    /**
     *  Destructor.
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
    /** Is led On.
         */
    uint8_t isOn();
    /** Is led Off.
         */
    uint8_t isOff();

protected:

private:

    io::Pin m_pin; /**< pin object */



};

}


#endif
