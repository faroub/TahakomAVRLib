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
   myWatchdog.start(core::watchdogOperationMode::reset);
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

enum class watchdogOperationMode : uint8_t {
    interrupt=1,    /**<  */
    reset,       /**<  */
    interrupt_reset,   /**<  */
};

/** Map a watchdog timeout setting to the WDTCSR timeout bits (WDP3:WDP0).
 *
 *  The enum value uses a plain binary encoding (0..9), the WDTCSR lays the
 *  prescaler bits out as WDP3:WDP2:WDP1:WDP0 = bit5:bit2:bit1:bit0.
 *
 *  @param ar_timeOut Timeout setting
 *  @return WDTCSR timeout bits
 */
constexpr uint8_t timeOutToWDTCSR(const timeOut ar_timeOut)
{
    return static_cast<uint8_t>((static_cast<uint8_t>(ar_timeOut) & 7u) |
                                ((static_cast<uint8_t>(ar_timeOut) & 8u) << 2u));
}

/** Map a watchdog operation mode to the WDTCSR mode bits (WDIE, WDE).
 *
 *  @param ar_operationMode Operation mode
 *  @return WDTCSR mode bits
 */
constexpr uint8_t operationModeToWDTCSR(const watchdogOperationMode ar_operationMode)
{
    // mode bit 0 (value 1) maps to WDIE (bit 6); mode bit 1 (value 2) maps to
    // WDE (bit 3), so it must be shifted left by (WDE - 1): shifting by WDE
    // would land on WDCE (bit 4) instead
    return static_cast<uint8_t>(((static_cast<uint8_t>(ar_operationMode) & 1u) << WDIE) |
                                ((static_cast<uint8_t>(ar_operationMode) & 2u) << (WDE - 1)));
}

static_assert((operationModeToWDTCSR(watchdogOperationMode::reset) & (1u << WDE)) != 0,
              "watchdog reset mode must set WDE");
static_assert((operationModeToWDTCSR(watchdogOperationMode::interrupt_reset) & (1u << WDE)) != 0,
              "watchdog interrupt+reset mode must set WDE");
static_assert((operationModeToWDTCSR(watchdogOperationMode::interrupt) & (1u << WDE)) == 0,
              "watchdog interrupt-only mode must not set WDE");

class WatchdogTimer
{

public:

    static WatchdogTimer& getInstance();

    /** Select the watchdog timeout.
     *
     *  Rule: if the watchdog is already running (start() was called), the
     *  timeout change keeps the watchdog running: it preserves the
     *  programmed mode bits (WDIE/WDE) and only applies the new timeout.
     *  If the watchdog is not running, the timeout is only staged in WDTCSR
     *  without enabling it; start() turns it on.
     *
     *  @param ar_timeOut Defines the timeout
     */
    void selectTimeOut(const timeOut &ar_timeOut);

    void reset();

    void start(const watchdogOperationMode &ar_operationMode);

    void start(const watchdogOperationMode &ar_operationMode, const timeOut &ar_timeOut);


    void stop();

    static void timeOutServiceRoutine() __asm__(STR(WATCHDOG_TIMEOUT_INTERRUPT)) __attribute__((__signal__, __used__, __externally_visible__, weak));


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

    uint8_t m_isRunning; /**< Watchdog running state, tracked in software */

};

}


#endif
