/**
 * @file TimerCounter.h
 * @brief Header file of the TimerCounter class
 *
 * Basic class for abstraction of the TimerCounter peripherals.
 *
 * Usage example (Reaction Timer):
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

      myCounter.setCounter(core::timerCounter::timerCounter1,0);

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

    // Waits for a "random" delay from 1 - 3.5 sec
    // Requires timer 1 initialized and running
    // It's not really random, but very hard to control --like coin-flipping.

    uint16_t counter;
    _delay_ms(1000);

    myCounter.getCounter(core::timerCounter::timerCounter1, &counter);
    uint8_t randomTime = static_cast<uint8_t>(counter);
    // type-casting the 16-bit TCNT1 as an 8-bit number keeps
    //     only the 8 least-significant (fastest-changing) bits

    while (--randomTime) {
    _delay_ms(10);
    }
    }

    void printWord(uint16_t word) {

    while (!myUSART0.ready2Send()){};

    if (myUSART0.ready2Send())
    {
        myUSART0.sendByte('0' + (word / 10000));

    }

    while (!myUSART0.ready2Send()){};


    if (myUSART0.ready2Send())
    {
        myUSART0.sendByte('0' + ((word / 1000) % 10));
    }

    while (!myUSART0.ready2Send()){};


    if (myUSART0.ready2Send())
    {
        myUSART0.sendByte('0' + ((word / 100) % 10));
    }


    while (!myUSART0.ready2Send()){};

    if (myUSART0.ready2Send())
    {
        myUSART0.sendByte('0' + ((word / 10) % 10));
    }



    while (!myUSART0.ready2Send()){};

    if (myUSART0.ready2Send())
    {
        myUSART0.sendByte('0' + (word % 10));
    }

    }
    void printWord(uint16_t word) {

    char timerValue = '0' + (word / 10000);

    while (!myUSART0.ready2Send()){};

    if (myUSART0.ready2Send())
    {
       myUSART0.sendFrame(reinterpret_cast<uint8_t*>(&timerValue),BUFFER_SIZE);

    }

    timerValue = '0' + ((word / 1000) % 10);

    while (!myUSART0.ready2Send()){};


    if (myUSART0.ready2Send())
    {
       myUSART0.sendFrame(reinterpret_cast<uint8_t*>(&timerValue),BUFFER_SIZE);

    }

    timerValue = '0' + ((word / 100) % 10);

    while (!myUSART0.ready2Send()){};


    if (myUSART0.ready2Send())
    {
       myUSART0.sendFrame(reinterpret_cast<uint8_t*>(&timerValue),BUFFER_SIZE);

    }
    timerValue = '0' + ((word / 10) % 10);

    while (!myUSART0.ready2Send()){};

    if (myUSART0.ready2Send())
    {
       myUSART0.sendFrame(reinterpret_cast<uint8_t*>(&timerValue),BUFFER_SIZE);

    }

    timerValue = '0' + (word % 10);

    while (!myUSART0.ready2Send()){};

    if (myUSART0.ready2Send())
    {
       myUSART0.sendFrame(reinterpret_cast<uint8_t*>(&timerValue),BUFFER_SIZE);

    }

    }
 *
 * Usage example (PWM):
 *
 #include "Led.h"
 #include "USART0.h"
 #include "TimerCounter1.h"
 #include "TimerCounter2.h"

 uint8_t getNumber(const char *ap_receive);

 #define LED_1 1
 #define LED_2 2
 #define LED_3 3
 #define BUFFER_SIZE 4

 int main(void) {

    // Init
    // receiver buffer
    char l_receiverBuffer[BUFFER_SIZE];

    // instantiate Led objects
    component::Led Led1(io::Pin(LED_1,io::PortB));
    component::Led Led2(io::Pin(LED_2,io::PortB));
    component::Led Led3(io::Pin(LED_3,io::PortB));

    // instantiate USART0 object
    io::USART0 &myUSART0 = io::USART0::getInstance();

    // instantiate Timer1 object
    core::TimerCounter1 &myTimerCounter1 = core::TimerCounter1::getInstance();
    myTimerCounter1.selectOperationMode(core::operationMode::Fast_PWM_8bit);
    myTimerCounter1.selectCompareOutputMode(core::channel::A,core::compareOutputMode::Clear);
    myTimerCounter1.selectCompareOutputMode(core::channel::B,core::compareOutputMode::Clear);
    // instantiate Timer2 object
    core::TimerCounter2 &myTimerCounter2 = core::TimerCounter2::getInstance();
    myTimerCounter2.selectOperationMode(core::operationMode::Fast_PWM);
    myTimerCounter2.selectCompareOutputMode(core::channel::A,core::compareOutputMode::Clear);

    if (myUSART0.ready2Send())
    {
        myUSART0.sendString("-- LED PWM Demo --\r\n");
    }
    // wait to send next string
    while (!myUSART0.ready2Send()){}

    myTimerCounter1.start(core::clockSource::PS_64);
    myTimerCounter2.start(core::clockSource::PS_64);



    // ------ Event loop ------ //
    while (1) {

        if (myUSART0.ready2Send())
        {
            myUSART0.sendString("\r\nEnter (0-255) for PWM duty cycle: ");
        }

        myUSART0.receiveFrame(reinterpret_cast<uint8_t*>(l_receiverBuffer),BUFFER_SIZE);



        // wait for a key press
        while (myUSART0.getNumberBytesReceived()<4){}


        // reset number of bytes after extracting the received data
        myUSART0.resetNumberBytesReceived();

        // send back the received character
        if (myUSART0.ready2Send())
        {
            myUSART0.sendFrame(reinterpret_cast<uint8_t*>(l_receiverBuffer),BUFFER_SIZE);
        }

        while (!myUSART0.ready2Send()){}
        myTimerCounter2.setOutputCompareRegister(core::channel::A,myTimerCounter1.getOutputCompareRegister(core::channel::B));
        myTimerCounter1.setOutputCompareRegister(core::channel::B,myTimerCounter1.getOutputCompareRegister(core::channel::A));
        myTimerCounter1.setOutputCompareRegister(core::channel::A,getNumber(l_receiverBuffer));


    }
    return 0;
 }

 uint8_t getNumber(const char *ap_receive) {
  // Gets a numerical 0-255 from the serial port.
  // Converts from string to number.
  char hundreds = '0';
  char tens = '0';
  char ones = '0';
  char thisChar = '0';
  do {
    hundreds = tens;
    tens = ones;
    ones = thisChar;
    thisChar = *ap_receive;
    ap_receive++;

  } while (*ap_receive != '\r');
  return (100 * (hundreds - '0') + 10 * (tens - '0') + ones - '0');
 }
 *
 * Usage example (PWM on Any Pin):
 *
 * #include "TimerCounter0.h"
   #include "Led.h"

   #define LED_0 0
   #define LED_1 1
   #define LED_2 2
   #define LED_3 3

   // instantiate Timer0 object
   extern core::TimerCounter0 &myTimerCounter0;
   core::TimerCounter0 &myTimerCounter0 = core::TimerCounter0::getInstance();

   // instantiate Led objects
   extern component::Led Led0;
   component::Led Led0(io::Pin(LED_0,io::PortB));
   extern component::Led Led1;
   component::Led Led1(io::Pin(LED_1,io::PortB));
   extern component::Led Led2;
   component::Led Led2(io::Pin(LED_2,io::PortB));
   extern component::Led Led3;
   component::Led Led3(io::Pin(LED_3,io::PortB));

   #define DELAYTIME 3

   volatile uint8_t l_brightnessA;
   volatile uint8_t l_brightnessB;



   int main(void) {

    // Init
    uint8_t i;

    sei();

    myTimerCounter0.enableOutputCompareMatchInterrupt(core::channel::A,1);
    myTimerCounter0.enableOutputCompareMatchInterrupt(core::channel::B,1);
    myTimerCounter0.enableOverflowInterrupt(1);
    myTimerCounter0.start(core::clockSource::PS_1024);




    // ------ Event loop ------ //
    while (1) {

        for (i = 0; i < 255; i++) {
            _delay_ms(DELAYTIME);
            l_brightnessA = i;
            l_brightnessB = 255 - i;
        }
        for (i = 254; i > 0; i--) {
            _delay_ms(DELAYTIME);
            l_brightnessA = i;
            l_brightnessB = 255 - i;
        }



    }
    return 0;
    }


    void core::TimerCounter0::overflowServiceRoutine()
    {
        Led0.on();
        Led1.on();
        Led2.on();
        Led3.on();
        myTimerCounter0.setOutputCompareRegister(core::channel::A,l_brightnessA);
        myTimerCounter0.setOutputCompareRegister(core::channel::B,l_brightnessB);
    }

    void core::TimerCounter0::outputCompareMatchAServiceRoutine()
    {
        Led0.off();
        Led1.off();
        Led2.on();
        Led3.on();
    }
    void core::TimerCounter0::outputCompareMatchBServiceRoutine()
    {
        Led0.on();
        Led1.on();
        Led2.off();
        Led3.off();
    }
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/

#ifndef TIMER_COUNTER_H
#define TIMER_COUNTER_H
#include "ha_base.h"



namespace core
{

enum class channel : uint8_t {
    A=0,    /**<  */
    B,       /**<  */
};


enum class compareOutputMode : uint8_t {
    Normal=0,    /**<  */
    Toggle,       /**<  */
    Clear,   /**<  */
    Set,   /**<  */
};



enum class operationMode : uint8_t {
    Normal=0,    /**<  */
    PWM_PC,       /**<  */
    PWM_PC_8bit,   /**<  */
    PWM_PC_9bit,   /**<  */
    PWM_PC_10bit,   /**<  */
    PWM_PFC_ICR, /**<  */
    PWM_PFC_OCR, /**<  */
    PWM_PC_ICR, /**<  */
    PWM_PC_OCR, /**<  */
    Fast_PWM,   /**<  */
    Fast_PWM_8bit,   /**<  */
    Fast_PWM_9bit,   /**<  */
    Fast_PWM_10bit,   /**<  */
    Fast_PWM_ICR,   /**<  */
    Fast_PWM_OCR,   /**<  */
    CTC_OCR,   /**<  */
    CTC_ICR,   /**<  */
};
enum class clockSource {
     NoClock=0,
     PS_1,
     PS_8,
     PS_32,
     PS_64,
     PS_128,
     PS_256,
     PS_1024,
     Extern_Clock_T0_Falling_Edge,
     Extern_Clock_T0_Rising_Edge,
};

class TimerCounter
{

public:


    virtual void selectOperationMode(const operationMode &ar_operationMode) = 0;

    virtual void start(const clockSource &ar_clockSource) = 0;

    virtual void stop() = 0;

    virtual void selectCompareOutputMode(const channel &ar_channel, const compareOutputMode &ar_compareOutputMode) = 0;

    virtual void setCounter(const uint16_t &ar_dataBuffer) = 0;

    virtual uint16_t getCounter() const = 0;

    virtual void setOutputCompareRegister(const channel &ar_channel, uint16_t ar_dataBuffer) = 0;

    virtual uint16_t getOutputCompareRegister(const channel &ar_channel) const = 0;

    virtual void enableOutputCompareMatchInterrupt(const channel &ar_channel, const uint8_t a_enable) = 0;

    virtual void enableOverflowInterrupt(const uint8_t a_enable) = 0;


protected:

private:




};

}


#endif
