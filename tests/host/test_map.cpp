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

    printf("%s: %u failure(s), %u xfail(s), %u xpass(es)\n",
           failures ? "FAILED" : "PASSED", failures, xfailures, xpasses);
    return failures ? 1 : 0;
}
