/**
 * @file test_LDR.cpp
 * @brief Characterization tests of the LDR resistance and lux formulas
 *
 * Register-level host test: the ADC result register ADC is a plain global
 * (host_registers.h); the conversion-complete ISR of core::ADConverter is
 * driven by hand to publish a chosen 10-bit result.  The stub headers
 * under stubs/ supply the ATmega328P bit constants.
 *
 * These tests characterize the CURRENT behavior of component::LDR:
 * the voltage-divider resistance formula, the lux formula (which wraps
 * around 16 bits instead of clamping when the LDR is brightly lit), and
 * the integer division by zero that occurs when the ADC reads 0.  Cases
 * marked XFAIL are known-wrong behaviors that P21 reworks; an XFAIL that
 * starts to pass shows the fix landed.
 */

#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "host_registers.h"

#include "ADC.h"
#include "LDR.h"

static uint8_t failures = 0;
static uint16_t xfailures = 0;
static uint16_t xpasses = 0;

#define CHECK(cond) \
    do { \
        if (!(cond)) { \
            printf("  FAIL: %s (line %d)\n", #cond, __LINE__); \
            failures++; \
        } \
    } while (0)

// expected-to-fail: known-wrong current behavior; must not fail the suite
#define XFAIL(cond) \
    do { \
        if (!(cond)) { \
            printf("  XFAIL (known-wrong): %s (line %d)\n", #cond, __LINE__); \
            xfailures++; \
        } \
        else { \
            printf("  XPASS (was expected to fail): %s (line %d)\n", #cond, __LINE__); \
            xpasses++; \
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

    // the LDR pin: the constructor only configures the pin as an input,
    // it does not select the ADC channel (that is P21's rework)
    component::LDR ldr(io::Pin(1, io::PortC), adc);
    CHECK(DDRC == 0);

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
    printf("  R=%lu -> lux=%u (double formula gives %.2f)\n",
           (unsigned long)resistance, lux, expectedLux);
    CHECK(lux == (uint16_t)expectedLux);

    printf("bright light saturates the LDR and wraps around uint16_t:\n");
    // ADC=1021 is the highest reading that leaves a nonzero resistance;
    // the resulting small R gives a lux value far above 65535
    setConversionResult(adc, 1021);
    uint16_t bright = ldr.getLux(5);
    double expectedBright = 12518931.0 * pow((double)ldr.getResistance(5), -1.405);
    printf("  ADC=1021 -> lux=%u (double formula gives %.2f, 0x%04X after the wrap)\n",
           bright, expectedBright, (unsigned)(uint16_t)expectedBright);
    CHECK(bright == (uint16_t)expectedBright);
    XFAIL(bright == 65535);   // a saturated sensor should clamp, not wrap

    printf("a dark LDR reads as 0 lux (division result rounded down):\n");
    setConversionResult(adc, 1023);
    uint16_t dark = ldr.getLux(5);
    printf("  ADC=1023 -> R=%lu, lux=%u\n",
           (unsigned long)ldr.getResistance(5), dark);
    CHECK(ldr.getResistance(5) == 0);
    CHECK(dark <= 10);   // the dark end collapses to (almost) zero

    printf("ADC == 0 divides by zero (SIGFPE), there is no sentinel:\n");
    fflush(stdout);
    pid_t l_pid = fork();
    if (l_pid == 0)
    {
        // publish a zero 10-bit result, then compute the resistance
        ADC = 0;
        core::ADConverter::conversionCompleteServiceRoutine();
        component::LDR child(io::Pin(2, io::PortC), adc);
        child.getResistance(5);
        _exit(0);
    }
    int l_status = 0;
    waitpid(l_pid, &l_status, 0);
    printf("  child killed by signal %d (expected SIGFPE=%d)\n",
           WIFSIGNALED(l_status) ? WTERMSIG(l_status) : 0, SIGFPE);
    XFAIL(!WIFSIGNALED(l_status));   // P21 guards the zero instead of crashing

    printf("%s: %u failure(s), %u xfail(s), %u xpass(es)\n",
           failures ? "FAILED" : "PASSED", failures, xfailures, xpasses);
    return failures ? 1 : 0;
}
