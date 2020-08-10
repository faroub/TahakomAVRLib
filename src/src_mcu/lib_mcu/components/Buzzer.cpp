#include "Buzzer.h"


component::Buzzer::Buzzer(const io::Pin &ar_pin)
                 : m_pin(ar_pin)
{
    m_pin.toOutput();

}

component::Buzzer::~Buzzer()
{

}



void component::Buzzer::buzz(const uint16_t &ar_period_us , const uint16_t &ar_duration_ms)
{
    uint32_t l_duration_us = ar_duration_ms * 1000;

    for (uint32_t i = 0; i < l_duration_us; i += ar_period_us)
    {
        /* For loop with variable delay selects the pitch */
        // _delay_us() needs a constant defined at compile time
        for (uint16_t j = 0; j < ar_period_us; j++)
        {
            _delay_us(1);
        }
        m_pin.toggle();
    }


}

void component::Buzzer::noBuzz()
{
    m_pin.setLow();

}

