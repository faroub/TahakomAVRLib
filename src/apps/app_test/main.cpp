/**
 * @file main.cpp
 * @brief test mcu application
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/


#include "Buzzer.h"
#include "TimerCounter2.h"
#include "buzzer_pitches_8bit.h"


#define BUZZER 6


int main(void) {

    // Init
    // instantiate the Buzzer object
    component::Buzzer Buzzer(io::Pin(BUZZER,io::PortD));


    // instantiate the TimerCounter0 object
    core::TimerCounter2 &myTimerCounter2 = core::TimerCounter2::getInstance();

    // notes in the melody:
    const uint16_t notes[] = { C2, E2, G2, C3};




    for (uint8_t i = 0; i < sizeof (notes)/sizeof (uint16_t); i++)
    {

        Buzzer.buzz(notes[i],200);

    }

    _delay_ms(1000);
    _delay_ms(1000);
    _delay_ms(1000);

    for (uint8_t i = 0; i < sizeof (notes)/sizeof (uint16_t); i++)
    {
        Buzzer.buzz(myTimerCounter2,notes[i],200);
    }


    // ------ Event loop ------ //
    while (1) {
                                                  /* Get Key */

    }                                                  /* End event loop */
    return 0;
}
