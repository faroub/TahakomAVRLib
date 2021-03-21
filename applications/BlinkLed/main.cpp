/**
 * @file main.cpp
 *
 * Demo example: Blink a Led
 *
 * Usage demonstration of the TahakomAVRLib in a simple blink a Led example.
 * This example demonstrates the use of the component::Led() and io::Pin() abstraction objects.
 *
 * - Compiler: avr-gcc (GCC) 5.4.0
 * - Supported devices:  The example compiles on the ATmega48P/88P/168P/328P AVR family
 *
 *
 * @author Farid Oubbati (farid.oubbati@outlook.com)
 * @date March 2018
*/

#include "Led.h"
#include <util/delay.h>

/**
 *  Led pin number
*/
#define PIN_NUMBER 0

/**
 *  Time delay
*/
#define TIMEDELAY 500

int main(void) {

  // Init

  // Instantiate a Led object
  component::Led Led(io::Pin(PIN_NUMBER,io::PortB));

  // Mainloop
  while (1) {

      Led.on();
      _delay_ms(TIMEDELAY);

      Led.off();
      _delay_ms(TIMEDELAY);
  }
  return 0;
}
