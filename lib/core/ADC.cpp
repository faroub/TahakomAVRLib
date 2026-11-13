#include "ADC.h"
#include <util/atomic.h>
#include "MCU.h"

volatile uint16_t core::ADConverter::m_conversionResult = 0;
volatile uint32_t core::ADConverter::m_accumulatedResult = 0;
volatile uint16_t core::ADConverter::m_sampleIndex = 0;
uint8_t core::ADConverter::m_resolution = 10;

core::ADConverter& core::ADConverter::getInstance(const referenceVoltage &ar_refVoltage,
                                                  const clockPrescaler& ar_clockPrescaler,
                                                  const autoTriggerSource& ar_autoTriggerSource,
                                                  const io::Pin &ar_pin)
{
    static ADConverter l_instance(ar_refVoltage,
                                  ar_clockPrescaler,
                                  ar_autoTriggerSource,
                                  ar_pin);
    return l_instance;
}

core::ADConverter::ADConverter(const referenceVoltage &ar_refVoltage,
                               const clockPrescaler& ar_clockPrescaler,
                               const autoTriggerSource& ar_autoTriggerSource,
                               const io::Pin &ar_pin)
{
    core::MCU::enableADC(1);
    selectAnalogInput(ar_pin);
    selectReferenceVoltage(ar_refVoltage);
    selectClockPrescaler(ar_clockPrescaler);
    enableAutoTrigger(1);
    selectAutoTriggerSource(ar_autoTriggerSource);
    enableConversionCompleteInterrupt(1);
}

core::ADConverter::~ADConverter()
{
}

void core::ADConverter::resetOversamplingState()
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        m_accumulatedResult = 0;
        m_sampleIndex = 0;
    }
}

void core::ADConverter::selectReferenceVoltage(const referenceVoltage& ar_refVoltage)
{
    resetOversamplingState();
    ADC_SELECT_REF_VOLTAGE(static_cast<uint8_t>(ar_refVoltage));
}

void core::ADConverter::selectAnalogInput(io::Pin a_pin)
{
    resetOversamplingState();
    a_pin.toInput(0);
    ADC_SELECT_ANALOG_INPUT(a_pin.getPinNumber());
    ADC_DISABLE_DIGITAL_INPUT_REGISTER(a_pin.getPinNumber());
}

void core::ADConverter::start()
{
    resetOversamplingState();
    ADC_ENABLE;
    ADC_START_CONVERSION;
}

void core::ADConverter::stop()
{
    ADC_DISABLE;
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

void core::ADConverter::selectAutoTriggerSource(const autoTriggerSource& ar_autoTriggerSource)
{
    ADC_SELECT_AUTO_TRIGGER_SOURCE(static_cast<uint8_t>(ar_autoTriggerSource));
}

void core::ADConverter::conversionCompleteServiceRoutine()
{
    uint16_t sample = ADC;

    switch (m_resolution)
    {
        case 8:
            m_conversionResult = sample >> 8;
            break;
        case 9:
            m_conversionResult = sample >> 7;
            break;
        case 10:
            m_conversionResult = sample;
            break;
        case 11: // 4 samples, shift 1
            m_accumulatedResult += sample;
            m_sampleIndex++;
            if (m_sampleIndex >= 4) {
                m_conversionResult = static_cast<uint16_t>(m_accumulatedResult >> 1);
                m_accumulatedResult = 0;
                m_sampleIndex = 0;
            }
            break;
        case 12: // 16 samples, shift 2
            m_accumulatedResult += sample;
            m_sampleIndex++;
            if (m_sampleIndex >= 16) {
                m_conversionResult = static_cast<uint16_t>(m_accumulatedResult >> 2);
                m_accumulatedResult = 0;
                m_sampleIndex = 0;
            }
            break;
        case 13: // 64 samples, shift 3
            m_accumulatedResult += sample;
            m_sampleIndex++;
            if (m_sampleIndex >= 64) {
                m_conversionResult = static_cast<uint16_t>(m_accumulatedResult >> 3);
                m_accumulatedResult = 0;
                m_sampleIndex = 0;
            }
            break;
        case 14: // 256 samples, shift 4
            m_accumulatedResult += sample;
            m_sampleIndex++;
            if (m_sampleIndex >= 256) {
                m_conversionResult = static_cast<uint16_t>(m_accumulatedResult >> 4);
                m_accumulatedResult = 0;
                m_sampleIndex = 0;
            }
            break;
        case 15: // 1024 samples, shift 5
            m_accumulatedResult += sample;
            m_sampleIndex++;
            if (m_sampleIndex >= 1024) {
                m_conversionResult = static_cast<uint16_t>(m_accumulatedResult >> 5);
                m_accumulatedResult = 0;
                m_sampleIndex = 0;
            }
            break;
        case 16: // 4096 samples, shift 6
            m_accumulatedResult += sample;
            m_sampleIndex++;
            if (m_sampleIndex >= 4096) {
                m_conversionResult = static_cast<uint16_t>(m_accumulatedResult >> 6);
                m_accumulatedResult = 0;
                m_sampleIndex = 0;
            }
            break;
    }
}

uint16_t core::ADConverter::getConversionResult(const resolution& ar_resolution)
{
    uint8_t new_res = 10;
    switch (ar_resolution)
    {
        case core::resolution::res_8bit:  ADC_ADJUST_RESULT_LEFT;  new_res = 8;  break;
        case core::resolution::res_9bit:  ADC_ADJUST_RESULT_LEFT;  new_res = 9;  break;
        case core::resolution::res_10bit: ADC_ADJUST_RESULT_RIGHT; new_res = 10; break;
        case core::resolution::res_11bit: ADC_ADJUST_RESULT_RIGHT; new_res = 11; break;
        case core::resolution::res_12bit: ADC_ADJUST_RESULT_RIGHT; new_res = 12; break;
        case core::resolution::res_13bit: ADC_ADJUST_RESULT_RIGHT; new_res = 13; break;
        case core::resolution::res_14bit: ADC_ADJUST_RESULT_RIGHT; new_res = 14; break;
        case core::resolution::res_15bit: ADC_ADJUST_RESULT_RIGHT; new_res = 15; break;
        case core::resolution::res_16bit: ADC_ADJUST_RESULT_RIGHT; new_res = 16; break;
    }

    if (new_res != m_resolution) {
        resetOversamplingState();
        m_resolution = new_res;
    }

    uint16_t result;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        result = m_conversionResult;
    }
    return result;
}
