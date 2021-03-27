/**
 *
 * @author Farid Oubbati (farid.oubbati@outlook.com)
 * @date March 2018
 *
 *  *
 * See Led.h for a description of this code
*/

#include "Led.h"

component::Led::Led(const io::Pin &ar_pin)
         : m_pin(ar_pin)
{
    m_pin.toOutput();
}


component::Led::~Led()
{

}


void component::Led::on()
{
    m_pin.setHigh();

}

void component::Led::off()
{
    m_pin.setLow();

}


void component::Led::toggle()
{
    m_pin.toggle();

}

uint8_t component::Led::isOn()
{
    return m_pin.isHigh();

}


uint8_t component::Led::isOff()
{
    return m_pin.isLow();

}

