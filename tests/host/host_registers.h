/**
 * @file host_registers.h
 * @brief Device registers for host tests, as plain globals
 *
 * Include this file in a host test to provide the storage for the device
 * registers declared by the stub headers (stubs/avr/io.h).  Each test gets
 * its own set of zero-initialized registers.
 */
#ifndef HOST_REGISTERS_H
#define HOST_REGISTERS_H

#include <stdint.h>

// ATmega328P registers touched by the library, as plain globals
volatile uint8_t DDRB, PORTB, PINB;
volatile uint8_t DDRC, PORTC, PINC;
volatile uint8_t DDRD, PORTD, PIND;

volatile uint8_t UDR0, UCSR0A, UCSR0B, UCSR0C, UBRR0H, UBRR0L;

volatile uint8_t ADMUX, DIDR0, ADCSRA, ADCSRB;
volatile uint16_t ADC;

volatile uint8_t EICRA, EIMSK, PCICR, PCMSK0, PCMSK1, PCMSK2;

volatile uint8_t TCCR0A, TCCR0B, TCNT0, OCR0A, OCR0B, TIMSK0;
volatile uint8_t TCCR2A, TCCR2B, TCNT2, OCR2A, OCR2B, TIMSK2;
volatile uint8_t TCCR1A, TCCR1B, TIMSK1;
volatile uint16_t TCNT1, OCR1A, OCR1B, ICR1;

volatile uint8_t WDTCSR, MCUSR, SMCR, PRR, MCUCR;

volatile uint8_t SPCR, SPSR, SPDR;

volatile uint8_t TWBR, TWSR, TWCR, TWDR;

#endif /* HOST_REGISTERS_H */
