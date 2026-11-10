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
#include "TimerCounter1.h"
#include "PushButton.h"
#include "Buzzer.h"
#include "LDR.h"
#include "Telemetry.h"

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

    core::TimerCounter1 &timer1 = core::TimerCounter1::getInstance();
    timer1.selectClockSource(core::clockSource::PS_8);

    component::SSD1306_OLED oled(0x3C, 128, 64);
    oled.init();
    oled.clear();
    oled.setCursor(0, 0);
    oled.print("SmartHome Gateway");

    component::PushButton button(io::Pin(2, io::PortD), 1, 1);
    component::ServoMotor servo(io::Pin(1, io::PortB));
    servo.connect(timer1);

    component::Buzzer buzzer(io::Pin(3, io::PortD));
    component::LDR ldr(io::Pin(0, io::PortC), adc);

    uint8_t isLocked = 1;
    servo.rotate(timer1, 0);

    io::Telemetry::sendLog("SmartHome Gateway Active");

    while (1) {
        uint16_t lux = ldr.getLux(5);
        io::Telemetry::sendADC(0, lux);

        if (button.isPressed()) {
            isLocked = !isLocked;
            if (isLocked) {
                servo.rotate(timer1, 0);
                buzzer.buzz(200, 100);
                oled.setCursor(0, 2);
                oled.print("Status: LOCKED  ");
                io::Telemetry::sendLog("Door Locked");
            } else {
                servo.rotate(timer1, 90);
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
