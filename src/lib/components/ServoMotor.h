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

// TODO: check the use of other timers
// TODO: implement a function to select angles
// TODO: implement a function to compute value of ICR register

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
