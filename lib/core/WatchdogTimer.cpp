#include "WatchdogTimer.h"
#include <util/atomic.h>


core::WatchdogTimer& core::WatchdogTimer::getInstance()
{
    static WatchdogTimer l_instance;
    return l_instance;

}

core::WatchdogTimer::WatchdogTimer()
{
    stop();
}

core::WatchdogTimer::~WatchdogTimer()
{

}

void core::WatchdogTimer::selectTimeOut(const timeOut &ar_timeOut)
{
    m_timeOut = static_cast<uint8_t>(ar_timeOut);
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        wdt_reset();
        if (m_isRunning)
        {
            // watchdog is running: follow the timed change sequence and keep the
            // programmed mode bits (WDIE/WDE) set, so the watchdog keeps running
            // with the new timeout
            WDTCSR |= (1 << WDCE) | (1 << WDE);
            WDTCSR = timeOutToWDTCSR(static_cast<timeOut>(m_timeOut)) |
                     operationModeToWDTCSR(static_cast<operationMode>(m_operationMode));
        }
        else
        {
            // watchdog not running: stage the timeout bits only
            WATCHDOG_SELECT_TIMEOUT(timeOutToWDTCSR(static_cast<timeOut>(m_timeOut)));
        }
    }

}

void core::WatchdogTimer::reset()
{
    wdt_reset();

}

void core::WatchdogTimer::start(const operationMode &ar_operationMode)
{
    m_operationMode = static_cast<uint8_t>(ar_operationMode);
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        wdt_reset();
        WATCHDOG_START(operationModeToWDTCSR(static_cast<operationMode>(m_operationMode)),
                       timeOutToWDTCSR(static_cast<timeOut>(m_timeOut)));
        m_isRunning = 1;
    }

}

void core::WatchdogTimer::start(const operationMode &ar_operationMode, const timeOut &ar_timeOut)
{
    m_timeOut = static_cast<uint8_t>(ar_timeOut);
    m_operationMode = static_cast<uint8_t>(ar_operationMode);
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        wdt_reset();
        WATCHDOG_START(operationModeToWDTCSR(static_cast<operationMode>(m_operationMode)),
                       timeOutToWDTCSR(static_cast<timeOut>(m_timeOut)));
        m_isRunning = 1;
    }

}

void core::WatchdogTimer::stop()
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        wdt_reset();
        WATCHDOG_STOP;
        m_isRunning = 0;
    }

}
