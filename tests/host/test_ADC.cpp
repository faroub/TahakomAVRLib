/**
 * @file test_ADC.cpp
 * @brief Host test of the ADC register handling
 *
 * Register-level host test: the device registers used here (ADMUX, DIDR0,
 * ADCSRA, ADCSRB, ADC, PRR, DDRC/PORTC/PINC) are defined as plain globals in
 * this file; the stub headers under stubs/ supply the ATmega328P bit
 * constants.
 */

#include <stdint.h>
#include <stdio.h>

// device registers as plain globals
volatile uint8_t DDRB, PORTB, PINB, DDRD, PORTD, PIND;
volatile uint8_t SMCR, MCUCR;
volatile uint8_t ADMUX;
volatile uint8_t DIDR0;
volatile uint8_t ADCSRA;
volatile uint8_t ADCSRB;
volatile uint16_t ADC;
volatile uint8_t PRR;
volatile uint8_t DDRC;
volatile uint8_t PORTC;
volatile uint8_t PINC;

#include "ADC.h"

static uint8_t failures = 0;

#define CHECK(cond) \
    do { \
        if (!(cond)) { \
            printf("  FAIL: %s (line %d)\n", #cond, __LINE__); \
            failures++; \
        } \
    } while (0)

int main(void)
{
    core::ADConverter &adc = core::ADConverter::getInstance();

    printf("reference voltage -> ADMUX bits 7:6 (REFS):\n");
    struct RefCase { const char *name; core::referenceVoltage ref; uint8_t expected; };
    const RefCase refs[] = {
        { "AREF",    core::referenceVoltage::AREF,    0 },
        { "AVCC",    core::referenceVoltage::AVCC,    1 },
        { "internal", core::referenceVoltage::internal, 3 },
    };
    for (uint8_t i = 0; i < 3; i++)
    {
        ADMUX = 0;
        adc.selectReferenceVoltage(refs[i].ref);
        printf("  %-9s REFS = 0b%02b (expected 0b%02b)\n",
               refs[i].name, (ADMUX >> 6) & 3, refs[i].expected);
        CHECK(((ADMUX >> REFS0) & 3u) == refs[i].expected);
        CHECK((ADMUX & 0x3F) == 0);   // only the REFS field touched
    }

    printf("selectAnalogInput sets only the selected DIDR0 bit:\n");
    DIDR0 = 0x00;
    adc.selectAnalogInput(io::Pin(3, io::PortC));
    printf("  pin 3 -> DIDR0 = 0x%02X\n", DIDR0);
    CHECK(DIDR0 == (1 << ADC3D));
    adc.selectAnalogInput(io::Pin(4, io::PortC));
    printf("  pin 4 -> DIDR0 = 0x%02X\n", DIDR0);
    CHECK(DIDR0 == ((1 << ADC3D) | (1 << ADC4D)));   // previous bit not cleared

    printf("ADLAR handling per resolution (ADMUX bit 5):\n");
    struct ResCase { const char *name; core::resolution res; uint8_t adlar; };
    const ResCase cases[] = {
        { "8bit",  core::resolution::res_8bit,  1 },
        { "9bit",  core::resolution::res_9bit,  1 },
        { "10bit", core::resolution::res_10bit, 0 },
        { "11bit", core::resolution::res_11bit, 0 },
        { "12bit", core::resolution::res_12bit, 0 },
        { "13bit", core::resolution::res_13bit, 0 },
        { "14bit", core::resolution::res_14bit, 0 },
        { "15bit", core::resolution::res_15bit, 0 },
        { "16bit", core::resolution::res_16bit, 0 },
    };
    for (uint8_t i = 0; i < 9; i++)
    {
        ADMUX = 0;
        adc.getConversionResult(cases[i].res);
        uint8_t adlar = (ADMUX >> ADLAR) & 1u;
        printf("  %-6s ADLAR = %u (expected %u)\n", cases[i].name, adlar, cases[i].adlar);
        CHECK(adlar == cases[i].adlar);
    }

    printf("start/stop use the ADEN enable bit:\n");
    ADCSRA = 0;
    adc.start();
    CHECK((ADCSRA & (1 << ADEN)) != 0);
    CHECK((ADCSRA & (1 << ADSC)) != 0);
    adc.stop();
    CHECK((ADCSRA & (1 << ADEN)) == 0);   // stop() relies on ADEN, not ADSC

    printf("conversion complete ISR with 12-bit oversampling (4x decimation):\n");
    adc.getConversionResult(core::resolution::res_12bit);
    ADC = 1000;
    // 16 conversions accumulate, the 17th publishes the decimated result
    for (uint16_t i = 0; i < 17; i++)
    {
        core::ADConverter::conversionCompleteServiceRoutine();
    }
    uint16_t result = adc.getConversionResult(core::resolution::res_12bit);
    printf("  16 x 1000 >> 2 = %u (expected 4000)\n", result);
    CHECK(result == 4000);

    printf("%s: %u failure(s)\n", failures ? "FAILED" : "PASSED", failures);
    return failures ? 1 : 0;
}
