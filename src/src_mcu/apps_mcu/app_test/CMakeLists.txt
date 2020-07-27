cmake_minimum_required(VERSION 2.8)



# set an Environment Variable for Build Settings
set(ENV{MCU_PROJECT_NAME} APP_TEST)
message("MCU_PROJECT_NAME = $ENV{MCU_PROJECT_NAME}")


set (INTERN_LINK_LIBS ${INTERN_LINK_LIBS} core components io utils)
set (EXTERN_LINK_LIBS ${EXTERN_LINK_LIBS})

# target
add_executable($ENV{MCU_PROJECT_NAME} main.cpp)
target_link_libraries($ENV{MCU_PROJECT_NAME} ${INTERN_LINK_LIBS} ${EXTERN_LINK_LIBS})
# several executables 1) mcu code 2) Qt gui code

set_target_properties($ENV{MCU_PROJECT_NAME} PROPERTIES OUTPUT_NAME "$ENV{MCU_PROJECT_NAME}.elf")

# Compiling targets
add_custom_target($ENV{MCU_PROJECT_NAME}_strip ALL     ${AVRSTRIP} "$ENV{MCU_PROJECT_NAME}.elf" DEPENDS $ENV{MCU_PROJECT_NAME})
add_custom_target($ENV{MCU_PROJECT_NAME}_hex   ALL     ${OBJCOPY} -R .eeprom -O ihex "$ENV{MCU_PROJECT_NAME}.elf" "$ENV{MCU_PROJECT_NAME}.hex" DEPENDS $ENV{MCU_PROJECT_NAME}_strip)
add_custom_target($ENV{MCU_PROJECT_NAME}_eeprom        ${OBJCOPY} -j .eeprom --change-section-lma .eeprom=0 -O ihex "$ENV{MCU_PROJECT_NAME}.elf" "$ENV{MCU_PROJECT_NAME}.eeprom" DEPENDS $ENV{MCU_PROJECT_NAME}_strip)
