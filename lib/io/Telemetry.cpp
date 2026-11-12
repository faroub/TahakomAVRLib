/**
 * @file Telemetry.cpp
 * @brief Implementation of the Telemetry class
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2026
*/

#include "Telemetry.h"

io::USART0* io::Telemetry::mp_usart = nullptr;

void io::Telemetry::init(io::USART0 &ar_usart)
{
    mp_usart = &ar_usart;
}

void io::Telemetry::sendADC(uint8_t a_channel, uint16_t a_value)
{
    if (!mp_usart) {
        mp_usart = &io::USART0::getInstance();
    }
    a_channel &= 0x0F; // Clamp channel 0..15

    while (!mp_usart->ready2Send()) {}
    mp_usart->sendString("$ADC,");
    mp_usart->sendByte(a_channel);
    while (!mp_usart->ready2Send()) {}
    mp_usart->sendString(",");
    mp_usart->sendWord(a_value);
    while (!mp_usart->ready2Send()) {}
    mp_usart->sendString("\r\n");
}

void io::Telemetry::sendPin(uint8_t a_port, uint8_t a_pin, uint8_t a_state)
{
    if (!mp_usart) {
        mp_usart = &io::USART0::getInstance();
    }
    a_port %= 3;      // Clamp port 0..2
    a_pin &= 0x07;    // Clamp pin 0..7
    a_state = a_state ? 1 : 0;

    while (!mp_usart->ready2Send()) {}
    mp_usart->sendString("$PIN,");
    mp_usart->sendByte(a_port);
    while (!mp_usart->ready2Send()) {}
    mp_usart->sendString(",");
    mp_usart->sendByte(a_pin);
    while (!mp_usart->ready2Send()) {}
    mp_usart->sendString(",");
    mp_usart->sendByte(a_state);
    while (!mp_usart->ready2Send()) {}
    mp_usart->sendString("\r\n");
}

void io::Telemetry::sendMotor(uint16_t a_pos, uint16_t a_speed)
{
    if (!mp_usart) {
        mp_usart = &io::USART0::getInstance();
    }
    while (!mp_usart->ready2Send()) {}
    mp_usart->sendString("$MOTOR,");
    mp_usart->sendWord(a_pos);
    while (!mp_usart->ready2Send()) {}
    mp_usart->sendString(",");
    mp_usart->sendWord(a_speed);
    while (!mp_usart->ready2Send()) {}
    mp_usart->sendString("\r\n");
}

void io::Telemetry::sendLog(const char *ap_msg)
{
    if (!mp_usart) {
        mp_usart = &io::USART0::getInstance();
    }
    if (!ap_msg) {
        ap_msg = "NULL";
    }
    while (!mp_usart->ready2Send()) {}
    mp_usart->sendString("$LOG,");
    while (!mp_usart->ready2Send()) {}
    mp_usart->sendString(ap_msg);
    while (!mp_usart->ready2Send()) {}
    mp_usart->sendString("\r\n");
}
