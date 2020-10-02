/**
 * @file MCU.h
 * @brief Header file of the MCU class
 *
 * Basic class abstraction of the MCU.
 *
 * Usage example (test):
 *
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

 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/
#ifndef MCU_H
#define MCU_H
#include "ha_base.h"


namespace core
{

enum class BODMode : uint8_t {
    enabled=0,    /**<  */
    disabled,    /**<  */
};

enum class sleepMode : uint8_t {
    Idle=0,    /**<  */
    ADC_NoiseReduction,    /**<  */
    powerDown,    /**<  */
    powerSave,    /**<  */
    standby=6,    /**<  */
    extendedStandby,    /**<  */
};

class MCU
{
public:

    static void init();

    static void selectSleepMode(const sleepMode &a_sleepMode);

    static void goToSleep(const BODMode &a_BODMode);

    static void sleepEnable(const uint8_t ar_enable);

    static void enableUSART0(const uint8_t a_enable);

    static void enableTimerCounter0(const uint8_t a_enable);

    static void enableTimerCounter1(const uint8_t a_enable);

    static void enableTimerCounter2(const uint8_t a_enable);

    static void enableTWI(const uint8_t a_enable);

    static void enableSPI(const uint8_t a_enable);

    static void enableADC(const uint8_t a_enable);

    static void disableBOD();





protected:



private:







};





}
#endif
