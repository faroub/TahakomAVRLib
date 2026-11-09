/**
 * @file test_Organ.cpp
 * @brief Tests of the Organ note-length arithmetic
 *
 * No hardware involved: this test mirrors the note-length arithmetic of
 * applications/Organ/main.cpp (uint16_t l_currentNoteLength and the
 * '+'/'-' tempo keys, with 32-bit saturation on '+' and zero rejection
 * on '-').  Cases marked XFAIL are known limits that remain.
 */

#include <stdint.h>
#include <stdio.h>

#define NOTE_DURATION  0xF000 /**< Note duration, as in the Organ app */

// '+' as handled by the Organ: double in 32 bits, saturate at 0xFFFF
static uint16_t doubleNoteLength(const uint16_t a_length)
{
    uint32_t l_newNoteLength = static_cast<uint32_t>(a_length) * 2UL;
    if (l_newNoteLength > 0xFFFFUL)
    {
        l_newNoteLength = 0xFFFFUL;
    }
    return static_cast<uint16_t>(l_newNoteLength);
}

// '-' as handled by the Organ: halve, but never reach zero
static uint16_t halveNoteLength(const uint16_t a_length)
{
    if (a_length > 1)
    {
        return static_cast<uint16_t>(a_length / 2);
    }
    return a_length;
}

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
    printf("the note duration constant:\n");
    printf("  NOTE_DURATION = 0x%04X = %u us = %u.%02u ms\n",
           NOTE_DURATION, NOTE_DURATION, NOTE_DURATION / 1000,
           (NOTE_DURATION % 1000) / 10);
    CHECK(NOTE_DURATION == 61440);
    // a played note is about 61 ms, far below a musical 200-400 ms; the
    // bit-banged Buzzer::buzz() takes its duration in us in a uint16_t,
    // so the Organ app cannot express longer notes in a single call
    XFAIL(NOTE_DURATION / 1000U >= 200);

    printf("'+' doubles the note length in 32 bits and saturates at 0xFFFF:\n");
    uint16_t l_currentNoteLength = NOTE_DURATION;
    l_currentNoteLength = doubleNoteLength(l_currentNoteLength);
    printf("  61440 * 2 -> %u (0x%04X)\n", l_currentNoteLength, l_currentNoteLength);
    CHECK(l_currentNoteLength == 0xFFFF);   // 122880 saturates instead of wrapping to 57344
    // doubling on from the top stays at the top, it never wraps back to
    // a small or a zero length
    for (uint8_t i = 0; i < 3; i++)
    {
        l_currentNoteLength = doubleNoteLength(l_currentNoteLength);
    }
    printf("  three more doublings -> %u (0x%04X)\n", l_currentNoteLength, l_currentNoteLength);
    CHECK(l_currentNoteLength == 0xFFFF);
    // mid-range lengths still double exactly
    CHECK(doubleNoteLength(10000) == 20000);
    CHECK(doubleNoteLength(32767) == 65534);

    printf("'-' halves the note length but never reaches zero:\n");
    l_currentNoteLength = NOTE_DURATION;
    for (uint8_t i = 0; i < 15; i++)
    {
        l_currentNoteLength = halveNoteLength(l_currentNoteLength);
    }
    printf("  after 15 halvings: %u\n", l_currentNoteLength);
    CHECK(l_currentNoteLength == 1);
    // '-' on 1 is rejected: a zero length would silence every note
    l_currentNoteLength = halveNoteLength(l_currentNoteLength);
    printf("  one more: %u\n", l_currentNoteLength);
    CHECK(l_currentNoteLength == 1);
    // mid-range lengths still halve exactly
    CHECK(halveNoteLength(20000) == 10000);
    CHECK(halveNoteLength(2) == 1);

    printf("every reachable note length is playable:\n");
    // the saturation and the zero rejection together keep the length in
    // the 1..65535 range of Buzzer::buzz()'s duration parameter
    CHECK(doubleNoteLength(0xFFFF) == 0xFFFF);
    CHECK(halveNoteLength(1) == 1);
    CHECK(doubleNoteLength(halveNoteLength(1)) == 2);

    printf("%s: %u failure(s), %u xfail(s), %u xpass(es)\n",
           failures ? "FAILED" : "PASSED", failures, xfailures, xpasses);
    return failures ? 1 : 0;
}
