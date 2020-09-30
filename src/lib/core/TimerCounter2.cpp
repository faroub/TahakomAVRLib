#include "TimerCounter2.h"


core::TimerCounter2& core::TimerCounter2::getInstance(const channel &ar_channel,
                                                      const operationMode &ar_operationMode,
                                                      const clockSource &ar_clockSource,
                                                      const compareOutputMode& ar_compareOutputMode)
{
    static TimerCounter2 l_instance(ar_channel,
                                    ar_operationMode,
                                    ar_clockSource,
                                    ar_compareOutputMode);

    return l_instance;


}

core::TimerCounter2::TimerCounter2(const channel &ar_channel,
                                   const operationMode &ar_operationMode,
                                   const clockSource &ar_clockSource,
                                   const compareOutputMode& ar_compareOutputMode)
{
    selectOperationMode(ar_operationMode);
    start(ar_clockSource);
    selectCompareOutputMode(ar_channel, ar_compareOutputMode);

}
core::TimerCounter2::~TimerCounter2()
{

}


void core::TimerCounter2::selectOperationMode(const operationMode &ar_operationMode)
{
    switch (ar_operationMode)
    {
        case core::operationMode::normal:
        {
            TIMER2_SELECT_OPERATION_MODE(0);
            break;
        }
        case core::operationMode::PWM_PC:
        {
            TIMER2_SELECT_OPERATION_MODE(1);
            break;
        }
        case core::operationMode::CTC_OCR:
        {
            TIMER2_SELECT_OPERATION_MODE(2);
            break;
        }
        case core::operationMode::fast_PWM:
        {
            TIMER2_SELECT_OPERATION_MODE(3);
            break;
        }
        case core::operationMode::PWM_PC_OCR:
        {
            TIMER2_SELECT_OPERATION_MODE(5);
            break;
        }
        case core::operationMode::fast_PWM_OCR:
        {
            TIMER2_SELECT_OPERATION_MODE(7);
            break;
        }

    }

}


void core::TimerCounter2::start(const clockSource &ar_clockSource)
{
    switch (ar_clockSource)
    {
        case core::clockSource::noClock:
        {
            TIMER2_SELECT_CLOCK_SOURCE(0);
            break;
        }
        case core::clockSource::PS_1:
        {
            TIMER2_SELECT_CLOCK_SOURCE(1);
            break;
        }
        case core::clockSource::PS_8:
        {
            TIMER2_SELECT_CLOCK_SOURCE(2);
            break;
        }
        case core::clockSource::PS_32:
        {
            TIMER2_SELECT_CLOCK_SOURCE(3);
            break;
        }
        case core::clockSource::PS_64:
        {
            TIMER2_SELECT_CLOCK_SOURCE(4);
            break;
        }
        case core::clockSource::PS_128:
        {
            TIMER2_SELECT_CLOCK_SOURCE(5);
            break;
        }
        case core::clockSource::PS_256:
        {
            TIMER2_SELECT_CLOCK_SOURCE(6);
            break;
        }
        case core::clockSource::PS_1024:
        {
            TIMER2_SELECT_CLOCK_SOURCE(7);
            break;
        }
    }


}

void core::TimerCounter2::stop()
{
    TIMER2_STOP;
}


void core::TimerCounter2::selectCompareOutputMode(const channel &ar_channel, const compareOutputMode &ar_compareOutputMode)
{
    switch (ar_channel)
    {
        case core::channel::A:
        {
            TIMER2_SELECT_COM_CHANNEL_A(static_cast<uint8_t>(ar_compareOutputMode));
            break;
        }
        case core::channel::B:
        {
            TIMER2_SELECT_COM_CHANNEL_B(static_cast<uint8_t>(ar_compareOutputMode));
            break;
        }
    }


}



void core::TimerCounter2::setCounter(const uint16_t &ar_dataBuffer)
{
    TCNT2 = static_cast<uint8_t>(ar_dataBuffer);
}

uint16_t core::TimerCounter2::getCounter() const
{
    return TCNT2;
}

void core::TimerCounter2::setOutputCompareRegister(const channel &ar_channel, const uint16_t &ar_dataBuffer)
{
    switch (ar_channel)
    {
        case core::channel::A:
        {
            OCR2A = static_cast<uint8_t>(ar_dataBuffer);
            break;
        }
        case core::channel::B:
        {
            OCR2B = static_cast<uint8_t>(ar_dataBuffer);
            break;
        }
    }
}


uint16_t core::TimerCounter2::getOutputCompareRegister(const channel &ar_channel) const
{
    switch (ar_channel)
    {
        case core::channel::A:
        {
            return OCR2A;
        }
        case core::channel::B:
        {
            return OCR2B;
        }
    }

}

void core::TimerCounter2::enableOutputCompareMatchInterrupt(const channel &ar_channel, const uint8_t a_enable)
{
    switch (ar_channel)
    {
        case core::channel::A:
        {
            if (a_enable) {

                TIMER2_ENABLE_COM_CHANNEL_A_INTERRUPT;

            } else {

                TIMER2_DISABLE_COM_CHANNEL_A_INTERRUPT;
            }
            break;
        }
        case core::channel::B:
        {
            if (a_enable) {

                TIMER2_ENABLE_COM_CHANNEL_B_INTERRUPT;

            } else {

                TIMER2_DISABLE_COM_CHANNEL_B_INTERRUPT;

            }
            break;
        }
    }

}



void core::TimerCounter2::enableOverflowInterrupt(const uint8_t a_enable)
{

    if (a_enable) {

        TIMER2_ENABLE_OVERFLOW_INTERRUPT;

    } else {

        TIMER2_DISABLE_OVERFLOW_INTERRUPT;
    }

}
