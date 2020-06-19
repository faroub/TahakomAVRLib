#include "Pin.h"

//io::Port PortB = { &DDRB, &PORTB, &PINB };
//io::Port PortC = { &DDRC, &PORTC, &PINC };
//io::Port PortD = { &DDRD, &PORTD, &PIND };

io::Pin::Pin(const uint8_t &ar_pinNumber, const Port &mr_portName)
         : mr_portName(mr_portName), mr_pinNumber(ar_pinNumber)
{

}


io::Pin::~Pin()

{

}

void io::Pin::toOutput()
{
    *mr_portName.mp_ddrReg  |=  (1 << mr_pinNumber);
}

void io::Pin::toInput()
{
    *mr_portName.mp_portReg &= ~(1 << mr_pinNumber);
    *mr_portName.mp_ddrReg  &= ~(1 << mr_pinNumber);
}

void io::Pin::setLow()
{
    if (*mr_portName.mp_ddrReg & (1 << mr_pinNumber))
    {
        *mr_portName.mp_portReg &= ~(1 << mr_pinNumber);
    }
}

void io::Pin::setHigh()
{
    if (*mr_portName.mp_ddrReg & (1 << mr_pinNumber))
    {
        *mr_portName.mp_portReg |=  (1 << mr_pinNumber);
    }
}

void io::Pin::toggle()
{
    if (*mr_portName.mp_ddrReg & (1 << mr_pinNumber))
    {
        *mr_portName.mp_portReg ^=   1 << mr_pinNumber;
    }
}

uint8_t io::Pin::isHigh() const
{
    return   *mr_portName.mp_pinReg & (1 << mr_pinNumber);
}

uint8_t io::Pin::isLow() const
{
    return   !(*mr_portName.mp_pinReg & (1 << mr_pinNumber));
}

uint8_t io::Pin::getPinNumber() const
{
    return mr_pinNumber;
}
