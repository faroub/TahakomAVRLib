#include "TimerCounter0.h"


core::TimerCounter0& core::TimerCounter0::getInstance(const channel &ar_channel,
                                                      const operationMode &ar_operationMode,
                                                      const clockSource &ar_clockSource,
                                                      const compareOutputMode &ar_compareOutputMode)
{
    static TimerCounter0 l_instance(ar_channel,
                                    ar_operationMode,
                                    ar_clockSource,
                                    ar_compareOutputMode);

    return l_instance;


}

core::TimerCounter0::TimerCounter0(const channel &ar_channel,
                                   const operationMode &ar_operationMode,
                                   const clockSource &ar_clockSource,
                                   const compareOutputMode& ar_compareOutputMode)
{
    selectOperationMode(ar_operationMode);
    start(ar_clockSource);
    selectCompareOutputMode(ar_channel,ar_compareOutputMode);



}
core::TimerCounter0::~TimerCounter0()
{

}


void core::TimerCounter0::selectOperationMode(const operationMode &ar_operationMode)
{
    switch (ar_operationMode)
    {
        case core::operationMode::Normal:
        {
            TIMER0_SELECT_OPERATION_MODE(0);
            break;
        }
        case core::operationMode::PWM_PC:
        {
            TIMER0_SELECT_OPERATION_MODE(1);
            break;
        }
        case core::operationMode::CTC_OCR:
        {
            TIMER0_SELECT_OPERATION_MODE(2);
            break;
        }
        case core::operationMode::Fast_PWM:
        {
            TIMER0_SELECT_OPERATION_MODE(3);
            break;
        }
        case core::operationMode::PWM_PC_OCR:
        {
            TIMER0_SELECT_OPERATION_MODE(5);
            break;
        }
        case core::operationMode::Fast_PWM_OCR:
        {
            TIMER0_SELECT_OPERATION_MODE(7);
            break;
        }

    }

}


void core::TimerCounter0::start(const clockSource &ar_clockSource)
{
    switch (ar_clockSource)
    {
        case core::clockSource::NoClock:
        {
            TIMER0_SELECT_CLOCK_SOURCE(0);
            break;
        }
        case core::clockSource::PS_1:
        {
            TIMER0_SELECT_CLOCK_SOURCE(1);
            break;
        }
        case core::clockSource::PS_8:
        {
            TIMER0_SELECT_CLOCK_SOURCE(2);
            break;
        }
        case core::clockSource::PS_64:
        {
            TIMER0_SELECT_CLOCK_SOURCE(3);
            break;
        }
        case core::clockSource::PS_256:
        {
            TIMER0_SELECT_CLOCK_SOURCE(4);
            break;
        }
        case core::clockSource::PS_1024:
        {
            TIMER0_SELECT_CLOCK_SOURCE(5);
            break;
        }
        case core::clockSource::Extern_Clock_T0_Falling_Edge:
        {
            TIMER0_SELECT_CLOCK_SOURCE(6);
            break;
        }
        case core::clockSource::Extern_Clock_T0_Rising_Edge:
        {
            TIMER0_SELECT_CLOCK_SOURCE(7);
            break;
        }
    }

}

void core::TimerCounter0::stop()
{
    TIMER0_STOP;
}


void core::TimerCounter0::selectCompareOutputMode(const channel &ar_channel, const compareOutputMode &ar_compareOutputMode)
{
    switch (ar_channel)
    {
        case core::channel::A:
        {
            TIMER0_SELECT_COM_CHANNEL_A(static_cast<uint8_t>(ar_compareOutputMode));
            break;
        }
        case core::channel::B:
        {
            TIMER0_SELECT_COM_CHANNEL_B(static_cast<uint8_t>(ar_compareOutputMode));
            break;
        }
    }
}



void core::TimerCounter0::setCounter(const uint16_t &ar_dataBuffer)
{
    TCNT0 = static_cast<uint8_t>(ar_dataBuffer);
}

uint16_t core::TimerCounter0::getCounter() const
{
    return TCNT0;
}

void core::TimerCounter0::setOutputCompareRegister(const channel &ar_channel, uint16_t ar_dataBuffer)
{
    switch (ar_channel)
    {
        case core::channel::A:
        {
            OCR0A = static_cast<uint8_t>(ar_dataBuffer);
            break;
        }
        case core::channel::B:
        {
            OCR0B = static_cast<uint8_t>(ar_dataBuffer);
            break;
        }

    }
}



uint16_t core::TimerCounter0::getOutputCompareRegister(const channel &ar_channel) const
{
    switch (ar_channel)
    {
        case core::channel::A:
        {
            return OCR0A;
        }
        case core::channel::B:
        {
            return OCR0B;
        }
    }
}

void core::TimerCounter0::enableOutputCompareMatchInterrupt(const channel &ar_channel, const uint8_t a_enable)
{
    switch (ar_channel)
    {
        case core::channel::A:
        {
            if (a_enable) {

                TIMER0_ENABLE_COM_CHANNEL_A_INTERRUPT;

            } else {

                TIMER0_DISABLE_COM_CHANNEL_A_INTERRUPT;
            }
            break;
        }
        case core::channel::B:
        {
            if (a_enable) {

                TIMER0_ENABLE_COM_CHANNEL_B_INTERRUPT;

            } else {

                TIMER0_DISABLE_COM_CHANNEL_B_INTERRUPT;

            }
            break;
        }
    }

}



void core::TimerCounter0::enableOverflowInterrupt(const uint8_t a_enable)
{

    if (a_enable) {

        TIMER0_ENABLE_OVERFLOW_INTERRUPT;

    } else {

        TIMER0_DISABLE_OVERFLOW_INTERRUPT;
    }

}
