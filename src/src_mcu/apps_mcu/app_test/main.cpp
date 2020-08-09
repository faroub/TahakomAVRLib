/**
 * @file main.cpp
 * @brief test mcu application
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/


#include "Buzzer.h"
#include "TimerCounter0.h"
#include "BuzzerPitches.h"


#define BUZZER 6

int main(void) {

    // Init
    // Buzzer object
    component::Buzzer Buzzer(io::Pin(BUZZER,io::PortD));
    // instantiate the TimerCounter0 object
    core::TimerCounter0 &myTimerCounter0 = core::TimerCounter0::getInstance();
    // notes in the melody:
    uint16_t melody[] = {
      NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, NOTE_G3, NOTE_B3, NOTE_C4
    };

    // note durations: 4 = quarter note, 8 = eighth note, etc.:
    uint8_t noteDurations[] = {
      4, 8, 8, 4, 4, 4, 4, 4
    };
    uint16_t noteDuration;
    uint16_t pauseBetweenNotes;
    for (uint8_t thisNote = 0; thisNote < 8; thisNote++) {

        // to calculate the note duration, take one second divided by the note type.
        //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        noteDuration = 1000 / noteDurations[thisNote];
        Buzzer.buzz(myTimerCounter0,melody[thisNote], noteDuration);

        // to distinguish the notes, set a minimum time between them.
        // the note's duration + 30% seems to work well:
        pauseBetweenNotes = noteDuration * 1.3;
        Buzzer.pause(pauseBetweenNotes);
        // stop the tone playing:
        Buzzer.noBuzz(myTimerCounter0);
    }


    // ------ Event loop ------ //
    while (1) {

                                                              /* Get Key */

    }                                                  /* End event loop */
    return 0;
}


