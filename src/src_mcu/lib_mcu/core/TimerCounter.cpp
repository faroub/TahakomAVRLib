#include "TimerCounter.h"


core::TimerCounter& core::TimerCounter::getInstanceTimerCounter0()
{
    static TimerCounter l_instance(timerCounter::timerCounter0);

    return l_instance;


}


core::TimerCounter& core::TimerCounter::getInstanceTimerCounter1()
{
    static TimerCounter l_instance(timerCounter::timerCounter1);

    return l_instance;

}

core::TimerCounter& core::TimerCounter::getInstanceTimerCounter2()
{
    static TimerCounter l_instance(timerCounter::timerCounter2);

    return l_instance;

}

core::TimerCounter::TimerCounter(const timerCounter &ar_timerCounter)
{

}
core::TimerCounter::~TimerCounter()
{

}







void core::TimerCounter::selectOperationMode(const timerCounter &ar_timerCounter, const operationMode &ar_operationMode)
{
    switch (ar_timerCounter)
    {
        case timerCounter::timerCounter0:
        {
            if (ar_operationMode <= operationMode::Mode_7)
            {
                TIMER0_SELECT_OPERATION_MODE(static_cast<uint8_t>(ar_operationMode));
            }
            break;

        }
        case timerCounter::timerCounter1:
        {
            TIMER1_SELECT_OPERATION_MODE(static_cast<uint8_t>(ar_operationMode));
            break;

        }
        case timerCounter::timerCounter2:
        {
            if (ar_operationMode <= operationMode::Mode_7)
            {
                TIMER2_SELECT_OPERATION_MODE(static_cast<uint8_t>(ar_operationMode));
            }
            break;

        }

    }

}





void core::TimerCounter::selectClockSource(const timerCounter &ar_timerCounter,const clockSource &ar_clockSource)
{
    switch (ar_timerCounter)
    {
        case timerCounter::timerCounter0:
        {
            TIMER0_SELECT_CLOCK_SOURCE(static_cast<uint8_t>(ar_clockSource));
            break;
        }
        case timerCounter::timerCounter1:
        {
            TIMER1_SELECT_CLOCK_SOURCE(static_cast<uint8_t>(ar_clockSource));
            break;
        }
        case timerCounter::timerCounter2:
        {
            TIMER2_SELECT_CLOCK_SOURCE(static_cast<uint8_t>(ar_clockSource));
            break;
        }
    }
}

void core::TimerCounter::stopTimer(const timerCounter &ar_timerCounter)
{
    switch (ar_timerCounter)
    {
        case timerCounter::timerCounter0:
        {
            TIMER0_STOP;
            break;
        }
        case timerCounter::timerCounter1:
        {
            TIMER1_STOP;
            break;
        }
        case timerCounter::timerCounter2:
        {
            TIMER2_STOP;
            break;
        }
    }

}


void core::TimerCounter::selectCOMChannelA(const timerCounter &ar_timerCounter, const compareOutputMode &ar_compareOutputMode, io::Pin a_pin)
{
    a_pin.toOutput();

    switch (ar_timerCounter)
    {
        case timerCounter::timerCounter0:
        {
            TIMER0_SELECT_COM_CHANNEL_A(static_cast<uint8_t>(ar_compareOutputMode));
            break;
        }
        case timerCounter::timerCounter1:
        {
            TIMER1_SELECT_COM_CHANNEL_A(static_cast<uint8_t>(ar_compareOutputMode));
            break;
        }
        case timerCounter::timerCounter2:
        {
            TIMER2_SELECT_COM_CHANNEL_A(static_cast<uint8_t>(ar_compareOutputMode));
            break;
        }
    }
}

void core::TimerCounter::selectCOMChannelB(const timerCounter &ar_timerCounter, const compareOutputMode &ar_compareOutputMode, io::Pin a_pin)
{
    a_pin.toOutput();

    switch (ar_timerCounter)
    {
        case timerCounter::timerCounter0:
        {
            TIMER0_SELECT_COM_CHANNEL_B(static_cast<uint8_t>(ar_compareOutputMode));
            break;
        }
        case timerCounter::timerCounter1:
        {
            TIMER1_SELECT_COM_CHANNEL_B(static_cast<uint8_t>(ar_compareOutputMode));
            break;
        }
        case timerCounter::timerCounter2:
        {
            TIMER2_SELECT_COM_CHANNEL_B(static_cast<uint8_t>(ar_compareOutputMode));
            break;
        }
    }

}

void core::TimerCounter::resetCounter(const timerCounter &ar_timerCounter)
{
    switch (ar_timerCounter)
    {
        case timerCounter::timerCounter0:
        {
            TIMER0_RESET_COUNTER;
            break;
        }
        case timerCounter::timerCounter1:
        {
            TIMER1_RESET_COUNTER;
            break;
        }
        case timerCounter::timerCounter2:
        {
            TIMER2_RESET_COUNTER;
            break;
        }
    }

}

void core::TimerCounter::getCounter(const timerCounter &ar_timerCounter, uint16_t *ap_dataBuffer)
{
    switch (ar_timerCounter)
    {
        case timerCounter::timerCounter0:
        {
            *ap_dataBuffer = TCNT0;
            break;
        }
        case timerCounter::timerCounter1:
        {
            *ap_dataBuffer = TCNT1;
            break;
        }
        case timerCounter::timerCounter2:
        {
            *ap_dataBuffer = TCNT2;
            break;
        }
    }

}
