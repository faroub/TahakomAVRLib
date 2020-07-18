#include "ExternInterrupt.h"

core::ExternInterrupt& core::ExternInterrupt::getInstance()
{
    static ExternInterrupt l_instance;
    return l_instance;

}


core::ExternInterrupt::ExternInterrupt()
{
    sei();

}

core::ExternInterrupt::~ExternInterrupt()
{

}

void core::ExternInterrupt::setInt0SenseControl(const senseControl& ar_senseControl)
{
    EXT_INT_SET_INT0_SENSE_CONTROL(static_cast<uint8_t>(ar_senseControl));

}

void core::ExternInterrupt::setInt1SenseControl(const senseControl& ar_senseControl)
{
    EXT_INT_SET_INT1_SENSE_CONTROL(static_cast<uint8_t>(ar_senseControl));

}

void core::ExternInterrupt::enableInt0(const uint8_t a_enable)
{
    if (a_enable) {
        EXT_INT_ENABLE_INT0;

    } else {
        EXT_INT_DISABLE_INT0;
    }

}

void core::ExternInterrupt::enableInt1(const uint8_t a_enable)
{
    if (a_enable) {
        EXT_INT_ENABLE_INT1;

    } else {
        EXT_INT_DISABLE_INT1;
    }
}


void core::ExternInterrupt::enablePinChange(const pinChangePort& ar_pinChangePort, const uint8_t a_enable)
{
    if (a_enable) {
        EXT_INT_ENABLE_PIN_CHANGE_INTERRUPT(static_cast<uint8_t>(ar_pinChangePort));

    } else {
        EXT_INT_DISABLE_PIN_CHANGE_INTERRUPT(static_cast<uint8_t>(ar_pinChangePort));
    }

}

void core::ExternInterrupt::enablePinChangeMaskPortB(const uint8_t a_pinNumber, const uint8_t a_enable)
{
    if (a_enable) {
        EXT_INT_ENABLE_PIN_CHANGE_MASK_PORTB(a_pinNumber);

    } else {
        EXT_INT_ENABLE_PIN_CHANGE_MASK_PORTB(a_pinNumber);
    }

}


void core::ExternInterrupt::enablePinChangeMaskPortC(const uint8_t a_pinNumber, const uint8_t a_enable)
{
    if (a_enable) {
        EXT_INT_ENABLE_PIN_CHANGE_MASK_PORTC(a_pinNumber);

    } else {
        EXT_INT_ENABLE_PIN_CHANGE_MASK_PORTC(a_pinNumber);
    }

}

void core::ExternInterrupt::enablePinChangeMaskPortD(const uint8_t a_pinNumber, const uint8_t a_enable)
{
    if (a_enable) {
        EXT_INT_ENABLE_PIN_CHANGE_MASK_PORTD(a_pinNumber);

    } else {
        EXT_INT_ENABLE_PIN_CHANGE_MASK_PORTD(a_pinNumber);
    }
}
