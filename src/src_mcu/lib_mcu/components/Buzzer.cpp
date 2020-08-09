#include "Buzzer.h"


component::Buzzer::Buzzer(const io::Pin &ar_pin)
                 : m_pin(ar_pin)
{
    m_pin.toOutput();

}

component::Buzzer::~Buzzer()
{

}



void component::Buzzer::buzz(const uint16_t &ar_frequency_hz , const uint16_t &ar_duration_ms)
{
    uint16_t l_period_ms = 0xFFFF;

    if (ar_frequency_hz)
    {

        l_period_ms = 1000/ar_frequency_hz;
    }

    for (uint16_t i = 0; i < ar_duration_ms; i += l_period_ms)
    {
        /* For loop with variable delay selects the pitch */
        for (uint16_t j = 0; j < l_period_ms; j++)
        {
            _delay_ms(1);
        }
        m_pin.toggle();
    }


}

void component::Buzzer::noBuzz()
{
    m_pin.setLow();

}

void component::Buzzer::pause(uint16_t &ar_duration_ms)
{
    do {
      _delay_ms(1);
    } while (--ar_duration_ms);

}
