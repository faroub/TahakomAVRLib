/**
 * @file main.cpp
 * @brief Smart Home Peripheral Gateway Project
 *
 * Integrates OLED display, pushbutton door lock control, servo actuator,
 * buzzer alert, LDR ambient light sensor, and serial telemetry.
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2026
*/

#include "MCU.h"
#include "SSD1306_OLED.h"
#include "ServoMotor.h"
#include "PushButton.h"
#include "Buzzer.h"
#include "LDR.h"
#include "Telemetry.h"

int main(void) {
    core::MCU::init();

    io::USART0 &usart = io::USART0::getInstance();
    io::Telemetry::init(usart);

    component::SSD1306_OLED oled(0x3C, 128, 64);
    oled.init();
    oled.clear();
    oled.setCursor(0, 0);
    oled.print("SmartHome Gateway");

    component::PushButton button(io::Pin(2, io::PortD), 1, 1);
    component::ServoMotor servo(io::Pin(1, io::PortB));
    component::Buzzer buzzer(io::Pin(3, io::PortD));
    component::LDR ldr(io::Pin(0, io::PortC), 5000UL);

    uint8_t isLocked = 1;
    servo.setAngle(0);

    io::Telemetry::sendLog("SmartHome Gateway Active");

    while (1) {
        uint16_t lux = ldr.getLux();
        io::Telemetry::sendADC(0, lux);

        if (button.isPressed()) {
            isLocked = !isLocked;
            if (isLocked) {
                servo.setAngle(0);
                buzzer.buzz(200, 100);
                oled.setCursor(0, 2);
                oled.print("Status: LOCKED  ");
                io::Telemetry::sendLog("Door Locked");
            } else {
                servo.setAngle(90);
                buzzer.buzz(500, 200);
                oled.setCursor(0, 2);
                oled.print("Status: UNLOCKED");
                io::Telemetry::sendLog("Door Unlocked");
            }
        }

        _delay_ms(100);
    }

    return 0;
}
