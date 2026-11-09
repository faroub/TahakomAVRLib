/**
 * @file test_Organ.cpp
 * @brief Characterization tests of the Organ note-length arithmetic
 *
 * No hardware involved: this test mirrors the note-length arithmetic of
 * applications/Organ/main.cpp (uint16_t l_currentNoteLength and the
 * '+'/'-' tempo keys) to pin down its CURRENT behavior.  Cases marked
 * XFAIL are known-wrong behaviors that P26 reworks; an XFAIL that
 * starts to pass shows the fix landed.
 */

#include <stdint.h>
#include <stdio.h>

#define NOTE_DURATION  0xF000 /**< Note duration, as in the Organ app */

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
    // a played note is about 61 ms, far below a musical 200-400 ms
    XFAIL(NOTE_DURATION / 1000U >= 200);

    printf("'+' doubles the note length, wrapping around uint16_t:\n");
    uint16_t l_currentNoteLength = NOTE_DURATION;
    l_currentNoteLength = l_currentNoteLength * 2;
    printf("  61440 * 2 = %u (0x%04X)\n", l_currentNoteLength, l_currentNoteLength);
    CHECK(l_currentNoteLength == 57344);   // 122880 wraps to 57344
    XFAIL(l_currentNoteLength == 122880);  // doubling should not wrap
    // doubling on from 57344: 49152, then 32768, then 0 (65536 wraps)
    l_currentNoteLength = l_currentNoteLength * 2;
    CHECK(l_currentNoteLength == 49152);
    l_currentNoteLength = l_currentNoteLength * 2;
    CHECK(l_currentNoteLength == 32768);
    l_currentNoteLength = l_currentNoteLength * 2;
    printf("  doubling to the top of the range: %u\n", l_currentNoteLength);
    CHECK(l_currentNoteLength == 0);       // 65536 wraps to 0
    XFAIL(l_currentNoteLength == 0xFFFF);  // long notes should saturate

    printf("'-' halves the note length down to, and past, zero:\n");
    l_currentNoteLength = NOTE_DURATION;
    for (uint8_t i = 0; i < 15; i++)
    {
        l_currentNoteLength = l_currentNoteLength / 2;
    }
    printf("  after 15 halvings: %u\n", l_currentNoteLength);
    CHECK(l_currentNoteLength == 1);
    l_currentNoteLength = l_currentNoteLength / 2;
    printf("  one more: %u\n", l_currentNoteLength);
    CHECK(l_currentNoteLength == 0);
    XFAIL(l_currentNoteLength > 0);   // zero is never rejected as a duration
    // a zero note length plays nothing at all: Buzzer.buzz(note, 0)
    l_currentNoteLength = l_currentNoteLength / 2;
    CHECK(l_currentNoteLength == 0);

    printf("%s: %u failure(s), %u xfail(s), %u xpass(es)\n",
           failures ? "FAILED" : "PASSED", failures, xfailures, xpasses);
    return failures ? 1 : 0;
}
