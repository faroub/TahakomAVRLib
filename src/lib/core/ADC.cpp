#include "ADC.h"

volatile uint16_t* core::ADConverter::mp_conversionResult = nullptr;
volatile uint8_t core::ADConverter::m_conversionComplete = 0;
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
    selectAnalogInput(ar_pin);
    selectReferenceVoltage(ar_refVoltage);
    selectClockPrescaler(ar_clockPrescaler);
    enableAutoTrigger(1);
    selectAutoTriggerSource(ar_autoTriggerSource);
    sei();
    enableConversionCompleteInterrupt(1);


}


core::ADConverter::~ADConverter()
{

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


void core::ADConverter::selectAutoTriggerSource(const autoTriggerSource& ar_autoTriggerSource)
{
    ADC_SELECT_AUTO_TRIGGER_SOURCE(static_cast<uint8_t>(ar_autoTriggerSource));


}

void core::ADConverter::conversionCompleteServiceRoutine()
{

    static uint32_t l_resultData = 0;
    static uint16_t l_resultDataIndex = 0;

    m_conversionComplete = 0;
    switch (m_resolution)
    {
        case 8:
        {
            *mp_conversionResult = ADC >> 8;
            m_conversionComplete = 1;
            break;
        }
        case 9:
        {
            *mp_conversionResult = ADC >> 7;
            m_conversionComplete = 1;
            break;
        }
        case 10:
        {
            *mp_conversionResult = ADC;
            m_conversionComplete = 1;
            break;
        }
        case 11:
        {

            if (l_resultDataIndex < 4)
            {
                l_resultData += ADC;
                l_resultDataIndex++;

            }
            else
            {
                *mp_conversionResult = l_resultData >> 1;
                l_resultData = 0;
                l_resultDataIndex = 0;
                m_conversionComplete = 1;

            }

            break;
        }
        case 12:
        {
            if (l_resultDataIndex < 16)
            {
                l_resultData += ADC;
                l_resultDataIndex++;

            }
            else
            {
                *mp_conversionResult = l_resultData >> 2;
                l_resultData = 0;
                l_resultDataIndex = 0;
                m_conversionComplete = 1;

            }
            break;
        }
        case 13:
        {
            if (l_resultDataIndex < 64)
            {
                l_resultData += ADC;
                l_resultDataIndex++;

            }
            else
            {
                *mp_conversionResult = l_resultData >> 3;
                l_resultData = 0;
                l_resultDataIndex = 0;
                m_conversionComplete = 1;

            }
            break;
        }
        case 14:
        {
            if (l_resultDataIndex < 256)
            {
                l_resultData += ADC;
                l_resultDataIndex++;

            }
            else
            {
                *mp_conversionResult = l_resultData >> 4;
                l_resultData = 0;
                l_resultDataIndex = 0;
                m_conversionComplete = 1;

            }
            break;
        }
        case 15:
        {
            if (l_resultDataIndex < 1024)
            {
                l_resultData += ADC;
                l_resultDataIndex++;

            }
            else
            {
                *mp_conversionResult = l_resultData >> 5;
                l_resultData = 0;
                l_resultDataIndex = 0;
                m_conversionComplete = 1;

            }
            break;
        }
        case 16:
        {
            if (l_resultDataIndex < 4096)
            {

                l_resultData += ADC;
                l_resultDataIndex++;

            }
            else
            {
                *mp_conversionResult = l_resultData >> 6;
                l_resultData = 0;
                l_resultDataIndex = 0;
                m_conversionComplete = 1;

            }
            break;
        }

    }




}

uint8_t core::ADConverter::conversionComplete()
{
    return m_conversionComplete;

}


void core::ADConverter::getConversionResult(uint16_t *ap_resultData, const resolution& ar_resolution)
{
    mp_conversionResult = ap_resultData;

    switch (ar_resolution)
    {
        case core::resolution::RES_8bit:
        {
            ADC_ADJUST_RESULT_LEFT;
            m_resolution = 8;
            break;
        }
        case core::resolution::RES_9bit:
        {
            ADC_ADJUST_RESULT_LEFT;
            m_resolution = 9;
            break;
        }
        case core::resolution::RES_10bit:
        {
            ADC_ADJUST_RESULT_RIGHT;
            m_resolution = 10;
            break;
        }
        case core::resolution::RES_11bit:
        {

            m_resolution = 11;
            break;
        }
        case core::resolution::RES_12bit:
        {
            m_resolution = 12;
            break;
        }
        case core::resolution::RES_13bit:
        {
            m_resolution = 13;
            break;
        }
        case core::resolution::RES_14bit:
        {
            m_resolution = 14;
            break;
        }
        case core::resolution::RES_15bit:
        {
            m_resolution = 15;
            break;
        }
        case core::resolution::RES_16bit:
        {
            m_resolution = 16;
            break;
        }
    }


}
