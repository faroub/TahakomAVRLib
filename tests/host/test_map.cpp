/**
 * @file test_map.cpp
 * @brief Characterization tests of utils::map
 *
 * No hardware involved: utils::map is pure integer arithmetic, so this
 * test pins down its CURRENT behavior exactly: Arduino-style linear
 * mapping with integer truncation, no clamping at the range ends, and
 * extrapolation when x falls outside [in_min, in_max].  Cases marked
 * XFAIL are known behaviors that callers may not expect.
 */

#include <stdint.h>
#include <stdio.h>

#include "utils_m328p.h"

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

int main(void)
{
    printf("mapping inside the range truncates toward zero:\n");
    // (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min
    struct Case { long x; long in_min; long in_max; long out_min; long out_max; long expected; };
    const Case cases[] = {
        {   0,   0, 1023,   0, 255,     0 },   // bottom end, exact
        { 1023,   0, 1023,   0, 255,   255 },   // top end, exact
        { 512,    0, 1023,   0, 255,   127 },   // 127.52 truncated to 127
        {   2,    0,   10,   0, 100,    20 },
        {   5,    0,   10,   0, 100,    50 },   // midpoint, exact
        { 500,    0, 1000,   0,   2,     1 },
    };
    for (uint8_t i = 0; i < 6; i++)
    {
        long result = utils::map(cases[i].x, cases[i].in_min, cases[i].in_max,
                                 cases[i].out_min, cases[i].out_max);
        printf("  map(%4ld, %4ld, %4ld, %4ld, %4ld) = %ld (expected %ld)\n",
               cases[i].x, cases[i].in_min, cases[i].in_max,
               cases[i].out_min, cases[i].out_max, result, cases[i].expected);
        CHECK(result == cases[i].expected);
    }

    printf("negative and inverted ranges:\n");
    CHECK(utils::map(-5, -10, 10, 0, 100) == 25);    // negative input range
    CHECK(utils::map(5, 10, 0, 0, 100) == 50);       // in_min > in_max
    CHECK(utils::map(5, 0, 10, 100, 0) == 50);       // out_min > out_max
    printf("  map(-5,-10,10,0,100)=%ld map(5,10,0,0,100)=%ld map(5,0,10,100,0)=%ld\n",
           utils::map(-5, -10, 10, 0, 100),
           utils::map(5, 10, 0, 0, 100),
           utils::map(5, 0, 10, 100, 0));

    printf("remainders are dropped, not rounded:\n");
    // 1 * 2 / 3 = 0.66 -> 0, a rounding implementation would give 1
    CHECK(utils::map(1, 0, 3, 0, 2) == 0);
    XFAIL(utils::map(1, 0, 3, 0, 2) == 1);   // rounding, not truncating
    printf("  map(1,0,3,0,2)=%ld\n", utils::map(1, 0, 3, 0, 2));

    printf("values outside the input range are extrapolated, not clamped:\n");
    CHECK(utils::map(15, 0, 10, 0, 100) == 150);
    XFAIL(utils::map(15, 0, 10, 0, 100) == 100);   // clamped at out_max
    CHECK(utils::map(-5, 0, 10, 0, 100) == -50);
    XFAIL(utils::map(-5, 0, 10, 0, 100) == 0);     // clamped at out_min
    printf("  map(15,0,10,0,100)=%ld map(-5,0,10,0,100)=%ld\n",
           utils::map(15, 0, 10, 0, 100), utils::map(-5, 0, 10, 0, 100));

    // The LightMeter mapping (applications/LightMeter/main.cpp): the
    // lux measurement is clamped to [LUX_MIN, LUX_MAX] and mapped to the
    // 0..8 count of lit LEDs.  The clamp is essential because utils::map
    // extrapolates instead of clamping.
    printf("the LightMeter lux-to-LED mapping is monotone and saturated:\n");
    const long LUX_MIN = 1;
    const long LUX_MAX = 1000;
    struct LuxCase { uint16_t lux; uint8_t expected; };
    const LuxCase luxCases[] = {
        {    0, 0 },   // dark, clamped to LUX_MIN -> 0 LEDs
        {    1, 0 },   // LUX_MIN itself
        {  100, 0 },   // 0.79 -> 0
        {  126, 1 },   // 1.0 -> 1
        {  500, 3 },   // 3.99 -> 3
        {  750, 5 },   // 5.99 -> 5
        {  875, 6 },   // 6.99 -> 6
        {  999, 7 },   // 7.98 -> 7
        { 1000, 8 },   // LUX_MAX -> all 8 LEDs
        { 65535, 8 },  // sunlight, clamped to LUX_MAX -> 8
    };
    for (uint8_t i = 0; i < 10; i++)
    {
        uint16_t l_lux = luxCases[i].lux;
        if (l_lux < LUX_MIN)
        {
            l_lux = LUX_MIN;
        }
        if (l_lux > LUX_MAX)
        {
            l_lux = LUX_MAX;
        }
        uint8_t l_numLeds = static_cast<uint8_t>(utils::map(l_lux, LUX_MIN, LUX_MAX, 0, 8));
        printf("  lux=%5u -> %u LED(s) (expected %u)\n",
               luxCases[i].lux, l_numLeds, luxCases[i].expected);
        CHECK(l_numLeds == luxCases[i].expected);
    }

    printf("%s: %u failure(s), %u xfail(s), %u xpass(es)\n",
           failures ? "FAILED" : "PASSED", failures, xfailures, xpasses);
    return failures ? 1 : 0;
}
