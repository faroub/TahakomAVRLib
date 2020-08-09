#include "TimerCounter1.h"


core::TimerCounter1& core::TimerCounter1::getInstance(const operationMode &ar_operationMode,
                                                      const clockSource &ar_clockSource,
                                                      const compareOutputMode& ar_compareOutputMode)
{
    static TimerCounter1 l_instance(ar_operationMode,
                                    ar_clockSource,
                                    ar_compareOutputMode);

    return l_instance;


}

core::TimerCounter1::TimerCounter1(const operationMode &ar_operationMode,
                                   const clockSource &ar_clockSource,
                                   const compareOutputMode& ar_compareOutputMode)
{
    selectOperationMode(ar_operationMode);
    selectClockSource(ar_clockSource);
    selectCOMChannelA(ar_compareOutputMode);
    selectCOMChannelB(ar_compareOutputMode);

}
core::TimerCounter1::~TimerCounter1()
{

}


void core::TimerCounter1::selectOperationMode(const operationMode &ar_operationMode)
{

    TIMER1_SELECT_OPERATION_MODE(static_cast<uint8_t>(ar_operationMode));

}


void core::TimerCounter1::selectClockSource(const clockSource &ar_clockSource)
{

    TIMER1_SELECT_CLOCK_SOURCE(static_cast<uint8_t>(ar_clockSource));

}

void core::TimerCounter1::stopTimer()
{
    TIMER1_STOP;
}


void core::TimerCounter1::selectCOMChannelA(const compareOutputMode &ar_compareOutputMode)
{

    TIMER1_SELECT_COM_CHANNEL_A(static_cast<uint8_t>(ar_compareOutputMode));
}

void core::TimerCounter1::selectCOMChannelB(const compareOutputMode &ar_compareOutputMode)
{
    TIMER1_SELECT_COM_CHANNEL_B(static_cast<uint8_t>(ar_compareOutputMode));
}

void core::TimerCounter1::setCounter(uint16_t *ap_dataBuffer)
{
    TCNT1 = *ap_dataBuffer;
}

uint16_t core::TimerCounter1::getCounter() const
{
    return TCNT1;
}

void core::TimerCounter1::setOCRChannelA(uint16_t *ap_dataBuffer)
{
    OCR1A = *ap_dataBuffer;
}

void core::TimerCounter1::setOCRChannelB(uint16_t *ap_dataBuffer)
{
    OCR1B = *ap_dataBuffer;
}

uint16_t core::TimerCounter1::getOCRChannelA() const
{
    return OCR1A;
}

uint16_t core::TimerCounter1::getOCRChannelB() const
{
    return OCR1B;
}
