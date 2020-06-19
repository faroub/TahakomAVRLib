/**
 * @file main.cpp
 * @brief Simple application to blink a Led
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/

#include "Led.h"


int main(void) {

  // Init

  component::Led testLed(io::Pin(2,io::PortB));


  // Mainloop
  while (1) {


  }
  return 0;
}




