/**
 * @file main.cpp
 * @brief Closed-loop DC motor velocity control using PID and Rotary Encoder
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2026
*/

#include "MCU.h"
#include "DCMotor.h"
#include "TimerCounter0.h"
#include "RotaryEncoder.h"
#include "PIDController.h"

int main(void) {
    core::MCU::init();

    core::TimerCounter0 &timer0 = core::TimerCounter0::getInstance();
    timer0.selectClockSource(core::clockSource::PS_64);

    // Instantiate DC motor on Port D pin 6
    component::DCMotor motor(io::Pin(6, io::PortD));
    motor.connect(timer0);

    // Instantiate Rotary Encoder on Port D pins 2 and 3
    component::RotaryEncoder encoder(io::Pin(2, io::PortD), io::Pin(3, io::PortD));

    // Instantiate PID Controller (Kp=1.5, Ki=0.2, Kd=0.05)
    utils::PIDController pid(1.5f, 0.2f, 0.05f, 0.0f, 255.0f);

    float targetVelocity = 100.0f;
    int32_t lastPos = 0;

    while (1) {
        encoder.update();
        int32_t currentPos = encoder.getPosition();
        float currentVelocity = static_cast<float>(currentPos - lastPos);
        lastPos = currentPos;

        float pwmOutput = pid.compute(targetVelocity, currentVelocity, 0.05f);
        motor.spin(timer0, static_cast<uint16_t>(pwmOutput));

        _delay_ms(50);
    }

    return 0;
}
