/**
 * @file main.cpp
 * @brief test mcu application
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/


#include "StepperMotor.h"
#include "USART0.h"
#include "MCU.h"

#define StepperMotor_Pin_1 0
#define StepperMotor_Pin_2 1
#define StepperMotor_Pin_3 2
#define StepperMotor_Pin_4 3

#define STEP_ANGLE_FULL_STEP 0.176
#define STEP_ANGLE_HALF_STEP 0.0879



int main(void) {

    // initialize MCU
    core::MCU::init();

    // instantiate the USART0 object
    io::USART0 &myUSART0 = io::USART0::getInstance();

    myUSART0.sendString("Stepper Motor Speed!\r\n");



    // instantiate the StepperMotor object
    component::StepperMotor myStepperMotor(component::mode::halfStep,
                                           io::Pin(StepperMotor_Pin_1,io::PortB),
                                           io::Pin(StepperMotor_Pin_2,io::PortB),
                                           io::Pin(StepperMotor_Pin_3,io::PortB),
                                           io::Pin(StepperMotor_Pin_4,io::PortB));

    int16_t l_angle = 180;
    int16_t l_step = static_cast<int16_t>(l_angle / STEP_ANGLE_HALF_STEP);

    // set speed [steps/s]
    // full step: max speed: 500p/s
    // half step: max speed: 1000p/s
   

    // set speed [°/s]
    // full step: max speed: 40°/s
    // half step: max speed: 80°/s

//    myStepperMotor.computeStepDelay(3095, 1000, 1000, 1000);
//    myUSART0.sendWord(myStepperMotor.m_accelTime);
//    myUSART0.sendString("\r\n");
//    myUSART0.sendWord(myStepperMotor.m_constSpeedTime);
//    myUSART0.sendString("\r\n");
//    myUSART0.sendWord(myStepperMotor.m_decelTime);
//    myUSART0.sendString("\r\n");


   // Mainloop
   while (1) {

        while (!myStepperMotor.goalReached())
        {
            // set motor steps
            //myStepperMotor.step(l_step,1000);
            //myStepperMotor.step(l_step,80,STEP_ANGLE_HALF_STEP);
            myStepperMotor.step(3095, 1000, 1000, 1000);
//            myUSART0.sendLong(myStepperMotor.m_temp1);
//            myUSART0.sendString("\t\t");
//            myUSART0.sendLong(myStepperMotor.m_temp2);
//            myUSART0.sendString("\r\n");

        }
   }
   return 0;
}


