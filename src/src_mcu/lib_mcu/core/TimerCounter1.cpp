#include "TimerCounter1.h"


core::TimerCounter1& core::TimerCounter1::getInstance(const channel &ar_channel,
                                                      const operationMode &ar_operationMode,
                                                      const clockSource &ar_clockSource,
                                                      const compareOutputMode& ar_compareOutputMode)
{
    static TimerCounter1 l_instance(ar_channel,
                                    ar_operationMode,
                                    ar_clockSource,
                                    ar_compareOutputMode);

    return l_instance;


}

core::TimerCounter1::TimerCounter1(const channel &ar_channel,
                                   const operationMode &ar_operationMode,
                                   const clockSource &ar_clockSource,
                                   const compareOutputMode& ar_compareOutputMode)
{
    selectOperationMode(ar_operationMode);
    start(ar_clockSource);
    selectCompareOutputMode(ar_channel,ar_compareOutputMode);


}
core::TimerCounter1::~TimerCounter1()
{

}


void core::TimerCounter1::selectOperationMode(const operationMode &ar_operationMode)
{
    switch (ar_operationMode)
    {
        case core::operationMode::Normal:
        {
            TIMER1_SELECT_OPERATION_MODE(0);
            break;
        }
        case core::operationMode::PWM_PC_8bit:
        {
            TIMER1_SELECT_OPERATION_MODE(1);
            break;
        }
        case core::operationMode::PWM_PC_9bit:
        {
            TIMER1_SELECT_OPERATION_MODE(2);
            break;
        }
        case core::operationMode::PWM_PC_10bit:
        {
            TIMER1_SELECT_OPERATION_MODE(3);
            break;
        }
        case core::operationMode::CTC_OCR:
        {
            TIMER1_SELECT_OPERATION_MODE(4);
            break;
        }
        case core::operationMode::Fast_PWM_8bit:
        {
            TIMER1_SELECT_OPERATION_MODE(5);
            break;
        }
        case core::operationMode::Fast_PWM_9bit:
        {
            TIMER1_SELECT_OPERATION_MODE(6);
            break;
        }
        case core::operationMode::Fast_PWM_10bit:
        {
            TIMER1_SELECT_OPERATION_MODE(7);
            break;
        }
        case core::operationMode::PWM_PFC_ICR:
        {
            TIMER1_SELECT_OPERATION_MODE(8);
            break;
        }
        case core::operationMode::PWM_PFC_OCR:
        {
            TIMER1_SELECT_OPERATION_MODE(9);
            break;
        }
        case core::operationMode::PWM_PC_ICR:
        {
            TIMER1_SELECT_OPERATION_MODE(10);
            break;
        }
        case core::operationMode::PWM_PC_OCR:
        {
            TIMER1_SELECT_OPERATION_MODE(11);
            break;
        }
        case core::operationMode::CTC_ICR:
        {
            TIMER1_SELECT_OPERATION_MODE(12);
            break;
        }
        case core::operationMode::Fast_PWM_ICR:
        {
            TIMER1_SELECT_OPERATION_MODE(13);
            break;
        }
        case core::operationMode::Fast_PWM_OCR:
        {
            TIMER1_SELECT_OPERATION_MODE(14);
            break;
        }

    }


}


void core::TimerCounter1::start(const clockSource &ar_clockSource)
{
    switch (ar_clockSource)
    {
        case core::clockSource::NoClock:
        {
            TIMER1_SELECT_CLOCK_SOURCE(0);
            break;
        }
        case core::clockSource::PS_1:
        {
            TIMER1_SELECT_CLOCK_SOURCE(1);
            break;
        }
        case core::clockSource::PS_8:
        {
            TIMER1_SELECT_CLOCK_SOURCE(2);
            break;
        }
        case core::clockSource::PS_64:
        {
            TIMER1_SELECT_CLOCK_SOURCE(3);
            break;
        }
        case core::clockSource::PS_256:
        {
            TIMER1_SELECT_CLOCK_SOURCE(4);
            break;
        }
        case core::clockSource::PS_1024:
        {
            TIMER1_SELECT_CLOCK_SOURCE(5);
            break;
        }
        case core::clockSource::Extern_Clock_T0_Falling_Edge:
        {
            TIMER1_SELECT_CLOCK_SOURCE(6);
            break;
        }
        case core::clockSource::Extern_Clock_T0_Rising_Edge:
        {
            TIMER1_SELECT_CLOCK_SOURCE(7);
            break;
        }
    }

}

void core::TimerCounter1::stop()
{
    TIMER1_STOP;
}


void core::TimerCounter1::selectCompareOutputMode(const channel &ar_channel, const compareOutputMode &ar_compareOutputMode)
{
    switch (ar_channel)
    {
        case core::channel::A:
        {
            TIMER1_SELECT_COM_CHANNEL_A(static_cast<uint8_t>(ar_compareOutputMode));
            break;
        }
        case core::channel::B:
        {
            TIMER1_SELECT_COM_CHANNEL_B(static_cast<uint8_t>(ar_compareOutputMode));
            break;
        }
    }

}


void core::TimerCounter1::setCounter(const uint16_t &ar_dataBuffer)
{
    TCNT1 = ar_dataBuffer;
}

uint16_t core::TimerCounter1::getCounter() const
{
    return TCNT1;
}

void core::TimerCounter1::setOutputCompareRegister(const channel &ar_channel, const uint16_t &ar_dataBuffer)
{
    switch (ar_channel)
    {
        case core::channel::A:
        {
            OCR1A = ar_dataBuffer;
            break;
        }
        case core::channel::B:
        {
            OCR1B = ar_dataBuffer;
            break;
        }
    }
}



uint16_t core::TimerCounter1::getOutputCompareRegister(const channel &ar_channel) const
{
    switch (ar_channel)
    {
        case core::channel::A:
        {
            return OCR1A;
        }
        case core::channel::B:
        {
            return OCR1B;
        }
    }
}


