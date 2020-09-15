/**
 * @file ServoMotor.h
 * @brief Header file of the ServoMotor class
 *
 * Usage example (test):
 * #include "ServoMotor.h"
   #include "TimerCounter1.h"


   #define SERVOMOTOR 1
   #define PULSE_MIN 1000
   #define PULSE_MAX 6000
   #define PULSE_MID 3040


   int main(void) {

      // Init
      // instantiate the Buzzer object
      component::ServoMotor myServoMotor(io::Pin(SERVOMOTOR,io::PortB));


      // instantiate the TimerCounter0 object
      core::TimerCounter1 &myTimerCounter1 = core::TimerCounter1::getInstance();


      myServoMotor.rotate(myTimerCounter1,PULSE_MIN);
      _delay_ms(5000);
      myServoMotor.rotate(myTimerCounter1,PULSE_MID);
      _delay_ms(5000);
      myServoMotor.rotate(myTimerCounter1,PULSE_MAX);
      _delay_ms(5000);
      myServoMotor.rotate(myTimerCounter1,PULSE_MIN);
      _delay_ms(5000);

      // ------ Event loop ------ //
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
#include "TimerCounter.h"

// TODO: check the use of other timers
// TODO: implement a function to select angles
// TODO: implement a function to compute value of ICR register

namespace component
{


class ServoMotor
{
public:

    ServoMotor(const io::Pin &ar_pin);

    ~ServoMotor();

    template<typename TC>
    void rotate(TC &ar_timerCounter,
              const uint16_t &ar_angle_deg,
              const core::channel &ar_channel=core::channel::A,
              const core::clockSource &ar_clockSource= core::clockSource::PS_8
              )
    {
        ar_timerCounter.selectOperationMode(core::operationMode::Fast_PWM_ICR);
        ar_timerCounter.selectCompareOutputMode(ar_channel, core::compareOutputMode::Clear);
        ar_timerCounter.setInputCaptureRegister(39999);
        ar_timerCounter.setOutputCompareRegister(ar_channel, ar_angle_deg);

        // start timer
        ar_timerCounter.start(ar_clockSource);



    }

    template<typename TC>
    void stop(TC &ar_timerCounter,const core::channel &ar_channel=core::channel::A)
    {
        ar_timerCounter.selectCompareOutputMode(ar_channel, core::compareOutputMode::Normal);
        ar_timerCounter.stop();

    }

    uint16_t convertAngleToPulseWidth(const uint8_t &ar_angle_deg);

protected:

private:
    io::Pin m_pin; /**< pin object */




};


}



#endif
