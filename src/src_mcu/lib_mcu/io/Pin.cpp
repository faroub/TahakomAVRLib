#include <Pin.h>

io::Pin::Pin(const uint8_t &ar_pin, const Port &ar_port)
         : mr_port(ar_port), mr_pin(ar_pin)
{

}

void io::Pin::toOutput()
{
    *mr_port.mp_ddrReg  |=  (1 << mr_pin);
}

void io::Pin::toInput()
{
    *mr_port.mp_portReg &= ~(1 << mr_pin);
    *mr_port.mp_ddrReg  &= ~(1 << mr_pin);
}

void io::Pin::setLow()
{
    if (*mr_port.mp_ddrReg & (1 << mr_pin))
    {
        *mr_port.mp_portReg &= ~(1 << mr_pin);
    }
}

void io::Pin::setHigh()
{
    if (*mr_port.mp_ddrReg & (1 << mr_pin))
    {
        *mr_port.mp_portReg |=  (1 << mr_pin);
    }
}

void io::Pin::toggle()
{
    if (*mr_port.mp_ddrReg & (1 << mr_pin))
    {
        *mr_port.mp_portReg ^=   1 << mr_pin;
    }
}

uint8_t io::Pin::isHigh() const
{
    return   *mr_port.mp_pinReg & (1 << mr_pin);
}

uint8_t io::Pin::isLow() const
{
    return   !(*mr_port.mp_pinReg & (1 << mr_pin));
}

uint8_t io::Pin::getPin() const
{
    return mr_pin;
}
