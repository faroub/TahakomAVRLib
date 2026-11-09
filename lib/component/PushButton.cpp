/**
 *
 * @author Farid Oubbati (farid.oubbati@outlook.com)
 * @date March 2018
 *
 *  *
 * See PushButton.h for a description of this code
*/

#include "PushButton.h"

component::PushButton::PushButton(const io::Pin& ar_pin, uint8_t ar_useInternalPullUp, uint8_t ar_isActiveLow)
                                  : m_pin(ar_pin),
                                    m_isActiveLow(ar_isActiveLow),
                                    m_useInternalPullUp(ar_useInternalPullUp),
                                    m_lastState(0),
                                    m_buttonPressed(0)

{
    m_pin.toInput(m_useInternalPullUp);
}

component::PushButton::~PushButton()
{

}


uint8_t component::PushButton::isPressed()
{
    uint8_t l_pressed = 0;

    if (m_isActiveLow) {
        if (m_pin.isLow()) {
            _delay_us(PUSHBUTTON_DEBOUNCE_TIME_US);
            if (m_pin.isLow()) {
                l_pressed = 1;
            }
        }
    } else {
        if (m_pin.isHigh()) {
            _delay_us(PUSHBUTTON_DEBOUNCE_TIME_US);
            if (m_pin.isHigh()) {
                l_pressed = 1;
            }
        }
    }

    // count one press per release->press transition
    if (l_pressed && !m_lastState) {
        ++m_buttonPressed;
    }
    m_lastState = l_pressed;

    return l_pressed;

}

uint8_t component::PushButton::getPressedCount() const
{

    return m_buttonPressed;

}

void component::PushButton::resetPressedCount()
{
    m_buttonPressed = 0;

}
