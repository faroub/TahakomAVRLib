/**
 * @file ha_esp32.h
 * @brief Header file for the hardware abstraction macros of ESP32 / ESP32-S3
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2026
*/
#ifndef HA_ESP32_H
#define HA_ESP32_H

#ifndef HABASE_H
    #error "don't use this file directly! Please include only ha_base.h!"
#endif

// PushButton defines
#define PUSHBUTTON_DEBOUNCE_TIME_US 1000

// USART / Serial defines for ESP32
#define   USART0_SET_BAUDRATE_HIGH_REGISTER
#define   USART0_SET_BAUDRATE_LOW_REGISTER

#define   USART0_ENABLE_ASYNC_TRANSMISSION_MODE
#define   USART0_ENABLE_SYNC_TRANSMISSION_MODE
#define   USART0_ENABLE_MASTER_SPI_MODE

#define   USART0_ENABLE_DOUBLE_SPEED_MODE
#define   USART0_DISABLE_DOUBLE_SPEED_MODE

#define   USART0_ENABLE_EVEN_PARITY_MODE
#define   USART0_ENABLE_ODD_PARITY_MODE
#define   USART0_DISABLE_PARITY_MODE

#define   USART0_SET_ONE_STOP_BIT
#define   USART0_SET_TWO_STOP_BITS

#define   USART0_SET_9BIT_FRAME_SIZE   do {} while (0)
#define   USART0_SET_8BIT_FRAME_SIZE   do {} while (0)
#define   USART0_SET_7BIT_FRAME_SIZE  do {} while (0)
#define   USART0_SET_6BIT_FRAME_SIZE do {} while (0)
#define   USART0_SET_5BIT_FRAME_SIZE do {} while (0)

#define   USART0_ENABLE_TRANSMITTER      do {} while (0)
#define   USART0_DISABLE_TRANSMITTER     do {} while (0)

#define   USART0_ENABLE_RECEIVER         do {} while (0)
#define   USART0_DISABLE_RECEIVER        do {} while (0)

#define   USART0_CONTROL_STATUS_REGISTER 0
#define   USART0_DATA_REGISTER           0

#define   USART0_FRAME_ERROR             0
#define   USART0_DATA_OVERRUN            0
#define   USART0_PARITY_ERROR            0

#define   USART0_ENABLE_DATA_REGISTER_EMPTY_INTERRUPT   do {} while (0)
#define   USART0_DISABLE_DATA_REGISTER_EMPTY_INTERRUPT  do {} while (0)

#define   USART0_ENABLE_RECEIVE_COMPLETE_INTERRUPT   do {} while (0)
#define   USART0_DISABLE_RECEIVE_COMPLETE_INTERRUPT  do {} while (0)

#define   USART0_ENABLE_TRANSMIT_COMPLETE_INTERRUPT   do {} while (0)
#define   USART0_DISABLE_TRANSMIT_COMPLETE_INTERRUPT  do {} while (0)

#define   USART0_RECEIVE_COMPLETE_INTERRUPT     0
#define   USART0_TRANSMIT_COMPLETE_INTERRUPT    0
#define   USART0_DATA_REGISTER_EMPTY_INTERRUPT  0

// ADC defines
#define   ADC_SELECT_REF_VOLTAGE(refVoltage)            do {} while (0)
#define   ADC_ADJUST_RESULT_LEFT                        do {} while (0)
#define   ADC_ADJUST_RESULT_RIGHT                       do {} while (0)
#define   ADC_SELECT_ANALOG_INPUT(pinNumber)            do {} while (0)
#define   ADC_DISABLE_DIGITAL_INPUT_REGISTER(pinNumber) do {} while (0)

#define   ADC_ENABLE                                    do {} while (0)
#define   ADC_DISABLE                                   do {} while (0)

#define   ADC_START_CONVERSION                          do {} while (0)
#define   ADC_STOP_CONVERSION                           do {} while (0)

#define   ADC_ENABLE_AUTOTRIGGER                        do {} while (0)
#define   ADC_DISABLE_AUTOTRIGGER                      do {} while (0)

#define   ADC_ENABLE_CONVERSION_COMPLETE_INTERRUPT     do {} while (0)
#define   ADC_DISABLE_CONVERSION_COMPLETE_INTERRUPT    do {} while (0)

#define   ADC_SELECT_CLOCK_PRESCALER(clockPrescaler)   do {} while (0)
#define   ADC_SELECT_AUTO_TRIGGER_SOURCE(triggerSource) do {} while (0)

#define   ADC_CONVERSION_COMPLETE_INTERRUPT            0

// Watchdog Timer defines
#define WATCHDOG_SELECT_TIMEOUT(timeOut)               do {} while (0)
#define WATCHDOG_START(operationMode,timeOut)          do {} while (0)
#define WATCHDOG_STOP                                  do {} while (0)
#define WATCHDOG_TIMEOUT_INTERRUPT                    0

// MCU defines
#define MCU_SELECT_SLEEP_MODE(sleepMode)               do {} while (0)

#define MCU_SLEEP_ENABLE                               do {} while (0)
#define MCU_SLEEP_DISABLE                              do {} while (0)

#define MCU_TWI_ENABLE                                 do {} while (0)
#define MCU_TWI_DISABLE                                do {} while (0)

#define MCU_TIMER2_ENABLE                              do {} while (0)
#define MCU_TIMER2_DISABLE                             do {} while (0)

#define MCU_TIMER1_ENABLE                              do {} while (0)
#define MCU_TIMER1_DISABLE                             do {} while (0)

#define MCU_TIMER0_ENABLE                              do {} while (0)
#define MCU_TIMER0_DISABLE                             do {} while (0)

#define MCU_SPI_ENABLE                                 do {} while (0)
#define MCU_SPI_DISABLE                                do {} while (0)

#define MCU_USART0_ENABLE                              do {} while (0)
#define MCU_USART0_DISABLE                             do {} while (0)

#define MCU_ADC_ENABLE                                 do {} while (0)
#define MCU_ADC_DISABLE                                do {} while (0)

#endif
