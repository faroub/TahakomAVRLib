# Shared build configuration for TahakomAVRLib applications.
# These defaults can be overridden by passing -D<NAME>=<VALUE> to CMake.

if(NOT MCU)
    set(MCU atmega328p CACHE STRING "AVR MCU")
endif()

if(NOT F_CPU)
    set(F_CPU 16000000UL CACHE STRING "CPU frequency (Hz)")
endif()

if(NOT BAUD)
    set(BAUD 9600 CACHE STRING "Baud rate")
endif()

if(NOT PROG_TYPE)
    set(PROG_TYPE arduino CACHE STRING "Programmer type (see 'avrdude -c ?')")
endif()

if(NOT AVRFLASH_PORT)
    if(UNIX)
        set(AVRFLASH_PORT /dev/ttyACM0 CACHE STRING "Flash port on Unix-like systems")
    elseif(WIN32)
        set(AVRFLASH_PORT COM3 CACHE STRING "Flash port on Windows")
    else()
        set(AVRFLASH_PORT "" CACHE STRING "Flash port")
    endif()
endif()
