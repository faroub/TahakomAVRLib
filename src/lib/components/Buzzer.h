/**
 * @file Buzzer.h
 * @brief Header file of the Buzzer class
 *
 * Usage example (melody):
 *
    #include "Buzzer.h"
    #include "BuzzerPitches.h"


    #define BUZZER 1

    int main(void) {

        // Init
        component::Buzzer Buzzer(io::Pin(BUZZER,io::PortB));

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
            Buzzer.buzz(melody[thisNote], noteDuration);

            // to distinguish the notes, set a minimum time between them.
            // the note's duration + 30% seems to work well:
            pauseBetweenNotes = noteDuration * 1.3;
            Buzzer.pause(pauseBetweenNotes);
            // stop the tone playing:
            Buzzer.noBuzz();
          }




        // ------ Event loop ------ //
        while (1) {



        }
        return 0;
    }
 * Basic class for IO abstraction of Pin and Port
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/
#ifndef BUZZER_H
#define BUZZER_H
#include "ha_base.h"
#include "Pin.h"
#include "TimerCounter.h"

namespace component
{



class Buzzer
{

public:

    Buzzer(const io::Pin &ar_pin);

    ~Buzzer();

    void buzz(const uint16_t &ar_period_us , const uint16_t &ar_duration_ms);

    template<typename TC>
    void buzz(TC &ar_timerCounter,
              uint16_t ar_period_us,
              uint16_t a_duration_ms,
              const core::channel &ar_channel=core::channel::A,
              const core::clockSource &ar_clockSource= core::clockSource::PS_64
              )
    {
        ar_timerCounter.selectOperationMode(core::operationMode::CTC_OCR);
        ar_timerCounter.selectCompareOutputMode(ar_channel, core::compareOutputMode::Toggle);
        ar_timerCounter.setCounter(0);
        ar_timerCounter.setOutputCompareRegister(ar_channel, ar_period_us);
        // start timer
        ar_timerCounter.start(ar_clockSource);
        // wait for the pitch duration
        while (a_duration_ms) {
          _delay_ms(1);
          a_duration_ms--;
        }
        ar_timerCounter.selectCompareOutputMode(ar_channel, core::compareOutputMode::Normal);
        ar_timerCounter.stop();
    }


protected:

private:

    io::Pin m_pin; /**< pin object */


};
}


#endif
