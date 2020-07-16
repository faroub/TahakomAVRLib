/**
 * @file main.cpp
 * @brief test mcu application
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/


#include "ADC.h"
#include "USART0.h"

#define TRANSMIT_BUFFER_SIZE 5
#define ADC_CHANNEL_MUX_SIZE 2


int main(void) {

  // Init

  // instantiate the USART0 object
  io::USART0 &myUSART0 = io::USART0::getInstance();
  // transmit data buffer
  char l_data2Send[TRANSMIT_BUFFER_SIZE];
  // ready to send flag
  uint8_t l_ready2Send = 0;

  // instantiate the ADC object
  core::ADConverter &myADC = core::ADConverter::getInstance();
  // use 8 bit resolution
  myADC.adjustResult(core::resultAdjust::left);
  // flag to test multiplexing
  uint8_t channel = 0;
  // select analog input
  myADC.selectAnalogInput(io::Pin(channel,io::PortC));

  // variable to hold conversion result
  uint16_t l_conversionResult = 0;
  uint16_t l_analogInput[ADC_CHANNEL_MUX_SIZE] = {0,0};

  // enable and start conversion
  myADC.start();

  // Mainloop
  while (1) {

      if (l_ready2Send && !myUSART0.getNumberBytesSent())
      {
          myUSART0.sendFrame(reinterpret_cast<uint8_t*>(l_data2Send),TRANSMIT_BUFFER_SIZE);
          l_ready2Send = 0;
      }

      myADC.getConversionResult(&l_conversionResult, ADC_CHANNEL_MUX_SIZE);

      if (myADC.conversionComplete())
      {

          l_analogInput[channel] = l_conversionResult;
          l_data2Send[0] = '0' + ((l_analogInput[channel] >> 8) / 100);
          l_data2Send[1] = '0' + (((l_analogInput[channel] >> 8) / 10) % 10);
          l_data2Send[2] = '0' + ((l_analogInput[channel] >> 8) % 10);
          l_data2Send[3] = '\n';
          l_data2Send[4] = '\r';
          // switch between channel 0 and channel 1
          channel = 1 - channel;
          myADC.selectAnalogInput(io::Pin(channel,io::PortC));

          l_ready2Send = 1;
      }


//          uint16_t value = 12345;
//          char lo = value & 0xFF;
//          char hi = value >> 8;
  }
  return 0;
}




