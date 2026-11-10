/**
 * @file main.cpp
 * @brief I2C OLED display dashboard application
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2026
*/

#include "MCU.h"
#include "TWI.h"
#include "ADC.h"
#include "SSD1306_OLED.h"

int main(void) {
    core::MCU::init();

    // Initialize ADC on PC0
    core::ADConverter &adc = core::ADConverter::getInstance(
        core::referenceVoltage::AVCC,
        core::clockPrescaler::PS_128,
        core::autoTriggerSource::freeRunning,
        io::Pin(0, io::PortC)
    );
    adc.start();

    // Initialize SSD1306 OLED over I2C at address 0x3C
    component::SSD1306_OLED oled(0x3C, 128, 64);
    oled.init();
    oled.clear();

    oled.setCursor(0, 0);
    oled.print("TahakomAVR OLED");

    while (1) {
        uint16_t val = adc.getConversionResult(core::resolution::res_10bit);

        oled.setCursor(0, 2);
        oled.print("ADC Ch0: ");

        char buf[6];
        buf[0] = '0' + (val / 1000);
        buf[1] = '0' + ((val / 100) % 10);
        buf[2] = '0' + ((val / 10) % 10);
        buf[3] = '0' + (val % 10);
        buf[4] = '\0';
        oled.print(buf);

        _delay_ms(200);
    }

    return 0;
}
