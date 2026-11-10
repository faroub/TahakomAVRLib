/**
 * @file main.cpp
 * @brief Closed-loop DC motor velocity control using PID and Rotary Encoder
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2026
*/

#include "MCU.h"
#include "DCMotor.h"
#include "RotaryEncoder.h"
#include "PIDController.h"

int main(void) {
    core::MCU::init();

    // Instantiate DC motor on Port B pins 0 and 1
    component::DCMotor motor(io::Pin(0, io::PortB), io::Pin(1, io::PortB));

    // Instantiate Rotary Encoder on Port D pins 2 and 3
    component::RotaryEncoder encoder(io::Pin(2, io::PortD), io::Pin(3, io::PortD));

    // Instantiate PID Controller (Kp=1.5, Ki=0.2, Kd=0.05, output range: -255 to 255)
    utils::PIDController pid(1.5f, 0.2f, 0.05f, -255.0f, 255.0f);

    float targetVelocity = 100.0f; // Target steps / dt
    int32_t lastPos = 0;

    while (1) {
        encoder.update();
        int32_t currentPos = encoder.getPosition();
        float currentVelocity = static_cast<float>(currentPos - lastPos);
        lastPos = currentPos;

        // Compute PID output
        float pwmOutput = pid.compute(targetVelocity, currentVelocity, 0.05f);

        if (pwmOutput > 0) {
            motor.forward();
        } else if (pwmOutput < 0) {
            motor.backward();
        } else {
            motor.stop();
        }

        _delay_ms(50);
    }

    return 0;
}
