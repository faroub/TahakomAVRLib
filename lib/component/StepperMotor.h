/**
 * @file ServoMotor.h
 * @brief Header file of the StepperMotor class
 *
 * The StepperMotor class is implemented and tested for the 28BYJ-48 Stepper Motor
 * Usage example (basic):
 *
 #include "StepperMotor.h"

 #define StepperMotor_Pin_1 0
 #define StepperMotor_Pin_2 1
 #define StepperMotor_Pin_3 2
 #define StepperMotor_Pin_4 3

 #define STEP_ANGLE_FULL_STEP 0.176
 #define STEP_ANGLE_HALF_STEP 0.0879



 int main(void) {

    // instantiate the StepperMotor object
    component::StepperMotor myStepperMotor(component::mode::halfStep,
                                           io::Pin(StepperMotor_Pin_1,io::PortB),
                                           io::Pin(StepperMotor_Pin_2,io::PortB),
                                           io::Pin(StepperMotor_Pin_3,io::PortB),
                                           io::Pin(StepperMotor_Pin_4,io::PortB));

    int16_t l_angle = -180;
    int16_t l_step = static_cast<int16_t>(l_angle / STEP_ANGLE_HALF_STEP);

    // set speed [steps/s]
    // full step: max speed: 500p/s
    // half step: max speed: 1000p/s
   

    // set speed [°/s]
    // full step: max speed: 40°/s
    // half step: max speed: 80°/s

   // Mainloop
   while (1) {

        while (!myStepperMotor.goalReached())
        {
            // set motor steps
            myStepperMotor.step(l_step,1000);
            //myStepperMotor.step(l_step,80,STEP_ANGLE_HALF_STEP);

        }
   }
   return 0;
 }
 *
 *
 * class to control a servo motor
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/


#ifndef STEPPERMOTOR_H
#define STEPPERMOTOR_H
#include "ha_base.h"
#include "Pin.h"

namespace component
{

enum class mode : uint8_t {
    fullStep=0,    /**<  */
    halfStep,       /**<  */
};

class StepperMotor
{
public:

    StepperMotor(const mode &ar_mode,
                 const io::Pin &ar_pinCoil1,
                 const io::Pin &ar_pinCoil2,
                 const io::Pin &ar_pinCoil3,
                 const io::Pin &ar_pinCoil4);

    ~StepperMotor();

    /** Turn servo motor On.
         */

    void step(const int16_t a_step,
              const uint16_t a_speed);

    void step(const int16_t a_step, 
              const uint16_t a_speed, 
              const float a_stepAngle);

    void step(const int16_t a_step, 
              const uint16_t a_speed, 
              const uint16_t a_accel, 
              const uint16_t a_decel);

    void stepPulse(const uint8_t a_stepPulse);

    void stepDelay(uint8_t a_stepDelay);

    uint8_t goalReached();

    void setCurrentPos(uint16_t a_currentPos);

    uint16_t currentPos();


    uint8_t computeStepDelay(uint16_t a_step,
                           const uint16_t a_speed,
                           const uint16_t a_accel,
                           const uint16_t a_decel);

    uint16_t m_accelTime; /**< pulse delay in ms */
    uint16_t m_decelTime; /**< pulse delay in ms */
    uint16_t m_constSpeedTime; /**< pulse delay in ms */



private:

//    uint8_t computeStepDelay(int16_t a_step,
//                           const uint16_t a_speed,
//                           const uint16_t a_accel,
//                           const uint16_t a_decel);

    /** Reset the state at the start of a move: the step counter, the
     *  speed profile, and the goal flag.  Called when the goal of the
     *  previous move was reached or when the target changes, so the
     *  state of one move never leaks into the next.
     */
    void beginMove(const int16_t a_step);

    io::Pin m_pinCoil1; /**< pin object */
    io::Pin m_pinCoil2; /**< pin object */
    io::Pin m_pinCoil3; /**< pin object */
    io::Pin m_pinCoil4; /**< pin object */

    mode stepMode; /**< steps per revolution */
    uint8_t m_goalReached;
    int16_t m_stepNumber; /**< step counter of the current move */
    int16_t m_targetStep; /**< target step count of the current move */
    uint16_t m_currentPos; /**< pulse delay in ms */
    // speed profile state of the current move, recomputed per call
    uint32_t m_profileTime; /**< computeStepDelay() call counter */
    uint32_t m_profileSpeed; /**< current speed in 1/1000 steps/s */
    uint64_t m_speedTimeProduct; /**< accumulated speed-time product */
    uint64_t m_profilePosition; /**< product already turned into steps */




};

}
#endif // STEPPERMOTOR_H
