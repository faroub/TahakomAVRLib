#include "ServoMotor.h"



component::ServoMotor::ServoMotor(const io::Pin &ar_pin)
                 : m_pin(ar_pin)
{
    m_pin.toOutput();

}

component::ServoMotor::~ServoMotor()
{

}

uint16_t component::ServoMotor::convertAngleToPulseWidth(const uint8_t &ar_angle_deg)
{
    return static_cast<uint16_t>(utils::map(ar_angle_deg, 0, 180, 1088, 4800));

}
