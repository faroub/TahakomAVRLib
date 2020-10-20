#include "ServoMotor.h"



component::ServoMotor::ServoMotor(const io::Pin &ar_pin,
                                  const uint16_t &ar_pulseCycle,
                                  const uint16_t &ar_pulseWidthMin,
                                  const uint16_t &ar_pulseWidthMid,
                                  const uint16_t &ar_pulseWidthMax)
                      : m_pin(ar_pin),
                        m_pulseCycle(ar_pulseCycle),
                        m_pulseWidthMin(ar_pulseWidthMin),
                        m_pulseWidthMid(ar_pulseWidthMid),
                        m_pulseWidthMax(ar_pulseWidthMax)
{
    m_pin.toOutput();

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

uint16_t component::ServoMotor::computePulseCycleCount(const uint16_t &ar_clockPrescaler)
{
    return SERVOMOTOR_TIMER_PULSE_WIDTH_COUNT(m_pulseCycle,ar_clockPrescaler);
}

uint16_t component::ServoMotor::computePulseWidthMinCount(const uint16_t &ar_clockPrescaler)
{
    return SERVOMOTOR_TIMER_PULSE_WIDTH_COUNT(m_pulseWidthMin,ar_clockPrescaler);
}

uint16_t component::ServoMotor::computePulseWidthMaxCount(const uint16_t &ar_clockPrescaler)
{
    return SERVOMOTOR_TIMER_PULSE_WIDTH_COUNT(m_pulseWidthMax,ar_clockPrescaler);
}

uint16_t component::ServoMotor::computePulseWidthMidCount(const uint16_t &ar_clockPrescaler)
{
    return SERVOMOTOR_TIMER_PULSE_WIDTH_COUNT(m_pulseWidthMid,ar_clockPrescaler);
}

uint16_t component::ServoMotor::computeRotationAngleCount(const uint8_t &ar_angle_deg,const uint16_t &ar_clockPrescaler)
{
    return static_cast<uint16_t>(SERVOMOTOR_TIMER_ANGLE_COUNT(ar_angle_deg,static_cast<long>(computePulseWidthMinCount(ar_clockPrescaler)),static_cast<long>(computePulseWidthMidCount(ar_clockPrescaler)),static_cast<long>(computePulseWidthMaxCount(ar_clockPrescaler))));
}



void component::ServoMotor::rotate(core::TimerCounter1 &ar_timerCounter1,
                                   const uint8_t &ar_angle_deg,
                                   const core::channel &ar_channel)
{

    ar_timerCounter1.setOutputCompareRegister(ar_channel, computeRotationAngleCount(ar_angle_deg,ar_timerCounter1.getClockPrescaler()));

    // start timer
    ar_timerCounter1.start();


}


void component::ServoMotor::connect(core::TimerCounter1 &ar_timerCounter1,
             const core::channel &ar_channel)
{


    ar_timerCounter1.setInputCaptureRegister(computePulseCycleCount(ar_timerCounter1.getClockPrescaler()));
    ar_timerCounter1.selectOperationMode(core::operationMode::fast_PWM_ICR);
    ar_timerCounter1.selectCompareOutputMode(ar_channel, core::compareOutputMode::clear);
}

void component::ServoMotor::disconnect(core::TimerCounter1 &ar_timerCounter1,
                const core::channel &ar_channel)
{
    ar_timerCounter1.selectCompareOutputMode(ar_channel, core::compareOutputMode::normal);
    // stop timer
    ar_timerCounter1.stop();
}
