/**
 * @file main.cpp
 * @brief Simple application to blink a Led
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/
#include "Led.h"
#include <util/delay.h>

#define PIN_NUMBER 1  /**< pin number to which a Led is connected */
#define DELAYTIME 1000 /**< time delay */


int main(void) {

  // Init


  // instantiate a Led object
  component::Led Led(io::Pin(PIN_NUMBER,io::PortB));



  // Mainloop
  while (1) {


      Led.on();
      _delay_ms(DELAYTIME);

      Led.off();
      _delay_ms(DELAYTIME);


  }
  return 0;
}




