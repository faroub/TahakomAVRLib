#include "MCU.h"
#include <avr/sleep.h>
#include <util/atomic.h>

void core::MCU::enableInterrupts()
{
    sei();

}

void core::MCU::init()
{
    enableUSART0(0);
    enableTimerCounter0(0);
    enableTimerCounter1(0);
    enableTimerCounter2(0);
    enableTWI(0);
    enableSPI(0);
    enableADC(0);

}

void core::MCU::selectSleepMode(const sleepMode &a_sleepMode)
{
    MCU_SELECT_SLEEP_MODE(static_cast<uint8_t>(a_sleepMode));

}

void core::MCU::goToSleep(const BODMode &a_BODMode)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        switch (a_BODMode)
        {
            case core::BODMode::enabled:
            {
                sleepEnable(1);
                // the sleep instruction must run with interrupts enabled so
                // that the chip can wake up again
                sei();
                sleep_cpu();
                sleepEnable(0);
                break;
            }
            case core::BODMode::disabled:
            {
                sleepEnable(1);
                // the sleep instruction must run with interrupts enabled so
                // that the chip can wake up again
                sei();
                // the timed BOD disable sequence must immediately precede
                // sleep_cpu(), an interrupt in between would break it
                sleep_bod_disable();
                sleep_cpu();
                sleepEnable(0);
                break;
            }
        }
    }

}

void core::MCU::sleepEnable(const uint8_t a_enable)
{
    if (a_enable) {
        MCU_SLEEP_ENABLE;
    } else {
        MCU_SLEEP_DISABLE;
    }

}

void core::MCU::enableUSART0(const uint8_t a_enable)
{
    if (a_enable) {
        MCU_USART0_ENABLE;
    } else {
        MCU_USART0_DISABLE;
    }

}

void core::MCU::enableTimerCounter0(const uint8_t a_enable)
{
    if (a_enable) {
        MCU_TIMER0_ENABLE;
    } else {
        MCU_TIMER0_DISABLE;
    }

}

void core::MCU::enableTimerCounter1(const uint8_t a_enable)
{
    if (a_enable) {
        MCU_TIMER1_ENABLE;
    } else {
        MCU_TIMER1_DISABLE;
    }

}

void core::MCU::enableTimerCounter2(const uint8_t a_enable)
{
    if (a_enable) {
        MCU_TIMER2_ENABLE;
    } else {
        MCU_TIMER2_DISABLE;
    }

}

void core::MCU::enableTWI(const uint8_t a_enable)
{
    if (a_enable) {
        MCU_TWI_ENABLE;
    } else {
        MCU_TWI_DISABLE;
    }

}

void core::MCU::enableSPI(const uint8_t a_enable)
{
    if (a_enable) {
        MCU_SPI_ENABLE;
    } else {
        MCU_SPI_DISABLE;
    }

}

void core::MCU::enableADC(const uint8_t a_enable)
{
    if (a_enable) {
        MCU_ADC_ENABLE;
    } else {
        MCU_ADC_DISABLE;
    }
}
