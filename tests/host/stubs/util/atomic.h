/**
 * @file util/atomic.h
 * @brief Host-side stub of the avr-libc atomic block helpers.
 *
 * Mirrors the avr-libc ATOMIC_BLOCK semantics: interrupts are disabled on
 * entry and the state saved before the block is restored on exit
 * (ATOMIC_RESTORESTATE).  The interrupt enable/disable are the host stubs.
 */
#ifndef HOST_STUB_UTIL_ATOMIC_H
#define HOST_STUB_UTIL_ATOMIC_H

#include <stdint.h>
#include <avr/interrupt.h>

typedef uint8_t __atomic_save_t;

static inline __atomic_save_t __iCliRetVal(void)
{
    cli();
    return 1;
}

static inline void __iRestore(__atomic_save_t s)
{
    if (s)
    {
        sei();
    }
}

#define ATOMIC_FORCEON __atomic_save_t
#define ATOMIC_RESTORESTATE __atomic_save_t

#define ATOMIC_BLOCK(t) \
    for (t __atomic_tmp = __iCliRetVal(), __ToDo = 1; \
         __ToDo; \
         __ToDo = 0, __iRestore(__atomic_tmp))

#endif /* HOST_STUB_UTIL_ATOMIC_H */
