cmake_minimum_required(VERSION 3.10)
project(ESP32_WiFi_Telemetry CXX C)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../lib")

include_directories(
    "${LIB_DIR}/core"
    "${LIB_DIR}/io"
    "${LIB_DIR}/component"
    "${LIB_DIR}/utils"
)

file(GLOB LIB_SOURCES
    "${LIB_DIR}/core/*.cpp"
    "${LIB_DIR}/io/*.cpp"
    "${LIB_DIR}/component/*.cpp"
    "${LIB_DIR}/utils/*.cpp"
)

add_executable(ESP32_WiFi_Telemetry main.cpp ${LIB_SOURCES})
