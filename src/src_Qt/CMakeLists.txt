cmake_minimum_required(VERSION 3.1.0)

project(AVR_MCU_QT)

# flags
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

# include files
include_directories(${Qt5Widgets_INCLUDE_DIRS})

set(LIB_DIR "./lib_Qt"
    CACHE PATH "The path to the Qt libraries.")

include_directories(${LIB_DIR}/gui
                    ${LIB_DIR}/io
                    ${LIB_DIR}/processing)

add_subdirectory(${LIB_DIR}/gui)
add_subdirectory(${LIB_DIR}/io)
add_subdirectory(${LIB_DIR}/processing)

set(APPS_DIR "./apps_Qt"
    CACHE PATH "The path to the Qt apps.")

add_subdirectory(${APPS_DIR}/app_test)
#add_subdirectory(${APPS_DIR}/usart_mcu)



