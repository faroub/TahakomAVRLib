/**
 * @file main.cpp
 * @brief test mcu application
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/

#include "USART0.h"



int main(void) {

    // Init
    unsigned char l_data;

    io::USART0 &myUSART0 = io::USART0::getInstance();

    if (myUSART0.ready2Send())
    {
        myUSART0.sendString("Hello World!\r\n");
    }

    myUSART0.sendByte(100);
    // ------ Event loop ------ //
    while (1) {

        myUSART0.receiveChar(l_data);
        if (myUSART0.getNumberBytesReceived()==1)
        {
            if (myUSART0.ready2Send())
            {
                myUSART0.sendChar(l_data);
                myUSART0.resetNumberBytesReceived();
            }
        }


    }
    return 0;
}
















//#include "ADC.h"
//#include "USART0.h"




//#define TRANSMIT_BUFFER_SIZE 7



//int main(void) {

//   // Init


//   // instantiate the USART0 object
//   io::USART0 &myUSART0 = io::USART0::getInstance();
//   // transmit data buffer
//   char l_data2Send[TRANSMIT_BUFFER_SIZE];

//   // instantiate the ADC object
//   core::ADConverter &myADC = core::ADConverter::getInstance();


//   // select analog input
//   myADC.selectAnalogInput(io::Pin(0,io::PortC));

//   // variable to hold conversion result
//   uint16_t l_conversionResult = 0;

//   // enable and start conversion
//   myADC.start();

//   // ------ Event loop ------ //
//   while (1) {



//       myADC.getConversionResult(&l_conversionResult, core::resolution::RES_16bit);

//       if (myADC.conversionComplete())
//       {
//           l_data2Send[0] = '0' + (l_conversionResult / 10000);
//           l_data2Send[1] = '0' + ((l_conversionResult / 1000) % 10);
//           l_data2Send[2] = '0' + ((l_conversionResult / 100) % 10);
//           l_data2Send[3] = '0' + ((l_conversionResult / 10) % 10);
//           l_data2Send[4] = '0' + (l_conversionResult % 10);
//           l_data2Send[5] = '\n';
//           l_data2Send[6] = '\r';

//           if (myUSART0.ready2Send())
//           {
//               myUSART0.sendFrame(reinterpret_cast<uint8_t*>(l_data2Send),TRANSMIT_BUFFER_SIZE);
//           }

//       }
//   }
//   return 0;
//}




