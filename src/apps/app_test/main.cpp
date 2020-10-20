/**
 * @file main.cpp
 * @brief test mcu application
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/


#include "MCU.h"
#include "ServoMotor.h"
#include "TimerCounter1.h"

#define SERVOMOTOR_NUMBER 1
#define SERVOMOTOR_PULSE_CYCLE 30000 // pulse cycle [us]
#define SERVOMOTOR_PULSE_WIDTH_MIN 500// pulse width min [us]
#define SERVOMOTOR_PULSE_WIDTH_MID 1520 // pulse width mid [us]
#define SERVOMOTOR_PULSE_WIDTH_MAX 3000 // pulse width max [us]



int main(void) {

    // Init
    // initialize MCU
    core::MCU::init();

    // instantiate the TimerCounter1 object
    core::TimerCounter1 &myTimerCounter1 = core::TimerCounter1::getInstance();
    myTimerCounter1.selectClockSource(core::clockSource::PS_8);


    // instantiate the Buzzer object
    component::ServoMotor myServoMotor(io::Pin(SERVOMOTOR_NUMBER,io::PortB),
                                       SERVOMOTOR_PULSE_CYCLE,
                                       SERVOMOTOR_PULSE_WIDTH_MIN,
                                       SERVOMOTOR_PULSE_WIDTH_MID,
                                       SERVOMOTOR_PULSE_WIDTH_MAX);

    myServoMotor.connect(myTimerCounter1);

    myServoMotor.rotate(myTimerCounter1,0);
    _delay_ms(2000);
    myServoMotor.rotate(myTimerCounter1,45);
    _delay_ms(2000);
    myServoMotor.rotate(myTimerCounter1,90);
    _delay_ms(2000);
    myServoMotor.rotate(myTimerCounter1,135);
    _delay_ms(2000);
    myServoMotor.rotate(myTimerCounter1,180);
    _delay_ms(2000);
    myServoMotor.rotate(myTimerCounter1,0);
    _delay_ms(2000);

    myServoMotor.disconnect(myTimerCounter1);

    // Mainloop
    while (1) {



    }
    return 0;
}



