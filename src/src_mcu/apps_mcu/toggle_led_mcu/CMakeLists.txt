cmake_minimum_required(VERSION 2.8)

project (ToggleLed C CXX ASM)

#set(LIB_DIR "../../lib_mcu"
#    CACHE PATH "The path to the mcu library.")

#include_directories(${LIB_DIR}/io
#    ${LIB_DIR}/components)
#add_subdirectory(${LIB_DIR}/io ${CMAKE_CURRENT_BINARY_DIR}/io)
#add_subdirectory(${LIB_DIR}/components ${CMAKE_CURRENT_BINARY_DIR}/components)

#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist/lib)
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist/lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist/bin)

#set (INTERN_LINK_LIBS ${INTERN_LINK_LIBS} )
#set (EXTERN_LINK_LIBS ${EXTERN_LINK_LIBS} )
#set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build-mcu)

#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
#set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

#include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# target
add_executable(${PROJECT_NAME} main.cpp)
#target_link_libraries(${PROJECT_NAME} ${INTERN_LINK_LIBS} ${EXTERN_LINK_LIBS})
# several executables 1) mcu code 2) Qt gui code

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}.elf")

# Compiling targets
add_custom_target(${PROJECT_NAME}_strip ALL     ${AVRSTRIP} "${PROJECT_NAME}.elf" DEPENDS ${PROJECT_NAME})
add_custom_target(${PROJECT_NAME}_hex   ALL     ${OBJCOPY} -R .eeprom -O ihex "${PROJECT_NAME}.elf" "${PROJECT_NAME}.hex" DEPENDS ${PROJECT_NAME}_strip)
add_custom_target(${PROJECT_NAME}_eeprom        ${OBJCOPY} -j .eeprom --change-section-lma .eeprom=0 -O ihex "${PROJECT_NAME}.elf" "${PROJECT_NAME}.eeprom" DEPENDS ${PROJECT_NAME}_strip)
