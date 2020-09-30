#include "WatchdogTimer.h"


core::WatchdogTimer& core::WatchdogTimer::getInstance()
{
    static WatchdogTimer l_instance;
    return l_instance;

}

core::WatchdogTimer::WatchdogTimer()
{
    sei();
    stop();
}

core::WatchdogTimer::~WatchdogTimer()
{

}

void core::WatchdogTimer::selectTimeOut(const timeOut &ar_timeOut)
{
    m_timeOut = static_cast<uint8_t>(ar_timeOut);
    m_timeOut = static_cast<uint8_t>((m_timeOut & 7) | ((m_timeOut & 8) << 2 ));
    cli();
    wdt_reset();
    WATCHDOG_SELECT_TIMEOUT(m_timeOut);
    sei();

}

void core::WatchdogTimer::reset()
{
    wdt_reset();

}

void core::WatchdogTimer::start(const operationMode &ar_operationMode)
{
    m_operationMode = static_cast<uint8_t>(ar_operationMode);
    m_operationMode = static_cast<uint8_t>(((m_operationMode & 1) << 6) | ((m_operationMode & 2) << 3 ));
    cli();
    wdt_reset();
    WATCHDOG_START(m_operationMode,m_timeOut);
    sei();

}

void core::WatchdogTimer::start(const operationMode &ar_operationMode, const timeOut &ar_timeOut)
{
    m_timeOut = static_cast<uint8_t>(ar_timeOut);
    m_timeOut = static_cast<uint8_t>((m_timeOut & 7) | ((m_timeOut & 8) << 2 ));
    m_operationMode = static_cast<uint8_t>(ar_operationMode);
    m_operationMode = static_cast<uint8_t>(((m_operationMode & 1) << 6) | ((m_operationMode & 2) << 3 ));
    cli();
    wdt_reset();
    WATCHDOG_START(m_operationMode,m_timeOut);
    sei();

}

void core::WatchdogTimer::stop()
{
    cli();
    wdt_reset();
    WATCHDOG_STOP;
    sei();

}

