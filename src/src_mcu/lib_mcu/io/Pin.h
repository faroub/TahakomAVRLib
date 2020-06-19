/**
 * @file Pin.h
 * @brief Header file of the Pin class
 *
 * Basic class for IO abstraction of an avr chip Pin and Port
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/
#ifndef PIN_H
#define PIN_H
#include <stdint.h>
#include <avr/io.h>
namespace io
{

/** Structure. Contains pointers to the Port's registers
     *
     */
struct Port
{
  /** pointer to the data direction register
       */
  volatile uint8_t* mp_ddrReg;
  /** pointer to the port register
       */
  volatile uint8_t* mp_portReg;
  /** pointer to the pin register
       */
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
    Pin(const uint8_t &ar_pinNumber, const Port &ar_portName);
    /** Destructor.
        */
    ~Pin();
    /** Configure pin to output.
         */
    void toOutput();
    /** Configure pin to input.
         */
    void toInput();
    /** Set pin to logic low.
         */
    void setLow();
    /** Set pin to logic high.
         */
    void setHigh();
    /** Toggle pin state.
         */
    void toggle();
    /** Check if pin is logic high.
         */
    uint8_t isHigh() const;
    /** Check if pin is logic low.
         */
    uint8_t isLow() const;
    /** Get pin number.
         */
    uint8_t getPinNumber() const;

protected:


private:

     /** Constant reference to a port object
          */
     const Port &mr_portName;
     /** the pin number
          */
     const uint8_t &mr_pinNumber;



};

static io::Port PortB = { &DDRB, &PORTB, &PINB };
static io::Port PortC = { &DDRB, &PORTB, &PINB };
static io::Port PortD = { &DDRB, &PORTB, &PINB };

}


#endif
