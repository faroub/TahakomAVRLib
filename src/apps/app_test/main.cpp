/**
 * @file main.cpp
 * @brief test mcu application
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/


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



//#include "MCU.h"
//#include "TimerCounter0.h"
//#include "DCMotor.h"

//#define DCMOTOR_NUMBER 6

//int main(void) {

//   // Init
//   // initialize MCU
//   core::MCU::init();

//   // instantiate the TimerCounter0 object
//   core::TimerCounter0 &myTimerCounter0 = core::TimerCounter0::getInstance();
//   myTimerCounter0.selectClockSource(core::clockSource::PS_1024);

//   // instantiate a DCMotor object
//   component::DCMotor myDCMotor(io::Pin(DCMOTOR_NUMBER,io::PortD));


//   myDCMotor.connect(myTimerCounter0);

//   myDCMotor.spin(myTimerCounter0,0);
//   _delay_ms(5000);

//   myDCMotor.spin(myTimerCounter0,75);
//   _delay_ms(5000);

//   myDCMotor.spin(myTimerCounter0,190);
//   _delay_ms(5000);

//   myDCMotor.spin(myTimerCounter0,200);
//   _delay_ms(5000);

//   myDCMotor.disconnect(myTimerCounter0);


//   // Mainloop
//   while (1) {





//   }
//   return 0;
//}
