#include "ServoMotor.h"



component::ServoMotor::ServoMotor(const io::Pin &ar_pin,
                                  const uint16_t &ar_clockPrescaler,
                                  const uint16_t &ar_pulseCycle,
                                  const uint16_t &ar_pulseWidthMin,
                                  const uint16_t &ar_pulseWidthMid,
                                  const uint16_t &ar_pulseWidthMax)
                      : m_pin(ar_pin)
{
    m_pin.toOutput();
    setPulseCycleCount(ar_pulseCycle,ar_clockPrescaler);
    setPulseWidthMinCount(ar_pulseWidthMin, ar_clockPrescaler);
    setPulseWidthMidCount(ar_pulseWidthMid, ar_clockPrescaler);
    setPulseWidthMaxCount(ar_pulseWidthMax, ar_clockPrescaler);
}

component::ServoMotor::~ServoMotor()
{

}




void component::ServoMotor::on()
{
    m_pin.setHigh();
}

void component::ServoMotor::off()
{
    m_pin.setLow();
}

void component::ServoMotor::toggle()
{
    m_pin.toggle();

}

void component::ServoMotor::setPulseCycleCount(const uint16_t &ar_pulseCycle, const uint16_t &ar_clockPrescaler)
{
    m_pulseCycle = SERVOMOTOR_TIMER_PULSE_WIDTH_COUNT(ar_pulseCycle,ar_clockPrescaler);
}

void component::ServoMotor::setPulseWidthMinCount(const uint16_t &ar_pulseWidthMin, const uint16_t &ar_clockPrescaler)
{
    m_pulseWidthMin = SERVOMOTOR_TIMER_PULSE_WIDTH_COUNT(ar_pulseWidthMin,ar_clockPrescaler);
}

void component::ServoMotor::setPulseWidthMaxCount(const uint16_t &ar_pulseWidthMax, const uint16_t &ar_clockPrescaler)
{
    m_pulseWidthMax = SERVOMOTOR_TIMER_PULSE_WIDTH_COUNT(ar_pulseWidthMax,ar_clockPrescaler);
}

void component::ServoMotor::setPulseWidthMidCount(const uint16_t &ar_pulseWidthMid, const uint16_t &ar_clockPrescaler)
{
    m_pulseWidthMid = SERVOMOTOR_TIMER_PULSE_WIDTH_COUNT(ar_pulseWidthMid,ar_clockPrescaler);
}

uint16_t component::ServoMotor::getRotationAngleCount(const uint8_t &ar_angle_deg)
{
    return static_cast<uint16_t>(SERVOMOTOR_TIMER_ANGLE_COUNT(ar_angle_deg,static_cast<long>(m_pulseWidthMin),static_cast<long>(m_pulseWidthMid),static_cast<long>(m_pulseWidthMax)));
}

uint16_t component::ServoMotor::getPulseCycleCount()
{
    return m_pulseCycle;

}
