/**
 * @file main.cpp
 * @brief Environment monitor application using PCF8574 I2C LCD and LDR light sensor
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2026
*/

#include "MCU.h"
#include "PCF8574_LCD.h"
#include "LDR.h"
#include "ADC.h"

int main(void) {
    core::MCU::init();

    core::ADConverter &adc = core::ADConverter::getInstance(
        core::referenceVoltage::AVCC,
        core::clockPrescaler::PS_128,
        core::autoTriggerSource::freeRunning,
        io::Pin(0, io::PortC)
    );
    adc.start();

    // Instantiate LDR light sensor on PC0
    component::LDR ldr(io::Pin(0, io::PortC), adc);

    // Instantiate I2C LCD1602 at address 0x27
    component::PCF8574_LCD lcd(0x27, 16, 2);
    lcd.init();
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Light Monitor");

    while (1) {
        uint16_t lux = ldr.getLux(5); // 5 kOhm fixed resistor

        lcd.setCursor(0, 1);
        lcd.print("Lux: ");

        char buf[6];
        buf[0] = '0' + (lux / 10000);
        buf[1] = '0' + ((lux / 1000) % 10);
        buf[2] = '0' + ((lux / 100) % 10);
        buf[3] = '0' + ((lux / 10) % 10);
        buf[4] = '0' + (lux % 10);
        buf[5] = '\0';
        lcd.print(buf);

        _delay_ms(300);
    }

    return 0;
}
