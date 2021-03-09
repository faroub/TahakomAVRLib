#include "TimerCounter1.h"
#include "MCU.h"


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
    core::MCU::enableTimerCounter1(1);
    stop();
    selectOperationMode(ar_operationMode);
    selectClockSource(ar_clockSource);
    selectCompareOutputMode(ar_channel,ar_compareOutputMode);


}
core::TimerCounter1::~TimerCounter1()
{

}

void core::TimerCounter1::selectClockSource(const clockSource &ar_clockSource)
{
    switch (ar_clockSource)
    {
        case core::clockSource::noClock:
        {
            m_clockPrescaler=0;
            m_clockSource=0;
            break;
        }
        case core::clockSource::PS_1:
        {
            m_clockPrescaler=1;
            m_clockSource=1;
            break;
        }
        case core::clockSource::PS_8:
        {
            m_clockPrescaler=8;
            m_clockSource=2;
            break;
        }
        case core::clockSource::PS_64:
        {
            m_clockPrescaler=64;
            m_clockSource=3;
            break;
        }
        case core::clockSource::PS_256:
        {
            m_clockPrescaler=256;
            m_clockSource=4;
            break;
        }
        case core::clockSource::PS_1024:
        {
            m_clockPrescaler=1024;
            m_clockSource=5;
            break;
        }
        case core::clockSource::extern_Clock_T0_Falling_Edge:
        {
            m_clockPrescaler=0;
            m_clockSource=6;
            break;
        }
        case core::clockSource::extern_Clock_T0_Rising_Edge:
        {
            m_clockPrescaler=0;
            m_clockSource=7;
            break;
        }
    }


}

void core::TimerCounter1::selectOperationMode(const operationMode &ar_operationMode)
{
    switch (ar_operationMode)
    {
        case core::operationMode::normal:
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
        case core::operationMode::fast_PWM_8bit:
        {
            TIMER1_SELECT_OPERATION_MODE(5);
            break;
        }
        case core::operationMode::fast_PWM_9bit:
        {
            TIMER1_SELECT_OPERATION_MODE(6);
            break;
        }
        case core::operationMode::fast_PWM_10bit:
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
        case core::operationMode::fast_PWM_ICR:
        {
            TIMER1_SELECT_OPERATION_MODE(14);
            break;
        }
        case core::operationMode::fast_PWM_OCR:
        {
            TIMER1_SELECT_OPERATION_MODE(15);
            break;
        }

    }


}


void core::TimerCounter1::start()
{
    TIMER1_SELECT_CLOCK_SOURCE(m_clockSource);
}

uint16_t core::TimerCounter1::getClockPrescaler()
{
    return m_clockPrescaler;
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

void core::TimerCounter1::enableOutputCompareMatchInterrupt(const channel &ar_channel, const uint8_t a_enable)
{
    switch (ar_channel)
    {
        case core::channel::A:
        {
            if (a_enable) {

                TIMER1_ENABLE_COM_CHANNEL_A_INTERRUPT;

            } else {

                TIMER1_DISABLE_COM_CHANNEL_A_INTERRUPT;
            }
            break;
        }
        case core::channel::B:
        {
            if (a_enable) {

                TIMER1_ENABLE_COM_CHANNEL_B_INTERRUPT;

            } else {

                TIMER1_DISABLE_COM_CHANNEL_B_INTERRUPT;

            }
            break;
        }
    }

}



void core::TimerCounter1::enableOverflowInterrupt(const uint8_t a_enable)
{

    if (a_enable) {

        TIMER1_ENABLE_OVERFLOW_INTERRUPT;

    } else {

        TIMER1_DISABLE_OVERFLOW_INTERRUPT;
    }

}

void core::TimerCounter1::enableInputCaptureInterrupt(const uint8_t a_enable)
{
    if (a_enable) {

        TIMER1_ENABLE_INPUT_CAPTURE_INTERRUPT;

    } else {

        TIMER1_DISABLE_INPUT_CAPTURE_INTERRUPT;
    }
}

void core::TimerCounter1::setInputCaptureRegister(const uint16_t &ar_dataBuffer)
{
    ICR1 = ar_dataBuffer;
}


uint16_t core::TimerCounter1::getInputCaptureRegister() const
{
    return ICR1;
}
