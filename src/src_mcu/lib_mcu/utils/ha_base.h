/**
 * @file ha_base.h
 * @brief Header file for the basic hardware abstraction macros
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/

#ifndef HABASE_H
#define HABASE_H

// any class needs these includes
#include <stdint.h>
#include <avr/io.h>

/*
 * include hardware abstraction the Atmega328p
 */
#if   defined(__AVR_ATmega16__)

    #include "ha_m328p.h"

#endif


#endif
