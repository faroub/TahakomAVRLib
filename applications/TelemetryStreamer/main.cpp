/**
 * @file main.cpp
 * @brief Continuous hardware telemetry streaming over USART0
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2026
*/

#include "MCU.h"
#include "Telemetry.h"
#include "ADC.h"
#include "PushButton.h"

int main(void) {
    core::MCU::init();

    io::USART0 &usart = io::USART0::getInstance();
    io::Telemetry::init(usart);

    core::ADConverter &adc = core::ADConverter::getInstance(
        core::referenceVoltage::AVCC,
        core::clockPrescaler::PS_128,
        core::autoTriggerSource::freeRunning,
        io::Pin(0, io::PortC)
    );
    adc.start();

    component::PushButton button(io::Pin(2, io::PortD), 1, 1);

    io::Telemetry::sendLog("Telemetry Streamer Started");

    while (1) {
        uint16_t adcVal = adc.getConversionResult(core::resolution::res_10bit);
        uint8_t btnState = button.isPressed();

        io::Telemetry::sendADC(0, adcVal);
        io::Telemetry::sendPin(2, 2, btnState);

        _delay_ms(100);
    }

    return 0;
}
