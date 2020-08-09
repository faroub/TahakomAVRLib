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

    void buzz(const uint16_t &ar_frequency_hz , const uint16_t &ar_duration_ms);

    template<typename TC>
    void buzz(TC &ar_timerCounter,
              const uint16_t &ar_frequency_hz,
              const uint16_t &ar_duration_ms,
              const core::channel &ar_channel=core::channel::A)
    {
        uint16_t l_period_ms = 0xFFFF;

        if (ar_frequency_hz)
        {
            l_period_ms = 1000/ar_frequency_hz;
        }
        ar_timerCounter.selectOperationMode(core::operationMode::CTC_OCR);

        switch (ar_channel)
        {
            case core::channel::A:
            {
                ar_timerCounter.selectCOMChannelA(core::compareOutputMode::Toggle);
                ar_timerCounter.setOCRChannelA(&l_period_ms);
                break;
            }
            case core::channel::B:
            {
                ar_timerCounter.selectCOMChannelB(core::compareOutputMode::Toggle);
                ar_timerCounter.setOCRChannelB(&l_period_ms);
                break;
            }

        }
        // start timer
        ar_timerCounter.selectClockSource(core::clockSource::PS_1024);


    }



    void pause(uint16_t &ar_duration_ms);

    void noBuzz();

    template<typename TC>
    void noBuzz(TC &ar_timerCounter)
    {
        ar_timerCounter.stopTimer();
    }


protected:

private:

    io::Pin m_pin; /**< pin object */


};
}


#endif
