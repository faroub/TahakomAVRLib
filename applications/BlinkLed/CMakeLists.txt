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


# Compilers Setting
set(CMAKE_SYSTEM_NAME  Generic)
set(CMAKE_CXX_COMPILER ${AVRCPP})
set(CMAKE_C_COMPILER   ${AVRC})
set(CMAKE_ASM_COMPILER   ${AVRC})

# Program Name
project (BlinkLed C CXX ASM)

# Set a convenient common name for all applications
set(MCU_APPLICATION APP_TEST)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler Flags
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


set(LIB_DIR "../../lib"
    CACHE PATH "The path to the mcu libraries.")


include_directories(${LIB_DIR}/io
                    ${LIB_DIR}/components
                    ${LIB_DIR}/core
                    ${LIB_DIR}/utils)

add_subdirectory(${LIB_DIR}/io ${CMAKE_BINARY_DIR}/io)
add_subdirectory(${LIB_DIR}/components ${CMAKE_BINARY_DIR}/components)


set (INTERN_LINK_LIBS ${INTERN_LINK_LIBS} components io)
set (EXTERN_LINK_LIBS ${EXTERN_LINK_LIBS})

# Target
add_executable(${MCU_APPLICATION} main.cpp)

target_link_libraries(${MCU_APPLICATION} ${INTERN_LINK_LIBS} ${EXTERN_LINK_LIBS})


set_target_properties(${MCU_APPLICATION} PROPERTIES OUTPUT_NAME "${MCU_APPLICATION}.elf")

# Compiling targets
add_custom_target(${MCU_APPLICATION}_strip ALL     ${AVRSTRIP} "${MCU_APPLICATION}.elf" DEPENDS ${MCU_APPLICATION})
add_custom_target(${MCU_APPLICATION}_hex   ALL     ${OBJCOPY} -R .eeprom -O ihex "${MCU_APPLICATION}.elf" "${MCU_APPLICATION}.hex" DEPENDS ${MCU_APPLICATION}_strip)
add_custom_target(${MCU_APPLICATION}_eeprom        ${OBJCOPY} -j .eeprom --change-section-lma .eeprom=0 -O ihex "${MCU_APPLICATION}.elf" "${MCU_APPLICATION}.eeprom" DEPENDS ${MCU_APPLICATION}_strip)
