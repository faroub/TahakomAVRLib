/**
 * @file PIDController.cpp
 * @brief Implementation of the PID Controller class
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2026
*/

#include "PIDController.h"
#include <math.h>

utils::PIDController::PIDController(float a_kp, float a_ki, float a_kd, float a_minOutput, float a_maxOutput)
    : m_integral(0.0f),
      m_prevInput(0.0f),
      m_firstRun(1)
{
    setGains(a_kp, a_ki, a_kd);
    setOutputLimits(a_minOutput, a_maxOutput);
}

void utils::PIDController::setGains(float a_kp, float a_ki, float a_kd)
{
    m_kp = (isnan(a_kp) || isinf(a_kp)) ? 0.0f : a_kp;
    m_ki = (isnan(a_ki) || isinf(a_ki)) ? 0.0f : a_ki;
    m_kd = (isnan(a_kd) || isinf(a_kd)) ? 0.0f : a_kd;
}

void utils::PIDController::setOutputLimits(float a_minOutput, float a_maxOutput)
{
    if (isnan(a_minOutput) || isinf(a_minOutput)) a_minOutput = -255.0f;
    if (isnan(a_maxOutput) || isinf(a_maxOutput)) a_maxOutput = 255.0f;

    if (a_minOutput > a_maxOutput) {
        float tmp = a_minOutput;
        a_minOutput = a_maxOutput;
        a_maxOutput = tmp;
    }

    m_minOutput = a_minOutput;
    m_maxOutput = a_maxOutput;
}

void utils::PIDController::reset()
{
    m_integral = 0.0f;
    m_prevInput = 0.0f;
    m_firstRun = 1;
}

float utils::PIDController::compute(float a_setpoint, float a_input, float a_dt_s)
{
    if (isnan(a_setpoint) || isinf(a_setpoint) || isnan(a_input) || isinf(a_input) || isnan(a_dt_s) || isinf(a_dt_s) || a_dt_s <= 0.0f) {
        return 0.0f;
    }

    if (m_firstRun) {
        m_prevInput = a_input;
        m_firstRun = 0;
    }

    float error = a_setpoint - a_input;

    // Proportional term
    float p_out = m_kp * error;

    // Integral term
    m_integral += error * a_dt_s;

    // Clamp integral term to prevent windup
    if (m_ki > 0.0f) {
        float max_i = fabsf(m_maxOutput) / m_ki;
        if (m_integral > max_i) m_integral = max_i;
        if (m_integral < -max_i) m_integral = -max_i;
    }

    float i_out = m_ki * m_integral;

    // Derivative on measurement (prevents derivative kick on setpoint change)
    float d_input = (a_input - m_prevInput) / a_dt_s;
    float d_out = -m_kd * d_input;

    float total = p_out + i_out + d_out;

    // Anti-windup clamping
    if (total > m_maxOutput) {
        total = m_maxOutput;
        m_integral -= error * a_dt_s;
    } else if (total < m_minOutput) {
        total = m_minOutput;
        m_integral -= error * a_dt_s;
    }

    m_prevInput = a_input;
    return total;
}
