#include "TimerCounter2.h"


core::TimerCounter2& core::TimerCounter2::getInstance(const operationMode &ar_operationMode,
                                                      const clockSource &ar_clockSource,
                                                      const compareOutputMode& ar_compareOutputMode)
{
    static TimerCounter2 l_instance(ar_operationMode,
                                    ar_clockSource,
                                    ar_compareOutputMode);

    return l_instance;


}

core::TimerCounter2::TimerCounter2(const operationMode &ar_operationMode,
                                   const clockSource &ar_clockSource,
                                   const compareOutputMode& ar_compareOutputMode)
{
    selectOperationMode(ar_operationMode);
    selectClockSource(ar_clockSource);
    selectCOMChannelA(ar_compareOutputMode);
    selectCOMChannelB(ar_compareOutputMode);

}
core::TimerCounter2::~TimerCounter2()
{

}


void core::TimerCounter2::selectOperationMode(const operationMode &ar_operationMode)
{

        TIMER2_SELECT_OPERATION_MODE(static_cast<uint8_t>(ar_operationMode));


}


void core::TimerCounter2::selectClockSource(const clockSource &ar_clockSource)
{

    TIMER2_SELECT_CLOCK_SOURCE(static_cast<uint8_t>(ar_clockSource));

}

void core::TimerCounter2::stopTimer()
{
    TIMER2_STOP;
}


void core::TimerCounter2::selectCOMChannelA(const compareOutputMode &ar_compareOutputMode)
{

    TIMER2_SELECT_COM_CHANNEL_A(static_cast<uint8_t>(ar_compareOutputMode));
}

void core::TimerCounter2::selectCOMChannelB(const compareOutputMode &ar_compareOutputMode)
{
    TIMER2_SELECT_COM_CHANNEL_B(static_cast<uint8_t>(ar_compareOutputMode));
}

void core::TimerCounter2::setCounter(uint16_t *ap_dataBuffer)
{
    TCNT2 = static_cast<uint8_t>(*ap_dataBuffer);
}

uint16_t core::TimerCounter2::getCounter() const
{
    return TCNT2;
}

void core::TimerCounter2::setOCRChannelA(uint16_t *ap_dataBuffer)
{
    OCR2A = static_cast<uint8_t>(*ap_dataBuffer);
}

void core::TimerCounter2::setOCRChannelB(uint16_t *ap_dataBuffer)
{
    OCR2B = static_cast<uint8_t>(*ap_dataBuffer);
}

uint16_t core::TimerCounter2::getOCRChannelA() const
{
    return OCR2A;
}

uint16_t core::TimerCounter2::getOCRChannelB() const
{
    return OCR2B;
}
