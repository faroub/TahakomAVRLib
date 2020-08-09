#include "TimerCounter0.h"


core::TimerCounter0& core::TimerCounter0::getInstance(const operationMode &ar_operationMode,
                                                      const clockSource &ar_clockSource,
                                                      const compareOutputMode &ar_compareOutputMode)
{
    static TimerCounter0 l_instance(ar_operationMode,
                                    ar_clockSource,
                                    ar_compareOutputMode);

    return l_instance;


}

core::TimerCounter0::TimerCounter0(const operationMode &ar_operationMode,
                                   const clockSource &ar_clockSource,
                                   const compareOutputMode& ar_compareOutputMode)
{
    selectOperationMode(ar_operationMode);
    selectClockSource(ar_clockSource);
    selectCOMChannelA(ar_compareOutputMode);
    selectCOMChannelB(ar_compareOutputMode);


}
core::TimerCounter0::~TimerCounter0()
{

}


void core::TimerCounter0::selectOperationMode(const operationMode &ar_operationMode)
{

    TIMER0_SELECT_OPERATION_MODE(static_cast<uint8_t>(ar_operationMode));

}


void core::TimerCounter0::selectClockSource(const clockSource &ar_clockSource)
{

    TIMER0_SELECT_CLOCK_SOURCE(static_cast<uint8_t>(ar_clockSource));

}

void core::TimerCounter0::stopTimer()
{
    TIMER0_STOP;
}


void core::TimerCounter0::selectCOMChannelA(const compareOutputMode &ar_compareOutputMode)
{

    TIMER0_SELECT_COM_CHANNEL_A(static_cast<uint8_t>(ar_compareOutputMode));
}

void core::TimerCounter0::selectCOMChannelB(const compareOutputMode &ar_compareOutputMode)
{
    TIMER0_SELECT_COM_CHANNEL_B(static_cast<uint8_t>(ar_compareOutputMode));
}

void core::TimerCounter0::setCounter(uint16_t *ap_dataBuffer)
{
    TCNT0 = static_cast<uint8_t>(*ap_dataBuffer);
}

uint16_t core::TimerCounter0::getCounter() const
{
    return TCNT0;
}

void core::TimerCounter0::setOCRChannelA(uint16_t *ap_dataBuffer)
{
    OCR0A = static_cast<uint8_t>(*ap_dataBuffer);
}

void core::TimerCounter0::setOCRChannelB(uint16_t *ap_dataBuffer)
{
    OCR0B = static_cast<uint8_t>(*ap_dataBuffer);
}

uint16_t core::TimerCounter0::getOCRChannelA() const
{
    return OCR0A;
}

uint16_t core::TimerCounter0::getOCRChannelB() const
{
    return OCR0B;
}
