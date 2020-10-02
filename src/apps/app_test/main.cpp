/**
 * @file main.cpp
 * @brief test mcu application
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/

#include "PushButton.h"
#include "Led.h"
#include "ExternInterrupt.h"
#include "MCU.h"

#define PUSHBUTTON_NUMBER 2
#define LED_NUMBER 0

int main(void) {

    // Init
    // initialize MCU
    core::MCU::init();

    // instantiate a Led object
    component::Led Led(io::Pin(LED_NUMBER,io::PortB));

    // instantiate a Led object
    component::PushButton PushButton(io::Pin(PUSHBUTTON_NUMBER,io::PortD));

    // instantiate the external interrupt manager
    core::ExternInterrupt &myExternInterrupt = core::ExternInterrupt::getInstance();
    myExternInterrupt.enableInt0(1);
    myExternInterrupt.setInt0SenseControl(core::senseControl::logicalChange);

    // set sleep mode
    core::MCU::selectSleepMode(core::sleepMode::powerDown);


    // Mainloop
    while (1) {

        //flash the LED
        for (uint8_t i=0;i<10;i++)
        {
            Led.on();
            _delay_ms(100);
            Led.off();
            _delay_ms(100);
        }
        _delay_ms(5000);
        Led.on();
        _delay_ms(100);
        Led.off();
        core::MCU::goToSleep(core::BODMode::enabled);

    }
    return 0;
}

void core::ExternInterrupt::Int0ServiceRoutine()
{


}


