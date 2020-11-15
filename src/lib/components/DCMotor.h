/**
 * @file DCMotor.h
 * @brief Header file of the DCMotor class
 *
 * Usage example (test):
 #include "MCU.h"
 #include "TimerCounter0.h"
 #include "DCMotor.h"

 #define DCMOTOR_NUMBER 6

 int main(void) {

   // Init
   // initialize MCU
   core::MCU::init();

   // instantiate the TimerCounter0 object
   core::TimerCounter0 &myTimerCounter0 = core::TimerCounter0::getInstance();
   myTimerCounter0.selectClockSource(core::clockSource::PS_1024);

   // instantiate a DCMotor object
   component::DCMotor myDCMotor(io::Pin(DCMOTOR_NUMBER,io::PortD));


   myDCMotor.connect(myTimerCounter0);

   myDCMotor.spin(myTimerCounter0,0);
   _delay_ms(5000);

   myDCMotor.spin(myTimerCounter0,75);
   _delay_ms(5000);

   myDCMotor.spin(myTimerCounter0,190);
   _delay_ms(5000);

   myDCMotor.spin(myTimerCounter0,200);
   _delay_ms(5000);

   myDCMotor.disconnect(myTimerCounter0);


   // Mainloop
   while (1) {





   }
   return 0;
 }
 * Usage example (H Bridge):
 *
 #include "MCU.h"
 #include "TimerCounter0.h"
 #include "DCMotor.h"

 #define DCMOTOR_NUMBER 6
 #define DCMOTOR_BACKWARD 0
 #define DCMOTOR_FORWARD 1

 int main(void) {

   // Init
   // initialize MCU
   core::MCU::init();

   // instantiate the TimerCounter0 object
   core::TimerCounter0 &myTimerCounter0 = core::TimerCounter0::getInstance();
   myTimerCounter0.selectClockSource(core::clockSource::PS_256);

   // instantiate a DCMotor object
   component::DCMotor myDCMotor(io::Pin(DCMOTOR_NUMBER,io::PortD));

   // instantiate direction pins objects

   io::Pin myPinForward = io::Pin(DCMOTOR_FORWARD,io::PortB);
   myPinForward.toOutput();

   io::Pin myPinBackward = io::Pin(DCMOTOR_BACKWARD,io::PortB);
   myPinBackward.toOutput();


   myDCMotor.connect(myTimerCounter0);

   myPinForward.setHigh();
   myPinBackward.setLow();
   myDCMotor.spin(myTimerCounter0,30);
   _delay_ms(5000);

   myPinForward.setLow();
   myPinBackward.setHigh();
   myDCMotor.spin(myTimerCounter0,30);
   _delay_ms(5000);

   myDCMotor.disconnect(myTimerCounter0);


   // Mainloop
   while (1) {





   }
   return 0;
 }


 * Basic class for IO abstraction of Pin and Port
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/

#ifndef DCMOTOR_H
#define DCMOTOR_H
#include "ha_base.h"
#include "Pin.h"
#include "TimerCounter.h"

namespace component
{

class DCMotor
{
public:
    DCMotor(const io::Pin &ar_pin);

    ~DCMotor();

    /** Turn servo motor On.
         */
    void on();
    /** Turn servo motor Off.
         */
    void off();
    /** Toggle servo motor state.
         */
    void toggle();

    template<typename TC>
    void spin(TC &ar_timerCounter,
              //const uint8_t &ar_dir,
              const uint16_t &ar_speed,
              const core::channel &ar_channel=core::channel::A)
    {

        ar_timerCounter.setOutputCompareRegister(ar_channel, ar_speed);
        ar_timerCounter.start();


    }
    template<typename TC>
    void stop(TC &ar_timerCounter)
    {
        ar_timerCounter.stop();


    }

    template<typename TC>
    void connect(TC &ar_timerCounter,
                 const core::channel &ar_channel=core::channel::A)
    {
        ar_timerCounter.selectOperationMode(core::operationMode::fast_PWM);
        ar_timerCounter.selectCompareOutputMode(ar_channel, core::compareOutputMode::clear);
        ar_timerCounter.setCounter(0);

    }

    template<typename TC>
    void disconnect(TC &ar_timerCounter,
                    const core::channel &ar_channel=core::channel::A)
    {
        ar_timerCounter.selectCompareOutputMode(ar_channel, core::compareOutputMode::normal);
        ar_timerCounter.stop();
    }



private:

    io::Pin m_pin; /**< pin object */

};

}
#endif // DCMOTOR_H
