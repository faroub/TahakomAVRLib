cmake_minimum_required(VERSION 3.14)

# Set AVR GCC toolchain
set(AVRCPP   avr-g++)
set(AVRC     avr-gcc)
set(AVRSTRIP avr-strip)
set(OBJCOPY  avr-objcopy)
set(OBJDUMP avr-objdump)
set(AVRSIZE avr-size)

# The programmer to use, ask avrdude ($ avrdude -c ?) for a list
set(PROG_TYPE arduino)

# Set flash software tool
set(AVRFLASH_TOOL avrdude)

# Set flash port, check the port name in your system
if (UNIX)
    set(AVRFLASH_PORT /dev/ttyACM0)
elseif (WIN32)
    set(AVRFLASH_PORT COM3)
endif ()


# Set AVR chip configuration
set(MCU   atmega328p)
set(F_CPU 16000000UL)
set(BAUD  9600)

# Pass defines to the compiler
# __AVR_ATmega328P__ definition is only necessary when using an IDE
add_definitions(-DF_CPU=${F_CPU} -DBAUD=${BAUD} -D__AVR_ATmega328P__)

# Use AVR GCC toolchain
set(CMAKE_SYSTEM_NAME  Generic)
set(CMAKE_CXX_COMPILER ${AVRCPP})
set(CMAKE_C_COMPILER   ${AVRC})
set(CMAKE_ASM_COMPILER   ${AVRC})

# Program name
project (BlinkLed C CXX ASM)

# Set a convenient common name for all applications
set(FIRMWARE APP)

# Use C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set compiler flags
set(CSTANDARD "-std=gnu99 ")
set(CDEBUG    "-gstabs -g -ggdb")
set(CWARN     "-Wall -Wstrict-prototypes -Wl,--gc-sections -Wl,--relax")
set(CTUNING   "-funsigned-char -funsigned-bitfields -fshort-enums -ffunction-sections -fdata-sections -fno-threadsafe-statics")
set(COPT      "-Os -lm -lprintf_flt")
set(CMCU      "-mmcu=${MCU}")
set(CDEFS     "-DF_CPU=${F_CPU} -DBAUD=${BAUD}")
set(CFLAGS   "${CMCU} ${CDEBUG} ${CDEFS} ${COPT} ${CWARN} ${CSTANDARD} ${CTUNING}")
set(CXXFLAGS "${CMCU} ${CDEBUG} ${CDEFS} ${COPT}  ${CTUNING}")

# Use compiler flags
set(CMAKE_C_FLAGS   "${CFLAGS}")
set(CMAKE_CXX_FLAGS "${CXXFLAGS}")
set(CMAKE_ASM_FLAGS   "${CFLAGS}")

# Set MCU library path
set(LIB_DIR "../../lib"
    CACHE PATH "The path to the MCU libraries.")

# Add MCU library to the compiler include directories
include_directories("${LIB_DIR}/io"
                    "${LIB_DIR}/components"
                    "${LIB_DIR}/core"
                    "${LIB_DIR}/utils")

# Add subdirectories to the build
add_subdirectory("${LIB_DIR}/io" "${CMAKE_BINARY_DIR}/io")
add_subdirectory("${LIB_DIR}/components" "${CMAKE_BINARY_DIR}/components")

# Set the internal and external libs
set (INTERN_LINK_LIBS ${INTERN_LINK_LIBS} components io)
set (EXTERN_LINK_LIBS ${EXTERN_LINK_LIBS})

# Add an executable target
add_executable(${FIRMWARE} main.cpp)

# Specify the libraries to use when linking the target
target_link_libraries(${FIRMWARE} ${INTERN_LINK_LIBS} ${EXTERN_LINK_LIBS})

# Set output name property of the target to target.elf
set_target_properties(${FIRMWARE} PROPERTIES OUTPUT_NAME "${FIRMWARE}.elf")

# Add custom target to ALL target to build the actual target.elf by discarding symbols from object file
add_custom_target(${FIRMWARE}_strip ALL     ${AVRSTRIP} "${FIRMWARE}.elf" DEPENDS ${FIRMWARE})

# Add custom command to list section sizes and total size
if (UNIX)
    add_custom_command(TARGET ${FIRMWARE}_strip POST_BUILD COMMAND ${AVRSIZE} -C --mcu=${MCU} "${FIRMWARE}.elf")
elseif (WIN32)
    add_custom_command(TARGET ${FIRMWARE}_strip POST_BUILD COMMAND ${AVRSIZE} -B --format=gnu --mcu=${MCU} "${FIRMWARE}.elf")
endif ()

# Add custom command to display the assembler mnemonics for the machine instructions
add_custom_command(TARGET ${FIRMWARE}_strip POST_BUILD COMMAND ${OBJDUMP} -S --source  "${FIRMWARE}.elf" > "${FIRMWARE}.lst")

# Add custom target to ALL target to build target.hex by removing eeprom section
add_custom_target(${FIRMWARE}_hex   ALL     ${OBJCOPY} -j .text -j .data -O ihex "${FIRMWARE}.elf" "${FIRMWARE}.hex" DEPENDS ${FIRMWARE}_strip)

# Add custom target to build target.eeprom by extracting eeprom section
add_custom_target(${FIRMWARE}_eeprom        ${OBJCOPY} -j .eeprom --change-section-lma .eeprom=0 -O ihex "${FIRMWARE}.elf" "${FIRMWARE}.eeprom" DEPENDS ${FIRMWARE}_strip)

# Flash the firmware with avrdude
add_custom_target(flash ${AVRFLASH_TOOL} -c ${PROG_TYPE} -p ${MCU} -P ${AVRFLASH_PORT} -U flash:w:"${FIRMWARE}.hex" DEPENDS ${FIRMWARE}_hex)

# Flash the eeprom with avrdude
add_custom_target(flash_eeprom ${AVRFLASH_TOOL}  -c ${PROG_TYPE} -p ${MCU}  -P ${AVRFLASH_PORT} -U eeprom:w:"${FIRMWARE}.eeprom" DEPENDS ${FIRMWARE}_eeprom )

# Add files for clean process
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${FIRMWARE}.elf ${FIRMWARE}.hex;${FIRMWARE}.eeprom;${FIRMWARE}.lst")
