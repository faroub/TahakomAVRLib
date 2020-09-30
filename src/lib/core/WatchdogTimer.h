/**
 * @file TimerCounter.h
 * @brief Header file of the TimerCounter class
 *
 * Basic class for abstraction of the TimerCounter peripherals.
 *
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/

#ifndef TIMER_COUNTER2_H
#define TIMER_COUNTER2_H
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
