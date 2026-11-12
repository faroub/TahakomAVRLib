/**
 * @file main.cpp
 * @brief ESP32 Wi-Fi & Serial Telemetry Streaming Application Example
 *
 * Demonstrates basic GPIO output, ADC light sensor reading, and telemetry
 * frame transmission ($ADC, $LOG) on ESP32 using TahakomAVRLib.
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2026
 */

#include "Led.h"
#include "ADC.h"
#include "Telemetry.h"
#include "MCU.h"

int main(void)
{
    // Initialize MCU hardware abstraction
    core::MCU::init();

    // Configure status LED on GPIO 2 (NodeMCU onboard LED)
    component::Led statusLed(io::Pin(2, io::PortB));
    statusLed.on();

    // Get singleton instance of USART0 for telemetry framing
    io::USART0 &usart = io::USART0::getInstance();
    io::Telemetry::init(usart);

    io::Telemetry::sendLog("ESP32 TahakomAVRLib Telemetry Node Booted");

    // Configure ADC Converter
    core::ADConverter &adc = core::ADConverter::getInstance(
        core::referenceVoltage::AVCC,
        core::clockPrescaler::PS_128,
        core::autoTriggerSource::freeRunning,
        io::Pin(0, io::PortC)
    );
    adc.start();

    uint16_t sampleCounter = 0;

    while (1) {
        // Read 10-bit analog light sensor value
        uint16_t sensorVal = adc.getConversionResult(core::resolution::res_10bit);

        // Send telemetry frame to Web IDE oscilloscope dashboard
        io::Telemetry::sendADC(0, sensorVal);

        // Toggle LED indicator
        statusLed.toggle();

        sampleCounter++;
        if (sampleCounter % 50 == 0) {
            io::Telemetry::sendLog("ESP32 Telemetry Status: OK");
        }

        _delay_ms(100);
    }

    return 0;
}
