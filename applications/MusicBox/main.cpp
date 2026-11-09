/**
 * @file main.cpp
 *
 * Demo example: AVR Music Box
 *
 * Usage demonstration of the TahakomAVRLib in an AVR based Music Box.
 * The example allows the user to step through a predefined song in a Buzzer
 * one note at a time via a PushButton; a note plays as long as the button is held.
 * This example demonstrates the use of the component::Buzzer() and component::PushButton() abstraction objects.
 *
 * - Compiler: avr-gcc (GCC) 5.4.0
 * - Supported devices: ATmega328P only
 *
 *
 * @author Farid Oubbati (farid.oubbati@outlook.com)
 * @date March 2018
*/

#include "Buzzer.h"
#include "PushButton.h"
#include "TimerCounter1.h"
#include <avr/pgmspace.h>
// Note pitch data
#include "buzzer_pitches_16bit.h"


#define BUZZER 2 /**< Buzzer pin number: PB2 is the Timer1 output-compare
                     pin OC1B, the pin the timer-driven buzz() toggles */

#define PUSHBUTTON 1 /**< PushButton pin number */

// Audible length of every played note in ms: the notes of the song are
// audible at this length, one note per button press
#define NOTE_DURATION_MS 300

#define SONG_LENGTH (sizeof(l_song) / sizeof(uint16_t)) /**< Song length */

int main(void) {

  // Init

  // Instantiate a Buzzer object on the Timer1 output-compare pin OC1B:
  // the timer-driven buzz() drives the hardware compare output, so the
  // buzzer must be wired to the compare pin of the timer in use
  component::Buzzer Buzzer(io::Pin(BUZZER,io::PortB));

  // Instantiate a TimerCounter1 object: the pitch table holds the note
  // periods in us, so the timer is clocked at prescaler 64 where one
  // tick is exactly 4 us at F_CPU = 16 MHz and the tick value of a note
  // is a quarter of its period in us
  core::TimerCounter1 &myTimerCounter1 = core::TimerCounter1::getInstance(
      core::channel::B, core::operationMode::normal, core::clockSource::PS_64);

  // Instantiate a PushButton object
  component::PushButton PushButton(io::Pin(PUSHBUTTON,io::PortB));

  // Song notes: the table holds the note periods in us and lives in
  // flash so the song does not consume RAM
  const uint16_t l_song[] PROGMEM = {
  E6, E6, E6, C6, E6, G6, G5,
  C6, G5, E5, A5, B5, Ax5, A5,
  G5, E6, G6, A6, F6, G6, E6, C6, D6, B5,
  C6, G5, E5, A5, B5, Ax5, A5,
  G5, E6, G6, A6, F6, G6, E6, C6, D6, B5,
  };

  // Start song at start
  uint8_t l_note = SONG_LENGTH - 1;

  // Initialize PushButton state
  uint8_t l_statePushButton = 0;


  // Mainloop
  while (1) {
      // Check if PushButton is pressed
      if (PushButton.isPressed())
      {
          // Check if a new button press
          if (!l_statePushButton) {
              // play next note
              l_note++;
              // start from start
              if (l_note == SONG_LENGTH) {

                  l_note = 0;

              }

              l_statePushButton = 1;

          }
          // Play note: tick value = period in us / 4 (4 us per tick at
          // prescaler 64), audibly NOTE_DURATION_MS long
          Buzzer.buzz(myTimerCounter1,
                      pgm_read_word(&l_song[l_note]) / 4,
                      NOTE_DURATION_MS,
                      core::channel::B);

      } else {

          l_statePushButton=0;

      }

  }
  return 0;
}
