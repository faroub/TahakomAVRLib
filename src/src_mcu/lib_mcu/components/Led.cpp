#include "Led.h"
#include "Pin.h"

components::Led::Led(io::Pin& ar_pin)
         : mr_pin(ar_pin)
{
    mr_pin.toOutput();
}


void components::Led::on()
{
    mr_pin.setHigh();

}

void components::Led::off()
{
    mr_pin.setLow();

}


void components::Led::toggle()
{
    mr_pin.toggle();

}

uint8_t components::Led::isOn()
{
    return mr_pin.isHigh();

}


uint8_t components::Led::isOff()
{
    return mr_pin.isLow();

}

