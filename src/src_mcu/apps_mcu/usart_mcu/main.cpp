/**
 * @file main.cpp
 * @brief Simple application to blink a Led
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/
#include "USART0.h"
#include <util/delay.h>

#define PIN_NUMBER 1  /**< pin number to which a Led is connected */
#define DELAYTIME 1000 /**< time delay */


int main(void) {

  // Init
  uint8_t serialCharacter;

  // instantiate the USART0 object
  io::USART0 &myUSART0 = io::USART0::getInstance();

  // Mainloop
  while (1) {

    myUSART0.receiveFrame(&serialCharacter,1);
    myUSART0.sendFrame(&serialCharacter,1);



  }
  return 0;
}




