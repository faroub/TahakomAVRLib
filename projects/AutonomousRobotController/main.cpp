/**
 * @file main.cpp
 * @brief Autonomous Robot Controller Project
 *
 * Differential drive chassis controller using dual DC motors, quadrature encoders,
 * closed-loop PID velocity control, and telemetry streaming.
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2026
*/

#include "MCU.h"
#include "DCMotor.h"
#include "TimerCounter0.h"
#include "TimerCounter2.h"
#include "RotaryEncoder.h"
#include "PIDController.h"
#include "Telemetry.h"

int main(void) {
    core::MCU::init();

    io::USART0 &usart = io::USART0::getInstance();
    io::Telemetry::init(usart);

    core::TimerCounter0 &timer0 = core::TimerCounter0::getInstance();
    timer0.selectClockSource(core::clockSource::PS_64);

    core::TimerCounter2 &timer2 = core::TimerCounter2::getInstance();
    timer2.selectClockSource(core::clockSource::PS_64);

    // Left Motor & Encoder
    component::DCMotor motorLeft(io::Pin(6, io::PortD));
    motorLeft.connect(timer0);
    component::RotaryEncoder encoderLeft(io::Pin(2, io::PortD), io::Pin(3, io::PortD));
    utils::PIDController pidLeft(1.5f, 0.2f, 0.05f, 0.0f, 255.0f);

    // Right Motor & Encoder
    component::DCMotor motorRight(io::Pin(3, io::PortB));
    motorRight.connect(timer2);
    component::RotaryEncoder encoderRight(io::Pin(4, io::PortD), io::Pin(5, io::PortD));
    utils::PIDController pidRight(1.5f, 0.2f, 0.05f, 0.0f, 255.0f);

    float targetSpeed = 80.0f;
    int32_t lastLeft = 0;
    int32_t lastRight = 0;

    io::Telemetry::sendLog("Robot Drive Controller Active");

    while (1) {
        encoderLeft.update();
        encoderRight.update();

        int32_t posL = encoderLeft.getPosition();
        int32_t posR = encoderRight.getPosition();

        float speedL = static_cast<float>(posL - lastLeft);
        float speedR = static_cast<float>(posR - lastRight);

        lastLeft = posL;
        lastRight = posR;

        float outL = pidLeft.compute(targetSpeed, speedL, 0.05f);
        float outR = pidRight.compute(targetSpeed, speedR, 0.05f);

        motorLeft.spin(timer0, static_cast<uint16_t>(outL));
        motorRight.spin(timer2, static_cast<uint16_t>(outR));

        io::Telemetry::sendMotor(static_cast<uint16_t>(posL), static_cast<uint16_t>(speedL));

        _delay_ms(50);
    }

    return 0;
}
