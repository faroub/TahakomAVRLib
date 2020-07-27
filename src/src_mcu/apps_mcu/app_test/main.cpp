/**
 * @file main.cpp
 * @brief test mcu application
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/

#include "TimerCounter.h"
#include "USART0.h"
#include "PushButton.h"
#include "Led.h"



void randomDelay(void);
void printWord(uint16_t word);

// instantiate the Counter object
extern core::TimerCounter &myCounter;
core::TimerCounter &myCounter = core::TimerCounter::getInstanceTimerCounter0();

// instantiate the USART0 object
extern io::USART0 &myUSART0;
io::USART0 &myUSART0 = io::USART0::getInstance();

#define BUFFER_SIZE 1
#define LED_NUMBER 0
#define PUSHBUTTON_NUMBER 2

int main(void) {

  // Init
  uint16_t timerValue;

  char l_receiverBuffer[BUFFER_SIZE];


  myCounter.selectClockSource(core::timerCounter::timerCounter1, core::clockSource::PS_1024);

  /// instantiate a Led object
  component::Led Led(io::Pin(LED_NUMBER,io::PortB));

  // instantiate a Led object
  component::PushButton PushButton(io::Pin(PUSHBUTTON_NUMBER,io::PortD));



  // ready to send flag
  uint8_t l_ready2Send = 1;

  if (myUSART0.ready2Send())
  {
    myUSART0.sendString("Reaction Timer:\r\n");
  }

  // wait to send next string
  while (!myUSART0.ready2Send()){};

  if (myUSART0.ready2Send())
  {
    myUSART0.sendString("---------------\r\n");
  }

  // wait to send next string
  while (!myUSART0.ready2Send()){};

  if (myUSART0.ready2Send())
  {
    myUSART0.sendString("Press any key to start.\r\n");
  }

  // Mainloop
  while (1) {

      myUSART0.receiveFrame(reinterpret_cast<uint8_t*>(l_receiverBuffer),BUFFER_SIZE);

      // wait for a key press
      while (!myUSART0.getNumberBytesReceived())
      {

      }

      // reset number of bytes after extracting the received data
      myUSART0.resetNumberBytesReceived();

      // wait to send next string
      while (!myUSART0.ready2Send()){};

      if (myUSART0.ready2Send())
      {
          myUSART0.sendString("\r\nGet ready...");
      }

      randomDelay();

      // wait to send next string
      while (!myUSART0.ready2Send()){};

      if (myUSART0.ready2Send())
      {
          myUSART0.sendString("\r\nGo!\r\n");
      }

      Led.on();

      myCounter.resetCounter(core::timerCounter::timerCounter1);

      if (PushButton.isPressed())
      {
          // wait to send next string
          while (!myUSART0.ready2Send()){};

          if (myUSART0.ready2Send())
          {
              myUSART0.sendString("You're only cheating yourself.\r\n");
          }

      } else {

          // wait for a button press
          while (!PushButton.isPressed())
          {

          }
          // get counter value
          myCounter.getCounter(core::timerCounter::timerCounter1, &timerValue);
          // bit shift divide by 16 = 2^4 to convert from micro to millseconds
          timerValue = timerValue >> 4;
          // print response time
          printWord(timerValue);
      }

      Led.off();

      while (!myUSART0.ready2Send()){};

      if (myUSART0.ready2Send())
      {
          myUSART0.sendString("\r\nPress any key to try again.\r\n");
      }

  }
  return 0;
}

void randomDelay(void) {
                        /* Waits for a "random" delay from 1 - 3.5 sec */
                           /* Requires timer 1 initialized and running */
  /* It's not really random, but very hard to control --
     like coin-flipping. */
  uint16_t counter;
//value & 0xFF
  _delay_ms(1000);                              /* wait at least 1 sec */

  myCounter.getCounter(core::timerCounter::timerCounter1, &counter);
  uint8_t randomTime = static_cast<uint8_t>(counter);
  /* type-casting the 16-bit TCNT1 as an 8-bit number keeps
     only the 8 least-significant (fastest-changing) bits  */
  while (--randomTime) {
    _delay_ms(10);                      /* max value is 255 ~= 2.5 sec */
  }
}

void printWord(uint16_t word) {

    while (!myUSART0.ready2Send()){};

    if (myUSART0.ready2Send())
    {
        myUSART0.sendCharacter('0' + (word / 10000));

    }

    while (!myUSART0.ready2Send()){};


    if (myUSART0.ready2Send())
    {
        myUSART0.sendCharacter('0' + ((word / 1000) % 10));
    }

    while (!myUSART0.ready2Send()){};


    if (myUSART0.ready2Send())
    {
        myUSART0.sendCharacter('0' + ((word / 100) % 10));
    }


    while (!myUSART0.ready2Send()){};

    if (myUSART0.ready2Send())
    {
        myUSART0.sendCharacter('0' + ((word / 10) % 10));
    }



    while (!myUSART0.ready2Send()){};

    if (myUSART0.ready2Send())
    {
        myUSART0.sendCharacter('0' + (word % 10));
    }

}



//void printWord(uint16_t word) {

//    char timerValue = '0' + (word / 10000);

//    while (!myUSART0.ready2Send()){};

//    if (myUSART0.ready2Send())
//    {
//        myUSART0.sendFrame(reinterpret_cast<uint8_t*>(&timerValue),BUFFER_SIZE);

//    }

//    timerValue = '0' + ((word / 1000) % 10);

//    while (!myUSART0.ready2Send()){};


//    if (myUSART0.ready2Send())
//    {
//        myUSART0.sendFrame(reinterpret_cast<uint8_t*>(&timerValue),BUFFER_SIZE);

//    }

//    timerValue = '0' + ((word / 100) % 10);

//    while (!myUSART0.ready2Send()){};


//    if (myUSART0.ready2Send())
//    {
//        myUSART0.sendFrame(reinterpret_cast<uint8_t*>(&timerValue),BUFFER_SIZE);

//    }
//     timerValue = '0' + ((word / 10) % 10);

//    while (!myUSART0.ready2Send()){};

//    if (myUSART0.ready2Send())
//    {
//        myUSART0.sendFrame(reinterpret_cast<uint8_t*>(&timerValue),BUFFER_SIZE);

//    }

//    timerValue = '0' + (word % 10);

//    while (!myUSART0.ready2Send()){};

//    if (myUSART0.ready2Send())
//    {
//        myUSART0.sendFrame(reinterpret_cast<uint8_t*>(&timerValue),BUFFER_SIZE);

//    }

//}
