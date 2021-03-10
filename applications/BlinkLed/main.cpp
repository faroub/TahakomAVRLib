/**
 * @brief Demo application: Blink a Led
 *
 * Usage demonstration of the TahakomAVRLib in a simple Blink Led application.
 * This example demonstrates the use of the Led and Pin abstraction objects.
 *
 * - File:               main.cpp
 * - Compiler:           avr
 * - Supported devices:  The example is written for the ATmega382p
 *
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/

#include "Led.h"
#include <util/delay.h>

#define PIN_NUMBER 1  /**< Led pin number */
#define TIMEDELAY 1000 /**< Time delay */


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

