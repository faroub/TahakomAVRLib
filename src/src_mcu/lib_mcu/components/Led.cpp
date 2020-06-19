#include "Led.h"

component::Led::Led(const io::Pin &ar_pin)
         : mr_pin(ar_pin)
{
    mr_pin.toOutput();
}

component::Led::~Led()
{

}


void component::Led::on()
{
    mr_pin.setHigh();

}

void component::Led::off()
{
    mr_pin.setLow();

}


void component::Led::toggle()
{
    mr_pin.toggle();

}

uint8_t component::Led::isOn()
{
    return mr_pin.isHigh();

}


uint8_t component::Led::isOff()
{
    return mr_pin.isLow();

}

