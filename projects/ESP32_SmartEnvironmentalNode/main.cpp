/**
 * @file main.cpp
 * @brief ESP32 Smart Environmental Sensing Node Project
 *
 * Integrated hardware project for ESP32 featuring an SSD1306 OLED display,
 * LDR light sensor, Quadrature Rotary Encoder, PID fan velocity controller,
 * and live telemetry streaming ($ADC, $PIN, $MOTOR, $LOG).
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2026
 */

#include "MCU.h"
#include "Led.h"
#include "LDR.h"
#include "ADC.h"
#include "SSD1306_OLED.h"
#include "RotaryEncoder.h"
#include "PIDController.h"
#include "Telemetry.h"

int main(void)
{
    // Initialize MCU hardware
    core::MCU::init();

    // Status Indicator LED on GPIO 2
    component::Led statusLed(io::Pin(2, io::PortB));
    statusLed.on();

    // Telemetry Streamer over USART0
    io::USART0 &usart = io::USART0::getInstance();
    io::Telemetry::init(usart);
    io::Telemetry::sendLog("ESP32 Smart Environmental Node Booted");

    // Configure ADC Converter
    core::ADConverter &adc = core::ADConverter::getInstance(
        core::referenceVoltage::AVCC,
        core::clockPrescaler::PS_128,
        core::autoTriggerSource::freeRunning,
        io::Pin(0, io::PortC)
    );
    adc.start();

    // LDR Light Sensor connected to Analog Pin 0
    component::LDR ldr(io::Pin(0, io::PortC), adc);

    // Rotary Encoder connected to Pins PB0 and PB1
    component::RotaryEncoder encoder(io::Pin(0, io::PortB), io::Pin(1, io::PortB));

    // SSD1306 I2C OLED Display (0x3C I2C Address)
    component::SSD1306_OLED oled(0x3C);
    oled.init();
    oled.clear();
    oled.setCursor(0, 0);
    oled.print("ESP32 ENV NODE");
    oled.setCursor(0, 2);
    oled.print("Status: ACTIVE");

    // Fixed-Point PID Controller for Fan Velocity Loop
    utils::PIDController pid(2.5f, 0.4f, 0.1f, 0.0f, 255.0f);

    float setpoint = 500.0f; // Target light setpoint
    uint32_t loopCounter = 0;

    while (1) {
        // Step 1: Update Rotary Encoder input
        encoder.update();
        int16_t encoderPos = encoder.getPosition();

        // Step 2: Sample LDR Ambient Light
        uint16_t rawAdc = adc.getConversionResult(core::resolution::res_10bit);
        uint16_t lux = ldr.getLux(5000U);
        (void)lux; // Explicitly marked used for sensor telemetry

        // Step 3: Compute PID fan control output (dt = 0.1s)
        float pidOut = pid.compute(setpoint, (float)rawAdc, 0.1f);

        // Step 4: Stream Telemetry to Web IDE Oscilloscope
        io::Telemetry::sendADC(0, rawAdc);
        io::Telemetry::sendPin(1, 2, 1);
        io::Telemetry::sendMotor((uint16_t)encoderPos, (uint16_t)pidOut);

        // Step 5: Periodically Update SSD1306 Display
        if (loopCounter % 10 == 0) {
            oled.clear();
            oled.setCursor(0, 0);
            oled.print("ESP32 ENV NODE");
            oled.setCursor(0, 2);
            oled.print("Lux: ACTIVE");
            oled.setCursor(0, 3);
            oled.print("PID Out: ACTIVE");
        }

        // Toggle Status Blinker
        statusLed.toggle();
        loopCounter++;

        _delay_ms(100);
    }

    return 0;
}
