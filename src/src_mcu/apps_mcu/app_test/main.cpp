/**
 * @file main.cpp
 * @brief test mcu application
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/

#include "PushButton.h"
#include "Led.h"
#include "ExternInterrupt.h"

#define PUSHBUTTON_1_NUMBER 2
#define PUSHBUTTON_2_NUMBER 3
#define MAIN_LED_NUMBER 0
#define INTERRUPT_LED_1_NUMBER 1
#define INTERRUPT_LED_2_NUMBER 2
#define DELAYTIME 1000



// instantiate a Led object
// declaration of global variables (shut up warning)
extern component::Led MainLed;
extern component::Led InterruptLed1;
extern component::Led InterruptLed2;
component::Led MainLed(io::Pin(MAIN_LED_NUMBER,io::PortB));
component::Led InterruptLed1(io::Pin(INTERRUPT_LED_1_NUMBER,io::PortB));
component::Led InterruptLed2(io::Pin(INTERRUPT_LED_2_NUMBER,io::PortB));


// instantiate a Led object
// declaration of global variable (shut up warning)
extern component::PushButton PushButton1;
extern component::PushButton PushButton2;
component::PushButton PushButton1(io::Pin(PUSHBUTTON_1_NUMBER,io::PortD));
component::PushButton PushButton2(io::Pin(PUSHBUTTON_2_NUMBER,io::PortB));

int main(void) {

  // Init

  // instantiate the external interrupt manager
  core::ExternInterrupt &myExternInterrupt = core::ExternInterrupt::getInstance();
  myExternInterrupt.enablePinChange(core::pinChangePort::PCINTD,1);
  myExternInterrupt.enablePinChange(core::pinChangePort::PCINTB,1);
  myExternInterrupt.enablePinChangeMaskPortD(PUSHBUTTON_1_NUMBER,1);
  myExternInterrupt.enablePinChangeMaskPortB(PUSHBUTTON_2_NUMBER,1);

  // Mainloop
  while (1) {

      MainLed.toggle();
      _delay_ms(DELAYTIME);

  }
  return 0;
}

void core::ExternInterrupt::pinChangePortDServiceRoutine()
{
    if (PushButton1.isPressed())
    {
        InterruptLed1.on();

    } else {
        InterruptLed1.off();

    }

}
void core::ExternInterrupt::pinChangePortBServiceRoutine()
{
    if (PushButton2.isPressed())
    {
        InterruptLed2.on();
    } else {
        InterruptLed2.off();
    }

}

