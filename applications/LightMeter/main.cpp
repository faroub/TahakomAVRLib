/**
 * @file main.cpp
 *
 * Demo example: Light Meter
 *
 * Usage demonstration of the TahakomAVRLib in an AVR based Light Meter.
 * The example presents an AVR based Light Meter that displays its Lux measurements in a Led bank.
 * This example demonstrates the use of the component::LED(), component::LDR() and core::ADC() abstraction objects.
 *
 * - Compiler: avr-gcc (GCC) 5.4.0
 * - Supported devices:  The example compiles on the ATmega48P/88P/168P/328P AVR family
 *
 *
 * @author Farid Oubbati (farid.oubbati@outlook.com)
 * @date March 2018
*/

#include "Led.h"
#include "ADC.h"
#include "LDR.h"

#define LED0 0 /**< Led 0 */
#define LED1 1 /**< Led 1 */
#define LED2 2 /**< Led 2 */
#define LED3 3 /**< Led 3 */
#define LED4 4 /**< Led 4 */
#define LED5 5 /**< Led 5 */
#define LED6 6 /**< Led 6 */
#define LED7 7 /**< Led 7 */

#define LDRVD 0 /**< LDR */


int main(void) {

  // Init

  // Instantiate Led objects
  component::Led Led[8]={
        {io::Pin(LED0,io::PortD)},
        {io::Pin(LED1,io::PortD)},
        {io::Pin(LED2,io::PortD)},
        {io::Pin(LED3,io::PortD)},
        {io::Pin(LED4,io::PortD)},
        {io::Pin(LED5,io::PortD)},
        {io::Pin(LED6,io::PortD)},
        {io::Pin(LED7,io::PortD)}};


  // Instantiate ADC object
  core::ADConverter &myADC = core::ADConverter::getInstance();

  // Start conversion
  myADC.start();

  // Instantiate LDR object
  component::LDR LDR(io::Pin(LDRVD,io::PortC),myADC);


  // Mainloop
  while (1) {


    for (uint8_t i=0;i<=(7-(LDR.getLux(5) & 7));i++)
    {
        Led[i].on();
    }

    for (uint8_t i=(7-(LDR.getLux(5) & 7))+1;i<8;i++)
    {
        Led[i].off();
    }

  }
  return 0;
}
