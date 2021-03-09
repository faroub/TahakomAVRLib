/**
 * @file Buzzer.h
 * @brief Header file of the Buzzer class
 *
 * Usage example (test):
 #include "Buzzer.h"
 #include "TimerCounter0.h"
 #include "buzzer_pitches_8bit.h"


 #define BUZZER 6


 int main(void) {

    // Init
    // instantiate the Buzzer object
    component::Buzzer Buzzer(io::Pin(BUZZER,io::PortD));


    // instantiate the TimerCounter0 object
    core::TimerCounter0 &myTimerCounter0 = core::TimerCounter0::getInstance();

    // notes in the melody:
    const uint16_t notes[] = {C2, E2, G2, C3};


    for (uint8_t i = 0; i < sizeof (notes)/sizeof (uint16_t); i++)
    {

        Buzzer.buzz(notes[i],200);

    }

    _delay_ms(1000);
    _delay_ms(1000);
    _delay_ms(1000);

    for (uint8_t i = 0; i < sizeof (notes)/sizeof (uint16_t); i++)
    {
        Buzzer.buzz(myTimerCounter0,notes[i],200);
    }


    // ------ Event loop ------ //
    while (1) {


    }
    return 0;
 }
 *
 *
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
              const uint16_t &ar_period_us,
              uint16_t &ar_duration_ms,
              const core::channel &ar_channel=core::channel::A,
              const core::clockSource &ar_clockSource= core::clockSource::PS_64
              )
    {
        ar_timerCounter.selectOperationMode(core::operationMode::CTC_OCR);
        ar_timerCounter.selectCompareOutputMode(ar_channel, core::compareOutputMode::toggle);
        ar_timerCounter.setCounter(0);
        ar_timerCounter.setOutputCompareRegister(ar_channel, ar_period_us);
        // start timer
        ar_timerCounter.start();
        // wait for the pitch duration
        while (ar_duration_ms) {
          _delay_ms(1);
          ar_duration_ms--;
        }
        // no buzz
        ar_timerCounter.selectCompareOutputMode(ar_channel, core::compareOutputMode::normal);
        ar_timerCounter.stop();
    }


protected:

private:

    io::Pin m_pin; /**< pin object */


};
}


#endif
