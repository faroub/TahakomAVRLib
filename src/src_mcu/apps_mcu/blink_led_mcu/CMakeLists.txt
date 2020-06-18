cmake_minimum_required(VERSION 2.8)

project (BlinkLed C CXX ASM)

# set an Environment Variable for Build Settings
set(ENV{MCU_PROJECT_NAME} ${PROJECT_NAME})
message("MCU_PROJECT_NAME = $ENV{MCU_PROJECT_NAME}")

# target
add_executable(${PROJECT_NAME} main.cpp)
#target_link_libraries(${PROJECT_NAME} ${INTERN_LINK_LIBS} ${EXTERN_LINK_LIBS})
# several executables 1) mcu code 2) Qt gui code

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}.elf")

# Compiling targets
add_custom_target(${PROJECT_NAME}_strip ALL     ${AVRSTRIP} "${PROJECT_NAME}.elf" DEPENDS ${PROJECT_NAME})
add_custom_target(${PROJECT_NAME}_hex   ALL     ${OBJCOPY} -R .eeprom -O ihex "${PROJECT_NAME}.elf" "${PROJECT_NAME}.hex" DEPENDS ${PROJECT_NAME}_strip)
add_custom_target(${PROJECT_NAME}_eeprom        ${OBJCOPY} -j .eeprom --change-section-lma .eeprom=0 -O ihex "${PROJECT_NAME}.elf" "${PROJECT_NAME}.eeprom" DEPENDS ${PROJECT_NAME}_strip)
