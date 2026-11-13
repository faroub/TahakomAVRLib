/**
 * @file RotaryEncoder.cpp
 * @brief Implementation of the Quadrature Rotary Encoder driver
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2026
*/

#include "RotaryEncoder.h"

// Quadrature state transition table (16 entries)
// CW sequence:  11 (3) -> 01 (1) -> 00 (0) -> 10 (2) -> 11 (3)
// CCW sequence: 11 (3) -> 10 (2) -> 00 (0) -> 01 (1) -> 11 (3)
static const int8_t stateTable[16] = {
     0, -1,  1,  0,  // old state 00 (0): 00->00(0), 00->01(-1), 00->10(+1), 00->11(0)
     1,  0,  0, -1,  // old state 01 (1): 01->00(+1), 01->01(0),  01->10(0),  01->11(-1)
    -1,  0,  0,  1,  // old state 10 (2): 10->00(-1), 10->01(0),  10->10(0),  10->11(+1)
     0,  1, -1,  0   // old state 11 (3): 11->00(0),  11->01(+1), 11->10(-1), 11->11(0)
};

component::RotaryEncoder::RotaryEncoder(const io::Pin &ar_pinA, const io::Pin &ar_pinB, uint8_t a_useInternalPullUp)
    : m_pinA(ar_pinA),
      m_pinB(ar_pinB),
      m_lastState(0),
      m_position(0),
      m_direction(0)
{
    m_pinA.toInput(a_useInternalPullUp);
    m_pinB.toInput(a_useInternalPullUp);
    uint8_t stateA = m_pinA.isHigh() ? 1 : 0;
    uint8_t stateB = m_pinB.isHigh() ? 1 : 0;
    m_lastState = (stateA << 1) | stateB;
}

void component::RotaryEncoder::update()
{
    uint8_t stateA = m_pinA.isHigh() ? 1 : 0;
    uint8_t stateB = m_pinB.isHigh() ? 1 : 0;
    uint8_t currState = (stateA << 1) | stateB;

    uint8_t index = ((m_lastState & 0x03) << 2) | (currState & 0x03);
    int8_t movement = stateTable[index];

    if (movement != 0) {
        m_position += movement;
        m_direction = movement;
    }
    m_lastState = currState;
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
