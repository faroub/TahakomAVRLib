#include "DCMotor.h"

component::DCMotor::DCMotor(const io::Pin &ar_pin)
       : m_pin(ar_pin)
{
    m_pin.toOutput();

}


component::DCMotor::~DCMotor()
{

}

void component::DCMotor::on()
{
    m_pin.setHigh();
}

void component::DCMotor::off()
{
    m_pin.setLow();
}

void component::DCMotor::toggle()
{
    m_pin.toggle();

}
