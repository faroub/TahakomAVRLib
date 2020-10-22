/**
 * @file main.cpp
 * @brief test mcu application
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/


#include "MCU.h"
#include "SPI.h"

#define SPI_SCK 5
#define SPI_MISO 4
#define SPI_MOSI 3
#define SPI_SS 2


int main(void) {

   // Init
   // initialize MCU
   core::MCU::init();
   // instantiate a SPI object
   io::SPI &myISP = io::SPI::getInstance(io::Pin(SPI_SCK,io::PortB),
                                         io::Pin(SPI_MISO,io::PortB),
                                         io::Pin(SPI_MOSI,io::PortB),
                                         io::Pin(SPI_SS,io::PortB));

   myISP.selectClockPrescaler(io::clockPrescaler::PS_128);
   myISP.selectDataMode(io::dataMode::mode_0);
   myISP.selectDataOrder(io::dataOrder::first_LSB);
   myISP.selectOperationMode(io::operationMode::master);

   myISP.masterSendByte(0x03);

   // Mainloop
   while (1) {



   }
   return 0;
}



