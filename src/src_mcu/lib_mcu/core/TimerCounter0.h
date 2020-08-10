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
 * Usage example (Reaction Timer):
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/

#ifndef TIMER_COUNTER0_H
#define TIMER_COUNTER0_H
#include "TimerCounter.h"
#include "ha_base.h"



namespace core
{


class TimerCounter0 : public TimerCounter
{

public:

    static TimerCounter0& getInstance(const channel &ar_channel = channel::A,
                                      const operationMode &ar_operationMode = operationMode::Normal,
                                      const clockSource &ar_clockSource= clockSource::NoClock,
                                      const compareOutputMode& ar_compareOutputMode = compareOutputMode::Normal);

    void selectOperationMode(const operationMode &ar_operationMode) override;

    void start(const clockSource &ar_clockSource) override;

    void stop() override;

    void selectCompareOutputMode(const channel &ar_channel, const compareOutputMode &ar_compareOutputMode) override;

    void setCounter(const uint16_t &ar_dataBuffer) override;

    uint16_t getCounter() const override;

    void setOutputCompareRegister(const channel &ar_channel, const uint16_t &ar_dataBuffer) override;

    uint16_t getOutputCompareRegister(const channel &ar_channel) const override;


protected:

private:

    TimerCounter0(const channel &ar_channel,
                  const operationMode &ar_operationMode,
                  const clockSource &ar_clockSource,
                  const compareOutputMode& ar_compareOutputMode);

    ~TimerCounter0();

    /** Overried Copy constructor.
        */
    TimerCounter0(const TimerCounter0&);

    /** Override assign operator.
        */
    const TimerCounter0& operator=(const TimerCounter0&);



};

}


#endif
