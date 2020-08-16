/**
 * @file main.cpp
 * @brief test mcu application
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/


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
    uint8_t l_brightness=0;
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

//            l_brightness++;
//            //_delay_ms(500);



//        OCR2A = OCR1B;
//        OCR1B = OCR1A;
//        OCR1A = l_brightness;

    }                                                  /* End event loop */
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













//#include "Buzzer.h"
//#include "TimerCounter0.h"
//#include "buzzer_pitches_8bit.h"


//#define BUZZER 6


//int main(void) {

//    // Init
//    // instantiate the Buzzer object
//    component::Buzzer Buzzer(io::Pin(BUZZER,io::PortD));


//    // instantiate the TimerCounter0 object
//    core::TimerCounter0 &myTimerCounter0 = core::TimerCounter0::getInstance();



//    // notes in the melody:
//    const uint16_t notes[] = { C2, E2, G2, C3};

//    myTimerCounter0.selectOperationMode(core::operationMode::CTC_OCR);
//    myTimerCounter0.selectCompareOutputMode(core::channel::A,core::compareOutputMode::Toggle);
//    myTimerCounter0.start(core::clockSource::PS_1024);

////    for (uint8_t i = 0; i < sizeof (notes)/sizeof (uint16_t); i++)
////    {
//        myTimerCounter0.setCounter(0);
//        myTimerCounter0.setOutputCompareRegister(core::channel::A,notes[1]);
//        _delay_ms(1000);
//        myTimerCounter0.stop();
//        Buzzer.noBuzz();
//        _delay_ms(3000);

////    }

////    for (uint8_t i = 0; i < sizeof (notes)/sizeof (uint16_t); i++)
////    {

////        Buzzer.buzz(notes[i],200);
////        Buzzer.noBuzz();
////        // delay between pitches
////        _delay_ms(200);

////    }

////    _delay_ms(1000);
////    _delay_ms(1000);
////    _delay_ms(1000);

////    for (uint8_t i = 0; i < sizeof (notes)/sizeof (uint16_t); i++)
////    {
////        Buzzer.buzz(myTimerCounter0,notes[i],250);
////        Buzzer.noBuzz(myTimerCounter0);
////        Buzzer.noBuzz();
////        // delay between pitches
////        //_delay_ms(200);


////    }


//    // ------ Event loop ------ //
//    while (1) {
//                                                  /* Get Key */

//    }                                                  /* End event loop */
//    return 0;
//}


