#include "LDR.h"
#include <math.h>

component::LDR::LDR(const io::Pin &ar_pin, core::ADConverter &ar_ADC)
               : m_pin(ar_pin), mr_ADC(ar_ADC)

{
    m_pin.toInput(0);

}

component::LDR::~LDR()
{

}

uint32_t component::LDR::getResistance(const uint8_t &ar_VDResistor)
{
    // 10bit resolution -> 1023
    return ar_VDResistor*((1000UL*1023/mr_ADC.getConversionResult(core::resolution::res_10bit))-1000UL);
}

uint16_t component::LDR::getLux(const uint8_t &ar_VDResistor)
{
    // see: https://www.digikey.com/en/maker/projects/design-a-luxmeter-with-an-ldr-and-an-arduino/623aeee0f93e427bb57e02c4592567d1
    return  12518931 * pow(ar_VDResistor*((1000UL*1023/mr_ADC.getConversionResult(core::resolution::res_10bit))-1000UL), -1.405);  ;
}
