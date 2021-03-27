#include "ADC.h"
#include "MCU.h"

volatile uint16_t core::ADConverter::m_conversionResult = 0;
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
    ADC_STOP_CONVERSION;
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

    static uint32_t l_resultData = 0;
    static uint16_t l_resultDataIndex = 0;



    switch (m_resolution)
    {
        case 8:
        {
            m_conversionResult = ADC >> 8;
            break;
        }
        case 9:
        {
            m_conversionResult = ADC >> 7;
             break;
        }
        case 10:
        {
            m_conversionResult = ADC;
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
                m_conversionResult = l_resultData >> 1;
                l_resultData = 0;
                l_resultDataIndex = 0;
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
                m_conversionResult = l_resultData >> 2;
                l_resultData = 0;
                l_resultDataIndex = 0;
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
                m_conversionResult = l_resultData >> 3;
                l_resultData = 0;
                l_resultDataIndex = 0;
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
                m_conversionResult = l_resultData >> 4;
                l_resultData = 0;
                l_resultDataIndex = 0;
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
                m_conversionResult = l_resultData >> 5;
                l_resultData = 0;
                l_resultDataIndex = 0;
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
                m_conversionResult = l_resultData >> 6;
                l_resultData = 0;
                l_resultDataIndex = 0;
            }
            break;
        }

    }




}




uint16_t core::ADConverter::getConversionResult( const resolution& ar_resolution)
{


    switch (ar_resolution)
    {
        case core::resolution::res_8bit:
        {
            ADC_ADJUST_RESULT_LEFT;
            m_resolution = 8;
            break;
        }
        case core::resolution::res_9bit:
        {
            ADC_ADJUST_RESULT_LEFT;
            m_resolution = 9;
            break;
        }
        case core::resolution::res_10bit:
        {
            ADC_ADJUST_RESULT_RIGHT;
            m_resolution = 10;
            break;
        }
        case core::resolution::res_11bit:
        {

            m_resolution = 11;
            break;
        }
        case core::resolution::res_12bit:
        {
            m_resolution = 12;
            break;
        }
        case core::resolution::res_13bit:
        {
            m_resolution = 13;
            break;
        }
        case core::resolution::res_14bit:
        {
            m_resolution = 14;
            break;
        }
        case core::resolution::res_15bit:
        {
            m_resolution = 15;
            break;
        }
        case core::resolution::res_16bit:
        {
            m_resolution = 16;
            break;
        }
    }

    return m_conversionResult;


}
