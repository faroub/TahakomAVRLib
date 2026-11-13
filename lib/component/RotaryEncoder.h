/**
 * @file RotaryEncoder.h
 * @brief Header file for the Quadrature Rotary Encoder driver
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2026
*/

#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

#include "ha_base.h"
#include "Pin.h"

namespace component
{

class RotaryEncoder
{
public:
    /** Constructor
     *
     *  @param ar_pinA Pin for Channel A
     *  @param ar_pinB Pin for Channel B
     *  @param a_useInternalPullUp Enable internal pullup resistors
     */
    RotaryEncoder(const io::Pin &ar_pinA, const io::Pin &ar_pinB, uint8_t a_useInternalPullUp = 1);

    /** Poll encoder pins and update position
     *
     *  Should be called periodically or inside a pin change ISR.
     */
    void update();

    /** Get current step position counter
     */
    int32_t getPosition() const;

    /** Reset position counter to 0
     */
    void resetPosition();

    /** Get last rotation direction (+1 = CW, -1 = CCW, 0 = stationary)
     */
    int8_t getDirection() const;

private:
    io::Pin m_pinA;
    io::Pin m_pinB;
    uint8_t m_lastState;
    int32_t m_position;
    int8_t m_direction;
};

}

#endif
