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
     *  Constructor
     *
     *  Initalizes the LDR object
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
     *  @param ar_VDResistor Defines the voltage devider resistor in Kohm
     *  @return  LDR resistance
     */
    uint32_t getResistance(const uint8_t &ar_VDResistor);

    /**
     *  Compute amount of illuminance
     *  @param ar_VDResistor Defines the voltage devider resistor in Kohm
     *  @return  Amount of illuminance
     */
    uint16_t getLux(const uint8_t &ar_VDResistor);

protected:

private:

    io::Pin m_pin; /**< Pin object */
    core::ADConverter &mr_ADC; /**< ADC object reference */



};

}

#endif // LDR_H
