cmake_minimum_required(VERSION 3.14)

# Include shared build configuration
include(${CMAKE_SOURCE_DIR}/../cmake/config.cmake)

# Set AVR GCC toolchain
set(AVRCPP   avr-g++)
set(AVRC     avr-gcc)
set(AVRSTRIP avr-strip)
set(OBJCOPY  avr-objcopy)
set(OBJDUMP avr-objdump)
set(AVRSIZE avr-size)

# Programmer type and flash port are defined in cmake/config.cmake
# The flash software tool
set(AVRFLASH_TOOL avrdude)


# Pass defines to the compiler
# __AVR_ATmega328P__ definition is only necessary when using an IDE
add_definitions(-DF_CPU=${F_CPU} -DBAUD=${BAUD} -D__AVR_ATmega328P__)

# Use AVR GCC toolchain
set(CMAKE_SYSTEM_NAME  Generic)
set(CMAKE_CXX_COMPILER ${AVRCPP})
set(CMAKE_C_COMPILER   ${AVRC})
set(CMAKE_ASM_COMPILER   ${AVRC})

# Program name
project (LightMeter C CXX ASM)

# Set a convenient common name for all applications
set(FIRMWARE APP)

# Set project pdf documentation name
set(PROJECTDOC TahakomAVRLibDoc)

# Use C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set compiler flags
set(CSTANDARD "-std=gnu99 ")
set(CDEBUG    "-gstabs -g -ggdb")
set(CWARN     "-Wall -Wstrict-prototypes -Wl,--relax")
set(CTUNING   "-funsigned-char -funsigned-bitfields -fshort-enums -ffunction-sections -fdata-sections -fno-threadsafe-statics")
set(COPT      "-Os")
set(CMCU      "-mmcu=${MCU}")
set(CDEFS     "-DF_CPU=${F_CPU} -DBAUD=${BAUD}")
set(CFLAGS   "${CMCU} ${CDEBUG} ${CDEFS} ${COPT} ${CWARN} ${CSTANDARD} ${CTUNING}")
set(CXXFLAGS "${CMCU} ${CDEBUG} ${CDEFS} ${COPT} ${CTUNING} -Wall -Wextra -fno-exceptions -fno-rtti")

# Move --gc-sections to the linker flags; it is a linker option
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections")

# Use compiler flags
set(CMAKE_C_FLAGS   "${CFLAGS}")
set(CMAKE_CXX_FLAGS "${CXXFLAGS}")
set(CMAKE_ASM_FLAGS   "${CFLAGS}")


# Build documentation along?
option(BUILD_DOC "Build documentation" OFF)


# Set MCU library path
set(LIB_DIR "../../lib"
    CACHE PATH "The path to the MCU libraries.")

# Add MCU library to the compiler include directories
include_directories("${LIB_DIR}/io"
                    "${LIB_DIR}/component"
                    "${LIB_DIR}/core"
                    "${LIB_DIR}/utils")

# Add subdirectories to the build
add_subdirectory("${LIB_DIR}/io" "${CMAKE_BINARY_DIR}/io")
add_subdirectory("${LIB_DIR}/component" "${CMAKE_BINARY_DIR}/component")
add_subdirectory("${LIB_DIR}/core" "${CMAKE_BINARY_DIR}/core")
add_subdirectory("${LIB_DIR}/utils" "${CMAKE_BINARY_DIR}/utils")

# Locate the CRT startup object: it must be linked AFTER the libraries, not
# in the driver's default first position.  The startup code weakly aliases
# every vector to __bad_interrupt and the first weak definition wins, so the
# library's weak default interrupt handlers only take effect if the CRT sees
# them first; -nostartfiles suppresses the automatic placement.
execute_process(COMMAND ${AVRCPP} -mmcu=${MCU} -print-file-name=crt${MCU}.o
                OUTPUT_VARIABLE AVRCRT OUTPUT_STRIP_TRAILING_WHITESPACE)
# Set the internal and external libs
set (INTERN_LINK_LIBS ${INTERN_LINK_LIBS} component io core utils)
set (EXTERN_LINK_LIBS ${EXTERN_LINK_LIBS})

# Add an executable target
add_executable("${FIRMWARE}" main.cpp)

# Specify the libraries to use when linking the target
# Suppress the driver's startup placement: the CRT is linked explicitly
# after the libraries (see AVRCRT above) so the library's weak default
# handlers take effect over the CRT's __bad_interrupt aliases
set_target_properties("${FIRMWARE}" PROPERTIES LINK_FLAGS "-nostartfiles")
target_link_libraries("${FIRMWARE}" ${INTERN_LINK_LIBS} ${EXTERN_LINK_LIBS} ${AVRCRT} m)

# Set output name property of the target to target.elf
set_target_properties("${FIRMWARE}" PROPERTIES OUTPUT_NAME "${FIRMWARE}.elf")

# Add custom command to build the actual target.elf by discarding symbols from object file
add_custom_command(TARGET "${FIRMWARE}" POST_BUILD COMMAND ${AVRSTRIP} "${FIRMWARE}.elf")

# Add custom command to list section sizes and total size
if (CMAKE_HOST_UNIX)
    add_custom_command(TARGET "${FIRMWARE}" POST_BUILD COMMAND ${AVRSIZE} --format=avr --mcu=${MCU} "${FIRMWARE}.elf")
elseif (CMAKE_HOST_WIN32)
    add_custom_command(TARGET "${FIRMWARE}" POST_BUILD COMMAND ${AVRSIZE} -B --format=gnu --mcu=${MCU} "${FIRMWARE}.elf")
endif ()

# Add custom command to display the assembler mnemonics for the machine instructions
add_custom_command(TARGET "${FIRMWARE}" POST_BUILD COMMAND ${OBJDUMP} -S --source  "${FIRMWARE}.elf" > "${FIRMWARE}.lst")

# Add custom command to build target.hex by removing eeprom section and target.eeprom by extracting eeprom section
add_custom_command(TARGET "${FIRMWARE}" POST_BUILD
    COMMAND ${OBJCOPY} -j .text -j .data -O ihex "${FIRMWARE}.elf" "${FIRMWARE}.hex"
    COMMAND ${OBJCOPY} -j .eeprom --change-section-lma .eeprom=0 -O ihex --no-change-warnings "${FIRMWARE}.elf" "${FIRMWARE}.eeprom")

# Flash the firmware with avrdude
add_custom_target(flash ${AVRFLASH_TOOL} -c ${PROG_TYPE} -p ${MCU} -P ${AVRFLASH_PORT} -U flash:w:"${FIRMWARE}.hex" DEPENDS "${FIRMWARE}")

# Flash the eeprom with avrdude
add_custom_target(flash_eeprom ${AVRFLASH_TOOL}  -c ${PROG_TYPE} -p ${MCU}  -P ${AVRFLASH_PORT} -U eeprom:w:"${FIRMWARE}.eeprom" DEPENDS "${FIRMWARE}")

# Add files for clean process
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${FIRMWARE}.elf;${FIRMWARE}.hex;${FIRMWARE}.eeprom;${FIRMWARE}.lst")

# Check to generate Doxygen docmentation
if (BUILD_DOC)

    # Check if Doxygen is installed
    find_package(Doxygen)

    if (DOXYGEN_FOUND)

        # set input and output files
        set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/../../docs/Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_BINARY_DIR}/Doxyfile)

        # request to configure the file
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

        message("Doxygen build started")

        # note the option ALL which allows to build the docs together with the application
        add_custom_target( DOC ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Generating documentation with Doxygen"
            VERBATIM )

        # optional: generate pdf from latex
        add_custom_command(TARGET DOC POST_BUILD
            COMMAND make  pdf
            COMMAND cmake -E rename  refman.pdf ${PROJECTDOC}.pdf
            COMMAND cmake -E copy ${PROJECTDOC}.pdf ${CMAKE_SOURCE_DIR}/../..
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/../../docs/latex )

    else (DOXYGEN_FOUND)

        message(WARNING "Doxygen not found, unable to generate documentation")

    endif (DOXYGEN_FOUND)
endif (BUILD_DOC)
