#include "PushButton.h"

component::PushButton::PushButton(const io::Pin& ar_pin, const uint8_t &ar_useInternalPullUp, const uint8_t &ar_isActiveLow)
                                  : mr_pin(ar_pin),
                                    mr_isActiveLow(ar_isActiveLow),
                                    mr_useInternalPullUp(ar_useInternalPullUp)


{
    mr_pin.toInput(mr_useInternalPullUp);
    m_buttonPressed = 0;
}

component::PushButton::~PushButton()
{

}


uint8_t component::PushButton::isPressed()
{
    if (mr_isActiveLow || mr_useInternalPullUp) {
        if (mr_pin.isLow()) {
            _delay_us(PUSHBUTTON_DEBOUNCE_TIME_US);
            if (mr_pin.isLow()) {
                ++m_buttonPressed;
                return 1;
            }
        }
    } else {

        if (mr_pin.isHigh()) {
            _delay_us(PUSHBUTTON_DEBOUNCE_TIME_US);
            if (mr_pin.isHigh()) {
                ++m_buttonPressed;
                return 1;
            }
        }

    }
    m_buttonPressed = 0;
    return 0;

}

uint8_t component::PushButton::getPressedCount() const
{

    return m_buttonPressed;

}

void component::PushButton::resetPressedCount()
{
    m_buttonPressed = 0;

}
