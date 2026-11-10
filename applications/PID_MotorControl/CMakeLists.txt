cmake_minimum_required(VERSION 3.14)

include(${CMAKE_SOURCE_DIR}/../cmake/config.cmake)

set(AVRCPP   avr-g++)
set(AVRC     avr-gcc)
set(AVRSTRIP avr-strip)
set(OBJCOPY  avr-objcopy)
set(OBJDUMP avr-objdump)
set(AVRSIZE avr-size)

set(AVRFLASH_TOOL avrdude)

add_definitions(-DF_CPU=${F_CPU} -DBAUD=${BAUD} -D__AVR_ATmega328P__)

set(CMAKE_SYSTEM_NAME  Generic)
set(CMAKE_CXX_COMPILER ${AVRCPP})
set(CMAKE_C_COMPILER   ${AVRC})
set(CMAKE_ASM_COMPILER ${AVRC})

project (PID_MotorControl C CXX ASM)

set(FIRMWARE APP)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CTUNING "-funsigned-char -funsigned-bitfields -fshort-enums -ffunction-sections -fdata-sections -fno-threadsafe-statics")
set(CXXFLAGS "-mmcu=${MCU} -DF_CPU=${F_CPU} -DBAUD=${BAUD} -Os ${CTUNING} -Wall -Wextra -fno-exceptions -fno-rtti")

set(CMAKE_CXX_FLAGS "${CXXFLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections")

set(LIB_DIR "../../lib" CACHE PATH "The path to the MCU libraries.")

include_directories("${LIB_DIR}/io" "${LIB_DIR}/component" "${LIB_DIR}/core" "${LIB_DIR}/utils")

add_subdirectory("${LIB_DIR}/io" "${CMAKE_BINARY_DIR}/io")
add_subdirectory("${LIB_DIR}/component" "${CMAKE_BINARY_DIR}/component")
add_subdirectory("${LIB_DIR}/core" "${CMAKE_BINARY_DIR}/core")
add_subdirectory("${LIB_DIR}/utils" "${CMAKE_BINARY_DIR}/utils")

execute_process(COMMAND ${AVRCPP} -mmcu=${MCU} -print-file-name=crt${MCU}.o OUTPUT_VARIABLE AVRCRT OUTPUT_STRIP_TRAILING_WHITESPACE)
set (INTERN_LINK_LIBS component io core utils)

add_executable("${FIRMWARE}" main.cpp)
set_target_properties("${FIRMWARE}" PROPERTIES LINK_FLAGS "-nostartfiles")
target_link_libraries("${FIRMWARE}" ${INTERN_LINK_LIBS} ${AVRCRT} m)
set_target_properties("${FIRMWARE}" PROPERTIES OUTPUT_NAME "${FIRMWARE}.elf")

add_custom_command(TARGET "${FIRMWARE}" POST_BUILD COMMAND ${AVRSTRIP} "${FIRMWARE}.elf")
add_custom_command(TARGET "${FIRMWARE}" POST_BUILD COMMAND ${AVRSIZE} --format=avr --mcu=${MCU} "${FIRMWARE}.elf")
add_custom_command(TARGET "${FIRMWARE}" POST_BUILD COMMAND ${OBJCOPY} -j .text -j .data -O ihex "${FIRMWARE}.elf" "${FIRMWARE}.hex")

add_custom_target(flash ${AVRFLASH_TOOL} -c ${PROG_TYPE} -p ${MCU} -P ${AVRFLASH_PORT} -U flash:w:"${FIRMWARE}.hex" DEPENDS "${FIRMWARE}")
