/**
 * @file ha_m328p.h
 * @brief Header file for the hardware abstraction macros of the Atmega328p
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/
#ifndef HAM329P_H
#define HAM329P_H

#ifndef HABASE_H
    #error "don't use this file directly! Please include only ha_base.h!"
#endif

#if defined(__AVR_ATmega328P__)


# define   USART0_CALC_UBRR(br)           ((F_CPU+br*8)/(br*16)-1)


#ifndef F_CPU
    #error "system clock frequency F_CPU must be defined"
#endif

#ifndef BAUD
    #error "baud rate BAUD must be defined"
#endif

// compute baud rate
#define UBRR_VAL ((F_CPU+BAUD*8)/(BAUD*16)-1)   // clever rounding
#define BAUD_REAL (F_CPU/(16*(UBRR_VAL+1)))     // Real baud rate
#define BAUD_ERROR ((BAUD_REAL*1000)/BAUD) // error in per thousand, 1000 = no error.

#if ((BAUD_ERROR<990) || (BAUD_ERROR>1010))
    #error "too high systematic baud rate error, greater than 1%!"
#endif

# define   USART0_SET_BAUDRATE_HIGH_REGISTER  UBRR0H = UBRR_VAL >> 8;
# define   USART0_SET_BAUDRATE_LOW_REGISTER   UBRR0L = UBRR_VAL & 0xFF;
# define   USART0_DATA_REGISTER           UDR0

# define   USART0_SET_8B1SNP              UCSRC |= 1 << URSEL | 1 << UCSZ1 | 1 << UCSZ0

# define   USART0_ENABLE_SENDER           UCSR0B |=   1 << TXEN0
# define   USART0_DISABLE_SENDER          UCSR0B &= ~(1 << TXEN0)
# define   USART0_ENABLE_SENDER_INTERRUPT   UCSR0B |=   1 << UDRIE0
# define   USART0_DISABLE_SENDER_INTERRUPT  UCSR0B &= ~(1 << UDRIE0)

# define   USART0_ENABLE_RECEIVER         UCSR0B |=   1 << RXEN0
# define   USART0_DISABLE_RECEIVER        UCSR0B &= ~(1 << RXEN0)

# define   USART0_ENABLE_RECEIVER_INTERRUPT   UCSR0B |=   1 << RXCIE0
# define   USART0_DISABLE_RECEIVER_INTERRUPT  UCSR0B &= ~(1 << RXCIE0)

# define   USART0_FRAME_ERROR             FE

# define   USART0_DATA_OVERRUN            DOR

# define   USART0_PARITY_ERROR            PE

# define   USART0_RECEIVE_INTERRUPT       USART_RXC_vect

# define   USART0_SENDER_INTERRUPT        USART_UDRE_vect

#endif
#endif
