/**
 * @file StepperMotor.h
 * @brief Header file of the StepperMotor class
 *
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
    fullStep=0,
    halfStep,
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

    void step(const int16_t a_step, const uint16_t a_speed);
    void step(const int16_t a_step, const uint16_t a_speed, const float a_stepAngle);
    void step(const int16_t a_step, const uint16_t a_speed, const uint16_t a_accel, const uint16_t a_decel);

    void stepPulse(const uint8_t a_stepPulse);
    void stepDelay(uint16_t a_stepDelay);

    uint8_t goalReached();
    void setCurrentPos(uint16_t a_currentPos);
    uint16_t currentPos();

    uint16_t computeStepDelay(uint16_t a_step,
                           const uint16_t a_speed,
                           const uint16_t a_accel,
                           const uint16_t a_decel);

    uint16_t m_accelTime;
    uint16_t m_decelTime;
    uint16_t m_constSpeedTime;

private:
    void beginMove(const int16_t a_step);

    io::Pin m_pinCoil1;
    io::Pin m_pinCoil2;
    io::Pin m_pinCoil3;
    io::Pin m_pinCoil4;

    mode stepMode;
    uint8_t m_goalReached;
    int32_t m_stepNumber;
    int32_t m_targetStep;
    uint16_t m_currentPos;

    uint32_t m_profileTime;
    uint32_t m_profileSpeed;
    uint64_t m_speedTimeProduct;
    uint64_t m_profilePosition;
};

}
#endif // STEPPERMOTOR_H
