
// #include "Buzzer.h"
// #include "TimerCounter0.h"
// #include "buzzer_pitches_8bit.h"


// #define BUZZER 6


// int main(void) {

//    // Init
//    // instantiate the Buzzer object
//    component::Buzzer Buzzer(io::Pin(BUZZER,io::PortD));


//    // instantiate the TimerCounter0 object
//    core::TimerCounter0 &myTimerCounter0 = core::TimerCounter0::getInstance();

//    // notes in the melody:
//    const uint16_t notes[] = {C2, E2, G2, C3};


//    for (uint8_t i = 0; i < sizeof (notes)/sizeof (uint16_t); i++)
//    {

//        Buzzer.buzz(notes[i],200);

//    }

//    _delay_ms(1000);
//    _delay_ms(1000);
//    _delay_ms(1000);

//    for (uint8_t i = 0; i < sizeof (notes)/sizeof (uint16_t); i++)
//    {
//        Buzzer.buzz(myTimerCounter0,notes[i],200);
//    }


//    // ------ Event loop ------ //
//    while (1) {


//    }
//    return 0;
// }

#ifndef BUZZER_H
#define BUZZER_H
#include "ha_base.h"
#include "Pin.h"
#include "TimerCounter.h"

/**
 *
 * @brief AVR chip external components
 *
 * The namespace englobes all external components that, when hooked up,
 * allows the AVR chip to interact with the external World
 *
*/
namespace component
{

/**
 * @brief Class for handling a Buzzer component
 *
 * The class implements Buzzer operations
 *
 * @author Farid Oubbati (farid.oubbati@outlook.com)
 * @date March 2018
*/
class Buzzer
{

public:
    /**
     *  Constructor
     *
     *  Initalizes the Buzzer object
     *
     *  @param ar_pin Defines a Pin object
     */
    Buzzer(const io::Pin &ar_pin);
    /**
     *  Destructor
     */
    ~Buzzer();

    /** Generate a Buzzer signal
    *
    * The pin toggles once per period for the given duration, so the
    * fundamental frequency is half of one over the period: a period of
    * 2273 us gives 220 Hz, an octave below A4.  A period of 0 returns
    * immediately (the pitch loop steps by the period and would spin
    * forever).  A duration of 0 toggles nothing.
    *
    * @param ar_period_us Defines period of Buzzer signal
    * @param ar_duration_us Defines duration of Buzzer signal
    */
    void buzz(const uint16_t &ar_period_us , const uint16_t &ar_duration_us);

    /** Generate a time break
    *
    * Waits for the given duration in microseconds.  pause(0) returns
    * immediately (the decrement loop would otherwise wrap around and
    * delay for about 65536 us).
    *
    * @param a_duration_us Defines duration of time break
    */
    void pause(uint16_t a_duration_us);

    /**
     * Generate a Buzzer signal
     *
     * \tparam TC Type Defines a TimerCounter object
     * @param ar_timerCounter Defines the TimerCounter generating the pitch
     * @param ar_period_ticks Defines the period of the Buzzer signal in
     *        timer ticks: the value loaded into the output compare
     *        register, not microseconds.  The toggle frequency is
     *        F_CPU / (2 * prescaler * ticks); with the default prescaler
     *        PS_64 at F_CPU = 16 MHz, 568 ticks toggle at 220 Hz.
     *        Timer0 and Timer2 are 8-bit, so their tick value must stay
     *        below 256.  The duration is taken by value, so the caller's
     *        variable keeps its value across the call.
     * @param a_duration_ms Defines duration of the signal in ms
     * @param ar_channel Defines TimerCounter channel
     * @param ar_clockSource Defines TimerCounter clock source, documented
     *        for the API; the call keeps the timer's current clock source
     */
    template<typename TC>
    void buzz(TC &ar_timerCounter,
              const uint16_t &ar_period_ticks,
              uint16_t a_duration_ms,
              const core::channel &ar_channel=core::channel::A,
              const core::clockSource &ar_clockSource=core::clockSource::PS_64
              )
    {
        (void)ar_clockSource;   // start() keeps the timer's current source
        ar_timerCounter.selectOperationMode(core::operationMode::CTC_OCR);
        ar_timerCounter.selectCompareOutputMode(ar_channel, core::compareOutputMode::toggle);
        ar_timerCounter.setCounter(0);
        ar_timerCounter.setOutputCompareRegister(ar_channel, ar_period_ticks);
        // start timer
        ar_timerCounter.start();
        // wait for the pitch duration
        while (a_duration_ms) {
          _delay_ms(1);
          a_duration_ms--;
        }
        // no buzz
        ar_timerCounter.selectCompareOutputMode(ar_channel, core::compareOutputMode::normal);
        ar_timerCounter.stop();
    }


protected:

private:

    io::Pin m_pin; /**< Pin object */


};
}


#endif
