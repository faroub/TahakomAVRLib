/**
 * @file util/delay.h
 * @brief Host-side stub of the avr-libc busy-wait delay functions.
 *
 * The host has no real time to burn, so the delays are no-ops.
 */
#ifndef HOST_STUB_UTIL_DELAY_H
#define HOST_STUB_UTIL_DELAY_H

static inline void _delay_ms(double) {}
static inline void _delay_us(double) {}

#endif /* HOST_STUB_UTIL_DELAY_H */
