/**
 * @file ha_base.h
 * @brief Base header file for the basic hardware abstraction macros
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/

#ifndef HABASE_H
#define HABASE_H

// helper construct to get interrupt numbers from names
#define STRx(s)  #s
#define STR(s)   STRx(s)

// any class needs these includes
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>


/*
 * include hardware abstraction the Atmega328p
 */
#if   defined(__AVR_ATmega328P__)

    #include "ha_m328p.h"
    #include "utils_m328p.h"

#endif


#endif
