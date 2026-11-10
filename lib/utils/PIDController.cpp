/**
 * @file PIDController.cpp
 * @brief Implementation of the PID Controller class
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2026
*/

#include "PIDController.h"

utils::PIDController::PIDController(float a_kp, float a_ki, float a_kd, float a_minOutput, float a_maxOutput)
    : m_kp(a_kp),
      m_ki(a_ki),
      m_kd(a_kd),
      m_minOutput(a_minOutput),
      m_maxOutput(a_maxOutput),
      m_integral(0.0f),
      m_prevError(0.0f)
{
}

void utils::PIDController::setGains(float a_kp, float a_ki, float a_kd)
{
    m_kp = a_kp;
    m_ki = a_ki;
    m_kd = a_kd;
}

void utils::PIDController::setOutputLimits(float a_minOutput, float a_maxOutput)
{
    m_minOutput = a_minOutput;
    m_maxOutput = a_maxOutput;
}

void utils::PIDController::reset()
{
    m_integral = 0.0f;
    m_prevError = 0.0f;
}

float utils::PIDController::compute(float a_setpoint, float a_input, float a_dt_s)
{
    if (a_dt_s <= 0.0f) return 0.0f;

    float error = a_setpoint - a_input;

    // Proportional term
    float p_out = m_kp * error;

    // Integral term with anti-windup clamping
    m_integral += error * a_dt_s;
    float i_out = m_ki * m_integral;

    // Derivative term
    float derivative = (error - m_prevError) / a_dt_s;
    float d_out = m_kd * derivative;

    float total = p_out + i_out + d_out;

    // Output clamping & anti-windup clamping
    if (total > m_maxOutput) {
        total = m_maxOutput;
        m_integral -= error * a_dt_s; // freeze integral integration
    } else if (total < m_minOutput) {
        total = m_minOutput;
        m_integral -= error * a_dt_s;
    }

    m_prevError = error;
    return total;
}
