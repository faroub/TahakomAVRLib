/**
 * @file main.cpp
 * @brief Simple application to blink a Led
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/
#include "USART0.h"



int main(void) {

  // Init
  volatile char l_receiverBuffer[10];
  volatile char l_transmitterBuffer[10];
  volatile int16_t l_dataSize;

  // instantiate the USART0 object
  io::USART0 &myUSART0 = io::USART0::getInstance();

  // Mainloop
  while (1) {

    l_dataSize = 1;

    if (myUSART0.ready2Receive())
    {
        myUSART0.receiveFrame(reinterpret_cast<volatile uint8_t*>(l_receiverBuffer),l_dataSize);
    }

    l_dataSize = 1;

    if (myUSART0.ready2Send())
    {
        myUSART0.sendFrame(reinterpret_cast<volatile uint8_t*>(l_receiverBuffer),l_dataSize);
    }





  }
  return 0;
}




