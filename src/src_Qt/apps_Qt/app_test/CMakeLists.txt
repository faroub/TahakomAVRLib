cmake_minimum_required(VERSION 2.8)



# set an Environment Variable for Build Settings
set(ENV{AVR_MCU_QT_PROJECT_NAME} APP_TEST)
message("AVR_MCU_QT_PROJECT_NAME = $ENV{AVR_MCU_QT_PROJECT_NAME}")


set (INTERN_LINK_LIBS ${INTERN_LINK_LIBS} io gui processing)
set (EXTERN_LINK_LIBS ${EXTERN_LINK_LIBS} Qt5::Widgets -lpthread)

# target
add_executable($ENV{AVR_MCU_QT_PROJECT_NAME} main.cpp)
target_link_libraries($ENV{AVR_MCU_QT_PROJECT_NAME} ${INTERN_LINK_LIBS} ${EXTERN_LINK_LIBS})
