#include "ADC.h"

volatile uint16_t* core::ADConverter::mp_conversionResult = nullptr;
uint8_t core::ADConverter::m_conversionComplete = 0;


core::ADConverter& core::ADConverter::getInstance(const referenceVoltage &ar_refVoltage,
                                                  const resultAdjust &ar_adjustResult,
                                                  const clockPrescaler& ar_clockPrescaler,
                                                  const autoTriggerSource& ar_autoTriggerSource,
                                                  const io::Pin &ar_pin)
{
    static ADConverter l_instance(ar_refVoltage,
                                  ar_adjustResult,
                                  ar_clockPrescaler,
                                  ar_autoTriggerSource,
                                  ar_pin);

    return l_instance;
}

core::ADConverter::ADConverter(const referenceVoltage &ar_refVoltage,
                               const resultAdjust &ar_adjustResult,
                               const clockPrescaler& ar_clockPrescaler,
                               const autoTriggerSource& ar_autoTriggerSource,
                               const io::Pin &ar_pin)
{
    selectAnalogInput(ar_pin);
    selectReferenceVoltage(ar_refVoltage);
    adjustResult(ar_adjustResult);
    selectClockPrescaler(ar_clockPrescaler);
    enableAutoTrigger(1);
    selectAutoTriggerSource(ar_autoTriggerSource);
    sei();
    enableConversionCompleteInterrupt(1);

}


core::ADConverter::~ADConverter()
{

}

void core::ADConverter::adjustResult(const resultAdjust &ar_adjustResult)
{
    if (static_cast<uint8_t>(ar_adjustResult)){
        ADC_ADJUST_RESULT_RIGHT;
    } else {
        ADC_ADJUST_RESULT_LEFT;

    }
}

void core::ADConverter::selectReferenceVoltage(const referenceVoltage& ar_refVoltage)
{
    ADC_SELECT_REF_VOLTAGE(static_cast<uint8_t>(ar_refVoltage));

}

void core::ADConverter::selectAnalogInput(io::Pin a_pin)
{
    a_pin.toInput(0);
    ADC_SELECT_ANALOG_INPUT(a_pin.getPinNumber());
    ADC_DISABLE_DIGITAL_INPUT_REGISTER(a_pin.getPinNumber());

}

void core::ADConverter::start()
{
    ADC_ENABLE;
    ADC_START_CONVERSION;
}

void core::ADConverter::stop()
{
    ADC_DISABLE;
    ADC_STOP_CONVERSION;

}

void core::ADConverter::enableAutoTrigger(const uint8_t a_enable)
{
    if (a_enable) {
        ADC_ENABLE_AUTOTRIGGER;

    } else {
        ADC_DISABLE_AUTOTRIGGER;
    }

}

void core::ADConverter::enableConversionCompleteInterrupt(const uint8_t a_enable)
{
    if (a_enable) {
        ADC_ENABLE_CONVERSION_COMPLETE_INTERRUPT;

    } else {
        ADC_DISABLE_CONVERSION_COMPLETE_INTERRUPT;
    }

}

void core::ADConverter::selectClockPrescaler(const clockPrescaler& ar_clockPrescaler)
{
    ADC_SELECT_CLOCK_PRESCALER(static_cast<uint8_t>(ar_clockPrescaler));

}

void core::ADConverter::getConversionResult(uint16_t *ap_dataBuffer)
{
    mp_conversionResult = ap_dataBuffer;



}

void core::ADConverter::selectAutoTriggerSource(const autoTriggerSource& ar_autoTriggerSource)
{
    ADC_SELECT_AUTO_TRIGGER_SOURCE(static_cast<uint8_t>(ar_autoTriggerSource));


}

void core::ADConverter::conversionCompleteServiceRoutine()
{
        m_conversionComplete = 0;
        *mp_conversionResult = ADC;
        m_conversionComplete = 1;



}

uint8_t core::ADConverter::conversionComplete()
{
    return m_conversionComplete;

}
