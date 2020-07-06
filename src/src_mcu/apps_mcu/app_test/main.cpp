/**
 * @file main.cpp
 * @brief test mcu application
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/


#include "USART0.h"

#define BUFFER_SIZE 1

int main(void) {

  // Init
  char l_receiverBuffer[BUFFER_SIZE];
  char l_transmitterBuffer[BUFFER_SIZE];
  uint8_t l_ready2Send = 0;
  uint8_t l_ready2Receive = 1;

  // instantiate the USART0 object
  io::USART0 &myUSART0 = io::USART0::getInstance();


  // Mainloop
  while (1) {


    if (l_ready2Send)
    {

        myUSART0.sendFrame(reinterpret_cast<uint8_t*>(l_receiverBuffer),BUFFER_SIZE);
        l_ready2Send = 0;
    }

    myUSART0.receiveFrame(reinterpret_cast<uint8_t*>(l_receiverBuffer),BUFFER_SIZE,l_ready2Receive);

    if (myUSART0.getNumberBytesReceived() /* number of bytes can be tested  */ )
    {
        // extract data received
        // .....
        // send back received data
        l_ready2Send = 1;
        // reset number of bytes after extracting the received data
        myUSART0.resetNumberBytesReceived();
    }






  }
  return 0;
}




