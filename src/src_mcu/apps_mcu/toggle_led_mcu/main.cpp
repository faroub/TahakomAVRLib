/**
 * \brief Simple Application to blink a Led
 * \file main.cpp
 * \author Farid Oubbati (https://github.com/faroub)
 */

#include "avr/io.h"                    // adding header files
#include <util/delay.h>                // for _delay_ms()
#include "Pin.h"


#define LED      PB0
#define LED_DDR  DDRB
#define LED_PORT PORTB


#define DELAYTIME 1000

#define setBit(sfr, bit)     (_SFR_BYTE(sfr) |= (1 << bit))
#define clearBit(sfr, bit)   (_SFR_BYTE(sfr) &= ~(1 << bit))
#define toggleBit(sfr, bit)  (_SFR_BYTE(sfr) ^= (1 << bit))

int main(void) {

  // Init
  setBit(LED_DDR, LED);                      /* set LED pin for output */

  // Mainloop
  while (1) {

    setBit(LED_PORT, LED);
    _delay_ms(DELAYTIME);

    clearBit(LED_PORT, LED);
    _delay_ms(DELAYTIME);

  }
  return 0;                                          /* end mainloop */
}




