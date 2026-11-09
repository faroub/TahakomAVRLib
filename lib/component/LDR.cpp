#include "LDR.h"
#include <math.h>

component::LDR::LDR(const io::Pin &ar_pin, core::ADConverter &ar_ADC)
               : m_pin(ar_pin), mr_ADC(ar_ADC)

{
    // selects the LDR pin as the ADC input and disables its digital
    // input buffer
    mr_ADC.selectAnalogInput(m_pin);
}

component::LDR::~LDR()
{

}

uint32_t component::LDR::getResistance(const uint16_t &ar_VDResistor)
{
    uint16_t l_result = mr_ADC.getConversionResult(core::resolution::res_10bit);
    // an ADC result of 0 would divide by zero: report it as a sentinel
    if (l_result == 0)
    {
        return ADC_ZERO_SENTINEL;
    }
    // 10bit resolution -> 1023
    return (uint32_t)ar_VDResistor * ((1000UL * 1023UL / l_result) - 1000UL);
}

uint16_t component::LDR::getLux(const uint16_t &ar_VDResistor)
{
    uint32_t l_resistance = getResistance(ar_VDResistor);
    // ADC result 0 is the dark end of the scale: no illuminance
    if (l_resistance == ADC_ZERO_SENTINEL)
    {
        return 0;
    }
    // see: https://www.digikey.com/en/maker/projects/design-a-luxmeter-with-an-ldr-and-an-arduino/623aeee0f93e427bb57e02c4592567d1
    float l_lux = 12518931.0f * powf((float)l_resistance, -1.405f);
    // clamp to the uint16_t range: a brightly lit LDR gives a value far
    // above 65535 and the cast used to wrap around
    if (l_lux < 0.0f)
    {
        l_lux = 0.0f;
    }
    if (l_lux > 65535.0f)
    {
        l_lux = 65535.0f;
    }
    return (uint16_t)l_lux;
}
