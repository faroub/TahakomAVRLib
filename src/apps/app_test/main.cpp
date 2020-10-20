/**
 * @file main.cpp
 * @brief test mcu application
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/


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



