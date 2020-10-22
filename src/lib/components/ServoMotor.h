/**
 * @file ServoMotor.h
 * @brief Header file of the ServoMotor class
 *
 * Usage example (separate):
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
                                      SERVOMOTOR_PULSE_CYCLE,
                                      SERVOMOTOR_PULSE_WIDTH_MIN,
                                      SERVOMOTOR_PULSE_WIDTH_MID,
                                      SERVOMOTOR_PULSE_WIDTH_MAX);

   myTimerCounter1.setInputCaptureRegister(myServoMotor.computePulseCycleCount(myTimerCounter1.getClockPrescaler()));
   myTimerCounter1.setOutputCompareRegister(core::channel::A, myServoMotor.computeRotationAngleCount(0,myTimerCounter1.getClockPrescaler()));
   myTimerCounter1.start();
   _delay_ms(2000);
   myTimerCounter1.setOutputCompareRegister(core::channel::A, myServoMotor.computeRotationAngleCount(45,myTimerCounter1.getClockPrescaler()));
   myTimerCounter1.start();
   _delay_ms(2000);
   myTimerCounter1.setOutputCompareRegister(core::channel::A, myServoMotor.computeRotationAngleCount(90,myTimerCounter1.getClockPrescaler()));
   myTimerCounter1.start();
   _delay_ms(2000);
   myTimerCounter1.setOutputCompareRegister(core::channel::A, myServoMotor.computeRotationAngleCount(135,myTimerCounter1.getClockPrescaler()));
   myTimerCounter1.start();
   _delay_ms(2000);
   myTimerCounter1.setOutputCompareRegister(core::channel::A, myServoMotor.computeRotationAngleCount(180,myTimerCounter1.getClockPrescaler()));
   myTimerCounter1.start();
   _delay_ms(2000);
   myTimerCounter1.setOutputCompareRegister(core::channel::A, myServoMotor.computeRotationAngleCount(0,myTimerCounter1.getClockPrescaler()));
   myTimerCounter1.start();
   _delay_ms(2000);

   // Mainloop
   while (1) {



   }
   return 0;
 }
 * Usage example (TimerCounter1):
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

    // instantiate the TimerCounter1 object
    core::TimerCounter1 &myTimerCounter1 = core::TimerCounter1::getInstance();
    myTimerCounter1.selectClockSource(core::clockSource::PS_8);


    // instantiate the Buzzer object
    component::ServoMotor myServoMotor(io::Pin(SERVOMOTOR_NUMBER,io::PortB),
                                       SERVOMOTOR_PULSE_CYCLE,
                                       SERVOMOTOR_PULSE_WIDTH_MIN,
                                       SERVOMOTOR_PULSE_WIDTH_MID,
                                       SERVOMOTOR_PULSE_WIDTH_MAX);

    myServoMotor.connect(myTimerCounter1);

    myServoMotor.rotate(myTimerCounter1,0);
    _delay_ms(2000);
    myServoMotor.rotate(myTimerCounter1,45);
    _delay_ms(2000);
    myServoMotor.rotate(myTimerCounter1,90);
    _delay_ms(2000);
    myServoMotor.rotate(myTimerCounter1,135);
    _delay_ms(2000);
    myServoMotor.rotate(myTimerCounter1,180);
    _delay_ms(2000);
    myServoMotor.rotate(myTimerCounter1,0);
    _delay_ms(2000);

    myServoMotor.disconnect(myTimerCounter1);

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
#include "TimerCounter1.h"



namespace component
{


class ServoMotor
{
public:

    ServoMotor(const io::Pin &ar_pin,
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

    uint16_t computePulseCycleCount(const uint16_t &ar_clockPrescaler);

    uint16_t computePulseWidthMinCount(const uint16_t &ar_clockPrescaler);

    uint16_t computePulseWidthMidCount(const uint16_t &ar_clockPrescaler);

    uint16_t computePulseWidthMaxCount(const uint16_t &ar_clockPrescaler);

    uint16_t computeRotationAngleCount(const uint8_t &ar_angle_deg, const uint16_t &ar_clockPrescaler);


    void rotate(core::TimerCounter1 &ar_timerCounter1,
                const uint8_t &ar_angle_deg,
                const core::channel &ar_channel=core::channel::A);
    void connect(core::TimerCounter1 &ar_timerCounter1,
                 const core::channel &ar_channel=core::channel::A);
    void disconnect(core::TimerCounter1 &ar_timerCounter1,
                    const core::channel &ar_channel=core::channel::A);



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
