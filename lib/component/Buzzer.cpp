/**
 *
 * @author Farid Oubbati (farid.oubbati@outlook.com)
 * @date March 2018
 *
 *  *
 * See Buzzer.h for a description of this code
*/

#include "Buzzer.h"


component::Buzzer::Buzzer(const io::Pin &ar_pin)
                 : m_pin(ar_pin)
{
    m_pin.toOutput();

}

component::Buzzer::~Buzzer()
{

}

void component::Buzzer::buzz(const uint16_t &ar_period_us , const uint16_t &ar_duration_us)
{

    for (uint16_t i = 0; i < ar_duration_us; i += ar_period_us)
    {
        // for loop with variable delay selects the pitch
        for (uint16_t j = 0; j < ar_period_us; j++)
        {
            _delay_us(1);
        }
        m_pin.toggle();
    }
    m_pin.setLow();

}


void component::Buzzer::pause(uint16_t a_duration_us)
{
    do {
      _delay_us(1);
    } while (--a_duration_us);
}
