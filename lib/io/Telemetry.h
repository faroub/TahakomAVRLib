/**
 * @file Telemetry.h
 * @brief Header file for the Telemetry class
 *
 * Provides real-time hardware telemetry streaming over USART0.
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2026
*/

#ifndef TELEMETRY_H
#define TELEMETRY_H

#include "ha_base.h"
#include "USART0.h"

namespace io
{

enum class TelemetryType : uint8_t {
    ADCSample = 0,
    PinState,
    MotorState,
    CustomLog
};

class Telemetry
{
public:
    /** Initialize telemetry system
     *
     *  @param ar_usart Reference to active USART0 instance
     */
    static void init(io::USART0 &ar_usart = io::USART0::getInstance());

    /** Send ADC channel sample
     *
     *  @param a_channel ADC pin channel (0..15)
     *  @param a_value Raw 16-bit ADC value
     */
    static void sendADC(uint8_t a_channel, uint16_t a_value);

    /** Send Pin state
     *
     *  @param a_port Port index (0=PortB, 1=PortC, 2=PortD)
     *  @param a_pin Pin index (0..7)
     *  @param a_state Logic level (0 or 1)
     */
    static void sendPin(uint8_t a_port, uint8_t a_pin, uint8_t a_state);

    /** Send Motor position & speed
     *
     *  @param a_pos Current step or pulse position
     *  @param a_speed Current speed
     */
    static void sendMotor(uint16_t a_pos, uint16_t a_speed);

    /** Send custom log string
     *
     *  @param ap_msg Text message
     */
    static void sendLog(const char *ap_msg);

private:
    static io::USART0 *mp_usart;
};

}

#endif
