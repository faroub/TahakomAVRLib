cmake_minimum_required(VERSION 3.14)

# Set AVR GCC toolchain
set(AVRCPP   avr-g++)
set(AVRC     avr-gcc)
set(AVRSTRIP avr-strip)
set(OBJCOPY  avr-objcopy)

# Set AVR chip configuration
set(MCU   atmega328p)
set(F_CPU 16000000)
set(BAUD  9600)

# Pass defines to the compiler
# __AVR_ATmega328P__ definition is necessary to use QtCreator IDE
add_definitions(-DF_CPU=${F_CPU} -DBAUD=${BAUD} -D__AVR_ATmega328P__)

# Use AVR GCC toolchain
set(CMAKE_SYSTEM_NAME  Generic)
set(CMAKE_CXX_COMPILER ${AVRCPP})
set(CMAKE_C_COMPILER   ${AVRC})
set(CMAKE_ASM_COMPILER   ${AVRC})

# Program name
project (BlinkLed C CXX ASM)

# Set a convenient common name for all applications
set(MCU_APPLICATION APP_TEST)

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
include_directories(${LIB_DIR}/io
                    ${LIB_DIR}/components
                    ${LIB_DIR}/core
                    ${LIB_DIR}/utils)

# Add subdirectories to the build
add_subdirectory(${LIB_DIR}/io ${CMAKE_BINARY_DIR}/io)
add_subdirectory(${LIB_DIR}/components ${CMAKE_BINARY_DIR}/components)

# Set the internal and external libs
set (INTERN_LINK_LIBS ${INTERN_LINK_LIBS} components io)
set (EXTERN_LINK_LIBS ${EXTERN_LINK_LIBS})

# Add an executable target
add_executable(${MCU_APPLICATION} main.cpp)

# Specify the libraries to use when linking the target
target_link_libraries(${MCU_APPLICATION} ${INTERN_LINK_LIBS} ${EXTERN_LINK_LIBS})

# Set output name property of the target to target.elf
set_target_properties(${MCU_APPLICATION} PROPERTIES OUTPUT_NAME "${MCU_APPLICATION}.elf")

# Add custom target to ALL target to build the actual target.elf by discarding symbols from object file
add_custom_target(${MCU_APPLICATION}_strip ALL     ${AVRSTRIP} "${MCU_APPLICATION}.elf" DEPENDS ${MCU_APPLICATION})

# Add custom target to ALL target to build target.hex by removing eeprom section
add_custom_target(${MCU_APPLICATION}_hex   ALL     ${OBJCOPY} -R .eeprom -O ihex "${MCU_APPLICATION}.elf" "${MCU_APPLICATION}.hex" DEPENDS ${MCU_APPLICATION}_strip)

# Add custom target to build target.eeprom by extracting eeprom section
add_custom_target(${MCU_APPLICATION}_eeprom        ${OBJCOPY} -j .eeprom --change-section-lma .eeprom=0 -O ihex "${MCU_APPLICATION}.elf" "${MCU_APPLICATION}.eeprom" DEPENDS ${MCU_APPLICATION}_strip)






# Add files for clean process
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${MCU_APPLICATION}.elf ${MCU_APPLICATION}.hex;${MCU_APPLICATION}.eeprom;${MCU_APPLICATION}.lst")
