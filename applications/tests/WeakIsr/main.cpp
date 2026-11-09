/**
 * @file applications/tests/WeakIsr/main.cpp
 * @brief Link check: every library interrupt enabled with no app handlers
 *
 * The library provides weak default handlers (empty bodies) for the
 * timer 0/1/2, external interrupt and watchdog vectors.  This app arms
 * every single one of those interrupts without defining a handler, so
 * its link proves the weak defaults cover all vectors; the resulting
 * map can be checked with avr-objdump to confirm each vector points at
 * a weak default instead of __bad_interrupt.
 */

#include "MCU.h"
#include "TimerCounter0.h"
#include "TimerCounter1.h"
#include "TimerCounter2.h"
#include "ExternInterrupt.h"
#include "WatchdogTimer.h"

int main(void)
{
    core::TimerCounter0 &timer0 = core::TimerCounter0::getInstance();
    timer0.enableOutputCompareMatchInterrupt(core::channel::A, 1);
    timer0.enableOutputCompareMatchInterrupt(core::channel::B, 1);
    timer0.enableOverflowInterrupt(1);

    core::TimerCounter1 &timer1 = core::TimerCounter1::getInstance();
    timer1.enableOutputCompareMatchInterrupt(core::channel::A, 1);
    timer1.enableOutputCompareMatchInterrupt(core::channel::B, 1);
    timer1.enableOverflowInterrupt(1);
    timer1.enableInputCaptureInterrupt(1);

    core::TimerCounter2 &timer2 = core::TimerCounter2::getInstance();
    timer2.enableOutputCompareMatchInterrupt(core::channel::A, 1);
    timer2.enableOutputCompareMatchInterrupt(core::channel::B, 1);
    timer2.enableOverflowInterrupt(1);

    core::ExternInterrupt &extint = core::ExternInterrupt::getInstance();
    extint.enableInt0(1);
    extint.enableInt1(1);
    extint.enablePinChange(core::pinChangePort::PCINTB, 1);
    extint.enablePinChange(core::pinChangePort::PCINTC, 1);
    extint.enablePinChange(core::pinChangePort::PCINTD, 1);

    core::WatchdogTimer &watchdog = core::WatchdogTimer::getInstance();
    watchdog.start(core::watchdogOperationMode::interrupt, core::timeOut::to_2s);

    core::MCU::enableInterrupts();

    while (1)
    {
    }

    return 0;
}
