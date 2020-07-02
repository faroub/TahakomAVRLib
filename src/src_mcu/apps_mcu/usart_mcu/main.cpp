/**
 * @file main.cpp
 * @brief Simple application to blink a Led
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/
#include "PushButton.h"
#include "Led.h"

#define LED_PIN_NUMBER 0  /**< pin number to which a Led is connected */
#define PUSHBUTTON_PIN_NUMBER 2  /**< pin number to which a Led is connected */

#define DELAYTIME 1000 /**< time delay */


int main(void) {

  // Init

    component::Led Led(io::Pin(0,io::PortB));
    component::PushButton PushButton(io::Pin(2,io::PortD));


  // Mainloop
  while (1) {

      if (PushButton.isPressed())
      {


              Led.on();
        }
        else
        {
          Led.off();


        }
    }

  return 0;
}









//#include "USART0.h"



//int main(void) {

//  // Init
//  volatile char l_receiverBuffer[10];
//  volatile char l_transmitterBuffer[10];
//  volatile int16_t l_dataSize;

//  // instantiate the USART0 object
//  io::USART0 &myUSART0 = io::USART0::getInstance();

//  // Mainloop
//  while (1) {

//    l_dataSize = 1;

//    if (myUSART0.ready2Receive())
//    {
//        myUSART0.receiveFrame(reinterpret_cast<volatile uint8_t*>(l_receiverBuffer),l_dataSize);
//    }

//    l_dataSize = 1;

//    if (myUSART0.ready2Send())
//    {
//        myUSART0.sendFrame(reinterpret_cast<volatile uint8_t*>(l_receiverBuffer),l_dataSize);
//    }





//  }
//  return 0;
//}




