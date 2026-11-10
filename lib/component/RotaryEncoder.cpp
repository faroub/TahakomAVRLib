/**
 * @file RotaryEncoder.cpp
 * @brief Implementation of the Quadrature Rotary Encoder driver
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2026
*/

#include "RotaryEncoder.h"

component::RotaryEncoder::RotaryEncoder(const io::Pin &ar_pinA, const io::Pin &ar_pinB, uint8_t a_useInternalPullUp)
    : m_pinA(ar_pinA),
      m_pinB(ar_pinB),
      m_lastStateA(0),
      m_lastStateB(0),
      m_position(0),
      m_direction(0)
{
    m_pinA.toInput(a_useInternalPullUp);
    m_pinB.toInput(a_useInternalPullUp);
    m_lastStateA = m_pinA.isHigh();
    m_lastStateB = m_pinB.isHigh();
}

void component::RotaryEncoder::update()
{
    uint8_t stateA = m_pinA.isHigh();
    uint8_t stateB = m_pinB.isHigh();

    if (stateA != m_lastStateA) {
        if (stateA != stateB) {
            m_position++;
            m_direction = 1;
        } else {
            m_position--;
            m_direction = -1;
        }
        m_lastStateA = stateA;
        m_lastStateB = stateB;
    }
}

int32_t component::RotaryEncoder::getPosition() const
{
    return m_position;
}

void component::RotaryEncoder::resetPosition()
{
    m_position = 0;
    m_direction = 0;
}

int8_t component::RotaryEncoder::getDirection() const
{
    return m_direction;
}
