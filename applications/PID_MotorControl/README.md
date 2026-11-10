# PID Closed-Loop DC Motor Control Application

This application demonstrates closed-loop speed control of a DC motor using:
* `utils::PIDController`: Proportional-Integral-Derivative control loop with anti-windup.
* `component::DCMotor`: H-bridge DC motor driver.
* `component::RotaryEncoder`: Quadrature encoder feedback.

## Hardware Connections

* DC Motor Pins: Port B, Pins 0 and 1
* Rotary Encoder Pins: Port D, Pins 2 and 3
* Microcontroller: ATmega328P @ 16 MHz
