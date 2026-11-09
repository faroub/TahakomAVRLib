/**
 * @file test_LDR.cpp
 * @brief Host test of the LDR resistance and lux formulas
 *
 * Register-level host test: the ADC result register ADC is a plain global
 * (host_registers.h); the conversion-complete ISR of core::ADConverter is
 * driven by hand to publish a chosen 10-bit result.  The stub headers
 * under stubs/ supply the ATmega328P bit constants.
 *
 * The tests pin the resistance formula of the voltage divider, the
 * clamped lux formula, the ADC channel selection in the constructor, and
 * the sentinel that guards an ADC result of 0 (P21).  The P18
 * characterization marks (wrapping lux, SIGFPE) are gone with the fix.
 */

#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "host_registers.h"

#include "ADC.h"
#include "LDR.h"

static uint8_t failures = 0;

#define CHECK(cond) \
    do { \
        if (!(cond)) { \
            printf("  FAIL: %s (line %d)\n", #cond, __LINE__); \
            failures++; \
        } \
    } while (0)

// publish a 10-bit result: at 10-bit resolution one ISR call stores ADC
static void setConversionResult(core::ADConverter &ar_adc, uint16_t a_value)
{
    ar_adc.getConversionResult(core::resolution::res_10bit);
    ADC = a_value;
    core::ADConverter::conversionCompleteServiceRoutine();
}

int main(void)
{
    core::ADConverter &adc = core::ADConverter::getInstance();

    // the LDR pin: the constructor selects it as the ADC input and
    // disables its digital input buffer (DIDR0 bit of channel 1); the
    // ADC singleton already selected its default channel 0
    component::LDR ldr(io::Pin(1, io::PortC), adc);
    CHECK(DDRC == 0);
    CHECK(DIDR0 == ((1 << 0) | (1 << 1)));
    CHECK((ADMUX & 0x0F) == 1);

    printf("voltage-divider resistance, fixed resistor 5 kOhm:\n");
    // R = r * (1000 * 1023 / adc - 1000), integer division
    struct Case { uint16_t adc; uint32_t expected; };
    const Case cases[] = {
        { 256, 14980 },
        { 512,  4990 },
        { 768,  1660 },
    };
    for (uint8_t i = 0; i < 3; i++)
    {
        setConversionResult(adc, cases[i].adc);
        uint32_t resistance = ldr.getResistance(5);
        printf("  ADC=%4u -> R=%5lu (expected %5lu)\n",
               cases[i].adc, (unsigned long)resistance, (unsigned long)cases[i].expected);
        CHECK(resistance == cases[i].expected);
    }

    printf("lux = 12518931 * pow(R, -1.405), truncated to uint16_t:\n");
    setConversionResult(adc, 512);
    uint32_t resistance = ldr.getResistance(5);
    uint16_t lux = ldr.getLux(5);
    double expectedLux = 12518931.0 * pow((double)resistance, -1.405);
    printf("  R=%lu -> lux=%u (float formula gives %.2f)\n",
           (unsigned long)resistance, lux, expectedLux);
    CHECK(lux == (uint16_t)expectedLux);

    printf("bright light clamps to 65535 instead of wrapping:\n");
    // ADC=1021 is the highest reading that leaves a nonzero resistance;
    // the resulting small R gives a lux value far above 65535
    setConversionResult(adc, 1021);
    uint16_t bright = ldr.getLux(5);
    printf("  ADC=1021 -> lux=%u\n", bright);
    CHECK(bright == 65535);

    printf("full scale (ADC=1023) is the bright end of the scale:\n");
    setConversionResult(adc, 1023);
    printf("  ADC=1023 -> R=%lu, lux=%u\n",
           (unsigned long)ldr.getResistance(5), ldr.getLux(5));
    CHECK(ldr.getResistance(5) == 0);
    CHECK(ldr.getLux(5) == 65535);

    printf("ADC == 0 returns the sentinel instead of dividing by zero:\n");
    setConversionResult(adc, 0);
    uint32_t dark = ldr.getResistance(5);
    printf("  R=0x%08lX (expected 0x%08lX)\n",
           (unsigned long)dark, (unsigned long)component::LDR::ADC_ZERO_SENTINEL);
    CHECK(dark == component::LDR::ADC_ZERO_SENTINEL);
    CHECK(ldr.getLux(5) == 0);

    printf("%s: %u failure(s)\n", failures ? "FAILED" : "PASSED", failures);
    return failures ? 1 : 0;
}
