cmake_minimum_required(VERSION 2.8)

# Variables regarding the AVR chip
set(MCU   atmega328p)
set(F_CPU 16000000)
set(BAUD  9600)
add_definitions(-DF_CPU=${F_CPU} -DBAUD=${BAUD} -D__AVR_ATmega328P__)

set(AVRCPP   avr-g++)
set(AVRC     avr-gcc)
set(AVRSTRIP avr-strip)
set(OBJCOPY  avr-objcopy)


# Sets the compiler
# Needs to come before the project function
set(CMAKE_SYSTEM_NAME  Generic)
set(CMAKE_CXX_COMPILER ${AVRCPP})
set(CMAKE_C_COMPILER   ${AVRC})
set(CMAKE_ASM_COMPILER   ${AVRC})

# program name
project (AVR_MCU C CXX ASM)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags
set(CSTANDARD "-std=gnu99 ")
set(CDEBUG    "-gstabs -g -ggdb")
set(CWARN     "-Wall -Wstrict-prototypes -Wl,--gc-sections -Wl,--relax")
set(CTUNING   "-funsigned-char -funsigned-bitfields -fshort-enums -ffunction-sections -fdata-sections -fno-threadsafe-statics")
set(COPT      "-Os -lm -lprintf_flt")
set(CMCU      "-mmcu=${MCU}")
set(CDEFS     "-DF_CPU=${F_CPU} -DBAUD=${BAUD}")

set(CFLAGS   "${CMCU} ${CDEBUG} ${CDEFS} ${COPT} ${CWARN} ${CSTANDARD} ${CTUNING}")
set(CXXFLAGS "${CMCU} ${CDEBUG} ${CDEFS} ${COPT}  ${CTUNING}")

set(CMAKE_C_FLAGS   "${CFLAGS}")
set(CMAKE_CXX_FLAGS "${CXXFLAGS}")
set(CMAKE_ASM_FLAGS   "${CFLAGS}")


set(LIB_DIR "./src/lib_mcu"
    CACHE PATH "The path to the mcu libraries.")

include_directories(${LIB_DIR}/io
                    ${LIB_DIR}/components
                    ${LIB_DIR}/core
                    ${LIB_DIR}/utils)

add_subdirectory(${LIB_DIR}/io)
add_subdirectory(${LIB_DIR}/components)
add_subdirectory(${LIB_DIR}/core)
add_subdirectory(${LIB_DIR}/utils)


set(APPS_DIR "./src/apps_mcu"
    CACHE PATH "The path to the mcu apps.")

#add_subdirectory(${APPS_DIR}/blink_led_mcu)
add_subdirectory(${APPS_DIR}/app_test)
