/**
 * @file ha_base.h
 * @brief Base header file for the basic hardware abstraction macros
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/

#ifndef HABASE_H
#define HABASE_H

/*
 *  Helper construct to get interrupt numbers from names
 */
#define STRx(s)  #s
#define STR(s)   STRx(s)

/*
 *  Any class needs these includes from avr-libc
 */
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>


/*
 * Include hardware abstraction for supported MCUs
 */
#if defined(__AVR_ATmega328P__)

    #include "ha_m328p.h"
    #include "utils_m328p.h"

#elif defined(__AVR_ATmega2560__)

    #include "ha_m2560.h"
    #include "utils_m328p.h"

#elif defined(__AVR_ATmega168P__) || defined(__AVR_ATmega88P__) || defined(__AVR_ATmega48P__)

    #include "ha_m168p.h"
    #include "utils_m328p.h"

#elif defined(__AVR_ATtiny85__)

    #include "ha_attiny85.h"
    #include "utils_m328p.h"

#elif defined(ESP32) || defined(ESP_PLATFORM)

    #include "ha_esp32.h"
    #include "utils_m328p.h"

#else

    #error "TahakomAVRLib: unsupported MCU"

#endif


#endif
