/**
 * @file PIDController.h
 * @brief Header file for the PID Controller class
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2026
*/

#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include <stdint.h>

namespace utils
{

class PIDController
{
public:
    /** Constructor
     *
     *  @param a_kp Proportional gain
     *  @param a_ki Integral gain
     *  @param a_kd Derivative gain
     *  @param a_minOutput Minimum output limit
     *  @param a_maxOutput Maximum output limit
     */
    PIDController(float a_kp, float a_ki, float a_kd, float a_minOutput = -255.0f, float a_maxOutput = 255.0f);

    /** Set PID gains
     */
    void setGains(float a_kp, float a_ki, float a_kd);

    /** Set output saturation limits
     */
    void setOutputLimits(float a_minOutput, float a_maxOutput);

    /** Reset integral accumulator and previous error state
     */
    void reset();

    /** Compute PID output step
     *
     *  @param a_setpoint Desired target value
     *  @param a_input Measured feedback input
     *  @param a_dt_s Sample time in seconds
     *  @return Calculated PID output clamped to limits
     */
    float compute(float a_setpoint, float a_input, float a_dt_s);

private:
    float m_kp;
    float m_ki;
    float m_kd;
    float m_minOutput;
    float m_maxOutput;
    float m_integral;
    float m_prevError;
};

}

#endif
