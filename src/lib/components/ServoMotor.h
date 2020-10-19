/**
 * @file ServoMotor.h
 * @brief Header file of the ServoMotor class
 *
 * Usage example (test):
 #include "MCU.h"
 #include "ServoMotor.h"
 #include "TimerCounter1.h"

 #define SERVOMOTOR_NUMBER 1
 #define SERVOMOTOR_PULSE_CYCLE 30000 // pulse cycle [us]
 #define SERVOMOTOR_PULSE_WIDTH_MIN 500// pulse width min [us]
 #define SERVOMOTOR_PULSE_WIDTH_MID 1520 // pulse width mid [us]
 #define SERVOMOTOR_PULSE_WIDTH_MAX 3000 // pulse width max [us]



 int main(void) {

    // Init
    // initialize MCU
    core::MCU::init();

    // instantiate the TimerCounter0 object
    core::TimerCounter1 &myTimerCounter1 = core::TimerCounter1::getInstance();
    myTimerCounter1.selectClockSource(core::clockSource::PS_8);
    myTimerCounter1.selectOperationMode(core::operationMode::fast_PWM_ICR);
    myTimerCounter1.selectCompareOutputMode(core::channel::A, core::compareOutputMode::clear);

    // instantiate the Buzzer object
    component::ServoMotor myServoMotor(io::Pin(SERVOMOTOR_NUMBER,io::PortB),
                                       myTimerCounter1.getClockPrescaler(),
                                       SERVOMOTOR_PULSE_CYCLE,
                                       SERVOMOTOR_PULSE_WIDTH_MIN,
                                       SERVOMOTOR_PULSE_WIDTH_MID,
                                       SERVOMOTOR_PULSE_WIDTH_MAX);

    myTimerCounter1.setInputCaptureRegister(myServoMotor.getPulseCycleCount());
    myTimerCounter1.setOutputCompareRegister(core::channel::A, myServoMotor.getRotationAngleCount(0));
    myTimerCounter1.start();
    _delay_ms(2000);
    myTimerCounter1.setOutputCompareRegister(core::channel::A, myServoMotor.getRotationAngleCount(45));
    myTimerCounter1.start();
    _delay_ms(2000);
    myTimerCounter1.setOutputCompareRegister(core::channel::A, myServoMotor.getRotationAngleCount(90));
    myTimerCounter1.start();
    _delay_ms(2000);
    myTimerCounter1.setOutputCompareRegister(core::channel::A, myServoMotor.getRotationAngleCount(135));
    myTimerCounter1.start();
    _delay_ms(2000);
    myTimerCounter1.setOutputCompareRegister(core::channel::A, myServoMotor.getRotationAngleCount(180));
    myTimerCounter1.start();
    _delay_ms(2000);
    myTimerCounter1.setOutputCompareRegister(core::channel::A, myServoMotor.getRotationAngleCount(0));
    myTimerCounter1.start();
    _delay_ms(2000);

    // Mainloop
    while (1) {



    }
    return 0;
 }
 *
 * class to control a servo motor
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/
#ifndef SERVOMOTOR_H
#define SERVOMOTOR_H
#include "ha_base.h"
#include "Pin.h"


namespace component
{


class ServoMotor
{
public:

    ServoMotor(const io::Pin &ar_pin,
               const uint16_t &ar_clockPrescaler=0,
               const uint16_t &ar_pulseCycle=0,
               const uint16_t &ar_pulseWidthMin=0,
               const uint16_t &ar_pulseWidthMid=0,
               const uint16_t &ar_pulseWidthMax=0);

    ~ServoMotor();

    /** Turn servo motor On.
         */
    void on();
    /** Turn servo motor Off.
         */
    void off();
    /** Toggle servo motor state.
         */
    void toggle();

    void setPulseCycleCount(const uint16_t &ar_pulseCycle, const uint16_t &ar_clockPrescaler);

    void setPulseWidthMinCount(const uint16_t &ar_pulseWidthMin, const uint16_t &ar_clockPrescaler);

    void setPulseWidthMidCount(const uint16_t &ar_pulseWidthMid, const uint16_t &ar_clockPrescaler);

    void setPulseWidthMaxCount(const uint16_t &ar_pulseWidthMax, const uint16_t &ar_clockPrescaler);

    uint16_t getRotationAngleCount(const uint8_t &ar_angle_deg);

    uint16_t getPulseCycleCount();


protected:

private:
    io::Pin m_pin; /**< pin object */

    uint16_t m_pulseCycle; /**< pulse cycle [us] */

    uint16_t m_pulseWidthMin; /**< pulse width min [us] */

    uint16_t m_pulseWidthMid; /**< pulse width mid [us] */

    uint16_t m_pulseWidthMax; /**< pulse width max [us] */








};


}



#endif
