cmake_minimum_required(VERSION 3.14)


# Program name
project (TAHAKOMAVRLIB C CXX ASM)


# Set MCU library path
set(LIB_DIR "./lib"
    CACHE PATH "The path to the MCU libraries.")

# Add MCU library to the compiler include directories
include_directories(${LIB_DIR}/io
                    ${LIB_DIR}/components
                    ${LIB_DIR}/core
                    ${LIB_DIR}/utils)

# Add subdirectories to the build
add_subdirectory(${LIB_DIR}/io)
add_subdirectory(${LIB_DIR}/components)
add_subdirectory(${LIB_DIR}/core)
add_subdirectory(${LIB_DIR}/utils)


