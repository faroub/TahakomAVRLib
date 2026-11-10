/**
 * @file ha_attiny85.h
 * @brief Header file for the hardware abstraction macros of ATtiny85
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2026
*/
#ifndef HA_ATTINY85_H
#define HA_ATTINY85_H

#ifndef HABASE_H
    #error "don't use this file directly! Please include only ha_base.h!"
#endif

// PushButton defines
#define PUSHBUTTON_DEBOUNCE_TIME_US 1000

// ADC defines
#define   ADC_SELECT_REF_VOLTAGE(refVoltage) \
    do { \
        ADMUX &= 0x3F; \
        ADMUX |= (refVoltage) << 6; \
    } while (0)

#define   ADC_ADJUST_RESULT_LEFT            ADMUX |=  1 << ADLAR
#define   ADC_ADJUST_RESULT_RIGHT           ADMUX &= ~(1 << ADLAR)

#define   ADC_SELECT_ANALOG_INPUT(pinNumber) \
    do { \
        ADMUX &= 0xF0; \
        ADMUX |= ((pinNumber) & 0x0F); \
    } while (0)

#define   ADC_DISABLE_DIGITAL_INPUT_REGISTER(pinNumber) \
    do { \
        DIDR0 |= (1 << (pinNumber)); \
    } while (0)

#define   ADC_ENABLE            ADCSRA |=  1 << ADEN
#define   ADC_DISABLE           ADCSRA &= ~(1 << ADEN)

#define   ADC_START_CONVERSION            ADCSRA |=  1 << ADSC
#define   ADC_STOP_CONVERSION           ADCSRA &= ~(1 << ADSC)

#define   ADC_ENABLE_AUTOTRIGGER          ADCSRA |=   1 << ADATE
#define   ADC_DISABLE_AUTOTRIGGER        ADCSRA &= ~(1 << ADATE)

#define   ADC_ENABLE_CONVERSION_COMPLETE_INTERRUPT   ADCSRA |=   1 << ADIE
#define   ADC_DISABLE_CONVERSION_COMPLETE_INTERRUPT  ADCSRA &= ~(1 << ADIE)

#define   ADC_SELECT_CLOCK_PRESCALER(clockPrescaler) \
    do { \
        ADCSRA &= 0xF8; \
        ADCSRA |= (clockPrescaler); \
    } while (0)

#define   ADC_CONVERSION_COMPLETE_INTERRUPT  ADC_vect

// Watchdog Timer defines
#define WATCHDOG_SELECT_TIMEOUT(timeOut) \
    do { \
        WDTCR |= (1<<WDCE) | (1<<WDE); \
        WDTCR = (0<<WDIE) | (0<<WDE) | (timeOut); \
    } while (0)
#define WATCHDOG_START(operationMode,timeOut) \
    do { \
        WDTCR |= (1<<WDCE) | (1<<WDE); \
        WDTCR = (operationMode) | (timeOut); \
    } while (0)
#define WATCHDOG_STOP \
    do { \
        WDTCR |= (1<<WDCE) | (1<<WDE); \
        WDTCR = 0x00; \
    } while (0)
#define WATCHDOG_TIMEOUT_INTERRUPT  WDT_vect

// MCU defines
#define MCU_SELECT_SLEEP_MODE(sleepMode) \
    do { \
        MCUCR &= 0xCF; \
        MCUCR |= (sleepMode) << 4; \
    } while (0)

#define MCU_SLEEP_ENABLE MCUCR |=  1 << SE
#define MCU_SLEEP_DISABLE MCUCR &= ~(1 << SE)

#endif
