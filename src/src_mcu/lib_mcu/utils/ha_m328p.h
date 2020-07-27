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

// PushButton defines
#define PUSHBUTTON_DEBOUNCE_TIME_US 1000 // microseconds to wait for next check
#define PUSHBUTTON_SAMPLING     12           // number of times a push button must be pressed


// USART defines
#include <util/setbaud.h>

#define   USART0_SET_BAUDRATE_HIGH_REGISTER     UBRR0H = UBRRH_VALUE
#define   USART0_SET_BAUDRATE_LOW_REGISTER      UBRR0L = UBRRL_VALUE

#define   USART0_ENABLE_ASYNC_TRANSMISSION_MODE     UCSR0C &= ~((1 << UMSEL01) | (1 << UMSEL00))
#define   USART0_ENABLE_SYNC_TRANSMISSION_MODE      UCSR0C = (UCSR0C & ~(1 << UMSEL01)) | (1 << UMSEL00)
#define   USART0_ENABLE_MASTER_SPI_MODE             UCSR0C |= (1 << UMSEL01) | (1 << UMSEL00)


#define   USART0_ENABLE_DOUBLE_SPEED_MODE
#define   USART0_DISABLE_DOUBLE_SPEED_MODE

#if USE_2X
    #undef  USART0_ENABLE_DOUBLE_SPEED_MODE
    #define USART0_ENABLE_DOUBLE_SPEED_MODE UCSR0A |= (1 << U2X0)
    #warning "double speed operation activated"
#else
    #undef  USART0_DISABLE_DOUBLE_SPEED_MODE
    #define USART0_DISABLE_DOUBLE_SPEED_MODE UCSR0A &= ~(1 << U2X0)
#endif

#define   USART0_ENABLE_EVEN_PARITY_MODE    UCSR0C = (1 << UPM01) | (UCSR0C & ~(1 << UPM00))
#define   USART0_ENABLE_ODD_PARITY_MODE     UCSR0C |=   ((1 << UPM01) | (1 << UPM00))
#define   USART0_DISABLE_PARITY_MODE        UCSR0C &= ~((1 << UPM01) | (1 << UPM00))


#define   USART0_SET_ONE_STOP_BIT   UCSR0C &= ~(1 << USBS0)
#define   USART0_SET_TWO_STOP_BITS  UCSR0C |= (1 << USBS0)


#define   USART0_SET_9BIT_FRAME_SIZE   \
    do { \
        UCSR0C |= ((1 << UCSZ01) | (1 << UCSZ00)); \
        UCSR0B |= (1 << UCSZ02); \
    } while (0)

#define   USART0_SET_8BIT_FRAME_SIZE   \
    do { \
        UCSR0C |= ((1 << UCSZ01) | (1 << UCSZ00)); \
        UCSR0B &= ~(1 << UCSZ02); \
    } while (0)

#define   USART0_SET_7BIT_FRAME_SIZE  \
    do { \
        UCSR0C = (1 << UCSZ01) | (UCSR0C & ~(1 << UCSZ00)); \
        UCSR0B &= ~(1 << UCSZ02); \
    } while (0)
#define   USART0_SET_6BIT_FRAME_SIZE \
    do { \
        UCSR0C = (UCSR0C & ~(1 << UCSZ01)) | (1 << UCSZ00); \
        UCSR0B &= ~(1 << UCSZ02); \
    } while (0)
#define   USART0_SET_5BIT_FRAME_SIZE \
    do { \
        UCSR0C &= ~((1 << UCSZ01) | (1 << UCSZ00)); \
        UCSR0B &= ~(1 << UCSZ02); \
    } while (0)


#define   USART0_ENABLE_TRANSMITTER      UCSR0B |=   1 << TXEN0
#define   USART0_DISABLE_TRANSMITTER     UCSR0B &= ~(1 << TXEN0)



#define   USART0_ENABLE_RECEIVER         UCSR0B |=   1 << RXEN0
#define   USART0_DISABLE_RECEIVER        UCSR0B &= ~(1 << RXEN0)



#define   USART0_CONTROL_STATUS_REGISTER UCSR0A
#define   USART0_DATA_REGISTER           UDR0




#define   USART0_FRAME_ERROR             FE0

#define   USART0_DATA_OVERRUN            DOR0

#define   USART0_PARITY_ERROR            UPE0


#define   USART0_ENABLE_DATA_REGISTER_EMPTY_INTERRUPT   UCSR0B |=   1 << UDRIE0
#define   USART0_DISABLE_DATA_REGISTER_EMPTY_INTERRUPT  UCSR0B &= ~(1 << UDRIE0)

#define   USART0_ENABLE_RECEIVE_COMPLETE_INTERRUPT   UCSR0B |=   1 << RXCIE0
#define   USART0_DISABLE_RECEIVE_COMPLETE_INTERRUPT  UCSR0B &= ~(1 << RXCIE0)

#define   USART0_ENABLE_TRANSMIT_COMPLETE_INTERRUPT   UCSR0B |=   1 << TXCIE0
#define   USART0_DISABLE_TRANSMIT_COMPLETE_INTERRUPT  UCSR0B &= ~(1 << TXCIE0)



#define   USART0_RECEIVE_COMPLETE_INTERRUPT     USART_RX_vect

#define   USART0_TRANSMIT_COMPLETE_INTERRUPT    USART_TX_vect

#define   USART0_DATA_REGISTER_EMPTY_INTERRUPT  USART_UDRE_vect


// ADC defines

#define   ADC_SELECT_REF_VOLTAGE(refVoltage)         ADMUX &= 0x3F; ADMUX |= refVoltage << 6

#define   ADC_ADJUST_RESULT_LEFT            ADMUX |=  1 << ADLAR
#define   ADC_ADJUST_RESULT_RIGHT           ADMUX &= ~(1 << ADLAR)

#define   ADC_SELECT_ANALOG_INPUT(pinNumber)         ADMUX &= 0xF0; ADMUX |= pinNumber
#define   ADC_DISABLE_DIGITAL_INPUT_REGISTER(pinNumber)         DIDR0 &= 0xC0; DIDR0 |= pinNumber


#define   ADC_ENABLE            ADCSRA |=  1 << ADEN
#define   ADC_DISABLE           ADCSRA &= ~(1 << ADEN)

#define   ADC_START_CONVERSION            ADCSRA |=  1 << ADSC
#define   ADC_STOP_CONVERSION           ADCSRA &= ~(1 << ADSC)

#define   ADC_ENABLE_AUTOTRIGGER          ADCSRA |=   1 << ADATE
#define   ADC_DISABLE_AUTOTRIGGER        ADCSRA &= ~(1 << ADATE)

#define   ADC_ENABLE_CONVERSION_COMPLETE_INTERRUPT   ADCSRA |=   1 << ADIE
#define   ADC_DISABLE_CONVERSION_COMPLETE_INTERRUPT  ADCSRA &= ~(1 << ADIE)

#define   ADC_SELECT_CLOCK_PRESCALER(clockPrescaler)         ADCSRA &= 0xF8; ADCSRA |= clockPrescaler


#define   ADC_SELECT_AUTO_TRIGGER_SOURCE(triggerSource)         ADCSRB &= 0xF8; ADCSRB |= triggerSource


#define   ADC_CONVERSION_COMPLETE_INTERRUPT  ADC_vect

// Externally Triggered Interrupts defines

#define EXT_INT_SET_INT0_SENSE_CONTROL(senseControl) EICRA &= 0xFC; EICRA |= senseControl
#define EXT_INT_SET_INT1_SENSE_CONTROL(senseControl) EICRA &= 0xF3; EICRA |= senseControl

#define EXT_INT_ENABLE_INT0   EIMSK |= 1 << INT0
#define EXT_INT_DISABLE_INT0   EIMSK &= ~(1 << INT0)

#define EXT_INT_ENABLE_INT1   EIMSK |= 1 << INT1
#define EXT_INT_DISABLE_INT1   EIMSK &= ~(1 << INT1)


#define EXT_INT_ENABLE_PIN_CHANGE_INTERRUPT(pinChangePort)    PCICR |= 1 << pinChangePort
#define EXT_INT_DISABLE_PIN_CHANGE_INTERRUPT(pinChangePort)   PCICR &= ~(1 << pinChangePort)


#define EXT_INT_ENABLE_PIN_CHANGE_MASK_PORTB(pinChangePin)    PCMSK0 |= 1 << pinChangePin
#define EXT_INT_DISABLE_PIN_CHANGE_MASK_PORTB(pinChangePin)   PCMSK0 &= ~(1 << pinChangePin)

#define EXT_INT_ENABLE_PIN_CHANGE_MASK_PORTC(pinChangePin)    PCMSK1 |= 1 << pinChangePin
#define EXT_INT_DISABLE_PIN_CHANGE_MASK_PORTC(pinChangePin)   PCMSK1 &= ~(1 << pinChangePin)

#define EXT_INT_ENABLE_PIN_CHANGE_MASK_PORTD(pinChangePin)    PCMSK2 |= 1 << pinChangePin
#define EXT_INT_DISABLE_PIN_CHANGE_MASK_PORTD(pinChangePin)   PCMSK2 &= ~(1 << pinChangePin)


#define  EXT_INT_INT0_INTERRUPT   INT0_vect
#define  EXT_INT_INT1_INTERRUPT   INT1_vect

#define  EXT_INT_PIN_CHANGE_PORTB_INTERRUPT   PCINT0_vect
#define  EXT_INT_PIN_CHANGE_PORTC_INTERRUPT   PCINT1_vect
#define  EXT_INT_PIN_CHANGE_PORTD_INTERRUPT   PCINT2_vect

// Timers/Counters defines

#define TIMER0_STOP TCCR0B &= 0xF8;
#define TIMER1_STOP TCCR1B &= 0xF8;
#define TIMER2_STOP TCCR2B &= 0xF8;

#define TIMER0_SELECT_CLOCK_SOURCE(clockSource) TCCR0B &= 0xF8; TCCR0B |= clockSource
#define TIMER1_SELECT_CLOCK_SOURCE(clockSource) TCCR1B &= 0xF8; TCCR1B |= clockSource
#define TIMER2_SELECT_CLOCK_SOURCE(clockSource) TCCR2B &= 0xF8; TCCR2B |= clockSource

#define TIMER0_SELECT_OPERATION_MODE(operationMode) TCCR0A &= 0xFC; TCCR0A |= (operationMode & 3); TCCR0B &= 0xF7; TCCR0B |= (operationMode & 12) << 3
#define TIMER1_SELECT_OPERATION_MODE(operationMode) TCCR1A &= 0xFC; TCCR1A |= (operationMode & 3); TCCR1B &= 0xE7; TCCR1B |= (operationMode & 12) << 3
#define TIMER2_SELECT_OPERATION_MODE(operationMode) TCCR2A &= 0xFC; TCCR2A |= (operationMode & 3); TCCR2B &= 0xF7; TCCR2B |= (operationMode & 12) << 3

#define TIMER0_SELECT_COM_CHANNEL_A(compareOutputMode) TCCR0A &= 0x3F; TCCR0A |= compareOutputMode << 6
#define TIMER0_SELECT_COM_CHANNEL_B(compareOutputMode) TCCR0A &= 0xCF; TCCR0A |= compareOutputMode << 4

#define TIMER1_SELECT_COM_CHANNEL_A(compareOutputMode) TCCR1A &= 0x3F; TCCR1A |= compareOutputMode << 6
#define TIMER1_SELECT_COM_CHANNEL_B(compareOutputMode) TCCR1A &= 0xCF; TCCR1A |= compareOutputMode << 4

#define TIMER2_SELECT_COM_CHANNEL_A(compareOutputMode) TCCR2A &= 0x3F; TCCR2A |= compareOutputMode << 6
#define TIMER2_SELECT_COM_CHANNEL_B(compareOutputMode) TCCR2A &= 0xCF; TCCR2A |= compareOutputMode << 4


#define TIMER0_RESET_COUNTER TCNT0 = 0x00
#define TIMER1_RESET_COUNTER TCNT1 = 0x00
#define TIMER2_RESET_COUNTER TCNT2 = 0x00

#endif

