/**
 * @file main.cpp
 * @brief Demo example: Blink a Led
 *
 * Usage demonstration of the TahakomAVRLib in a simple blink a Led example.
 * This example demonstrates the use of the Led and Pin abstraction objects.
 *
 * - Compiler:           gcc-avr
 * - Supported devices:  The example compiles on the ATmega48P/88P/168P/328P AVR family
 *
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2018
*/

#include "Led.h"
#include <util/delay.h>

#define PIN_NUMBER 0  /**< Led pin number */
#define TIMEDELAY 500 /**< Time delay */

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
