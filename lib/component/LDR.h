#ifndef LDR_H
#define LDR_H
#include "Pin.h"
#include "ADC.h"

/**
 *
 * @brief AVR chip external components
 *
 * The namespace englobes all external components that, when hooked up,
 * allows the AVR MCU to interact with the external World
 *
*/

namespace component
{

/**
 * @brief Class for handling an LDR component
 *
 * The class implements LDR operations
 *
 * @author Farid Oubbati (farid.oubbati@outlook.com)
 * @date March 2018
*/

class LDR
{

public:
    /**
     *  Value returned by getResistance() when the ADC reads 0: the
     *  voltage divider formula would divide by zero, so callers can
     *  test for this sentinel instead of crashing.
     */
    static const uint32_t ADC_ZERO_SENTINEL = 0xFFFFFFFFUL;

    /**
     *  Constructor
     *
     *  Initalizes the LDR object and selects the LDR pin as the analog
     *  input of the ADC.  The pin must be one of the ADC pins
     *  (PC0..PC5 on the ATmega328P).
     *
     *  @param ar_pin Defines a Pin object
     *  @param ar_ADC Defines the ADC object
     */
    LDR(const io::Pin &ar_pin,core::ADConverter &ar_ADC);
    /**
     *  Destructor
     */
    ~LDR();
    /**
     *  Compute LDR resistance
     *
     *  An ADC result of 0 (the LDR is an open circuit) is guarded: the
     *  function returns ADC_ZERO_SENTINEL instead of dividing by zero.
     *
     *  @param ar_VDResistor Defines the voltage devider resistor in Kohm
     *  @return  LDR resistance in Ohm, or ADC_ZERO_SENTINEL
     */
    uint32_t getResistance(const uint16_t &ar_VDResistor);

    /**
     *  Compute amount of illuminance
     *
     *  The formula is evaluated as float once and clamped to the
     *  0..65535 range before the cast, so a brightly lit LDR saturates
     *  the result instead of wrapping around the 16-bit return value.
     *  An ADC result of 0 is the dark end of the scale and returns 0.
     *
     *  @param ar_VDResistor Defines the voltage devider resistor in Kohm
     *  @return  Amount of illuminance, clamped to 0..65535
     */
    uint16_t getLux(const uint16_t &ar_VDResistor);

protected:

private:

    io::Pin m_pin; /**< Pin object */
    core::ADConverter &mr_ADC; /**< ADC object reference */



};

}

#endif // LDR_H
