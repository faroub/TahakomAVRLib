/**
 * @file WatchdogTimer.h
 * @brief Header file of the WatchdogTimer class
 *
 * Basic class abstraction of the WatchdogTimer peripheral
 *
 * Usage example (test):
 *
 #include "WatchdogTimer.h"
 #include "Led.h"
 // instantiate a Led object
 extern component::Led Led;
 component::Led Led(io::Pin(1,io::PortB));
 extern component::Led LedStart;
 component::Led LedStart(io::Pin(2,io::PortB));


 int main(void) {

   // Init
   // instantiate a Watchdog object
   core::WatchdogTimer &myWatchdog = core::WatchdogTimer::getInstance();
   myWatchdog.selectTimeOut(core::timeOut::to_8s);
   LedStart.on();
   _delay_ms(5000);
   LedStart.off();
   myWatchdog.start(core::operationMode::reset);
   // ------ Event loop ------ //
   while (1) {

       Led.on();
       _delay_ms(1000);
       Led.off();
       _delay_ms(1000);

   }
   return 0;
 }

 void core::WatchdogTimer::timeOutServiceRoutine()
 {
    for (uint8_t i=0;i<10;i++)
    {
        Led.on();
        _delay_ms(100);
        Led.off();
        _delay_ms(100);

    }

 }
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/

#ifndef WATCHDOG_TIMER_H
#define WATCHDOG_TIMER_H
#include "ha_base.h"



namespace core
{
enum class timeOut : uint8_t {
    to_16ms=0,    /**<  */
    to_32ms,    /**<  */
    to_64ms,    /**<  */
    to_125ms,    /**<  */
    to_250ms,    /**<  */
    to_500ms,    /**<  */
    to_1s,    /**<  */
    to_2s,    /**<  */
    to_4s,    /**<  */
    to_8s,    /**<  */
};

enum class operationMode : uint8_t {
    interrupt=1,    /**<  */
    reset,       /**<  */
    interrupt_reset,   /**<  */
};

class WatchdogTimer
{

public:

    static WatchdogTimer& getInstance();

    void selectTimeOut(const timeOut &ar_timeOut);

    void reset();

    void start(const operationMode &ar_operationMode);

    void start(const operationMode &ar_operationMode, const timeOut &ar_timeOut);


    void stop();

    static void timeOutServiceRoutine() __asm__(STR(WATCHDOG_TIMEOUT_INTERRUPT)) __attribute__((__signal__, __used__, __externally_visible__));


protected:

private:

    WatchdogTimer();

    /** Destructor.
        */
    ~WatchdogTimer();

    /** Overried Copy constructor.
        */
    WatchdogTimer(const WatchdogTimer&);

    /** Override assign operator.
        */
    const WatchdogTimer& operator=(const WatchdogTimer&);

    uint8_t m_timeOut;

    uint8_t m_operationMode;

};

}


#endif
