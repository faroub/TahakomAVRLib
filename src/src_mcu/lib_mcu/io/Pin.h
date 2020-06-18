/**
 * @file Pin.h
 * @brief Header file of the Pin class
 *
 * Basic IO abstraction of an avr chip Pin and Port
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/
#ifndef PIN_H
#define PIN_H
#include <stdint.h>
#include <avr/io.h>
namespace io
{

struct Port
{
  volatile uint8_t* mp_ddrReg;
  volatile uint8_t* mp_portReg;
  volatile uint8_t* mp_pinReg;
};

class Pin
{

public:
    /** Constructor. Initalizes the pin number and port name
         *
         *  @param ar_port IO port of the avr chip
         *  @param ar_pin IO pin of the avr chip
         */
    Pin(const uint8_t &ar_pin, const Port &ar_port);
    /** Default destructor.
        */
    ~Pin();
    /** configure pin to output.
         */
    void toOutput();
    /** configure pin to input.
         */
    void toInput();
    /** set pin to logic low.
         */
    void setLow();
    /** set pin to logic high.
         */
    void setHigh();
    /** toggle pin state.
         */
    void toggle();
    /** check if pin is logic high.
         */
    uint8_t isHigh() const;
    /** check if pin is logic low.
         */
    uint8_t isLow() const;
    /** get pin number.
         */
    uint8_t getPin() const;

protected:


private:

     const Port &mr_port;

     const uint8_t &mr_pin;



};
}


#endif
