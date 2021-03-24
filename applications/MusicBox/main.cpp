/**
 * @file main.cpp
 *
 * Demo example: AVR Music Box
 *
 * Usage demonstration of the TahakomAVRLib in an AVR based Music Box.
 * The example allows the user to play a predefined song in a Buzzer and control its tempo via a PushButton
 * This example demonstrates the use of the component::Buzzer() and component::PushButton() abstraction objects.
 *
 * - Compiler: avr-gcc (GCC) 5.4.0
 * - Supported devices:  The example compiles on the ATmega48P/88P/168P/328P AVR family
 *
 *
 * @author Farid Oubbati (farid.oubbati@outlook.com)
 * @date March 2018
*/

#include "Buzzer.h"
#include "PushButton.h"
// Note pitch data
#include "buzzer_pitches_16bit.h"


#define BUZZER 0 /**< Buzzer pin number */

#define PUSHBUTTON 1 /**< PushButton pin number */

#define SONG_LENGTH (sizeof(l_song) / sizeof(uint16_t)) /**< Song length */

int main(void) {

  // Init

  // Instantiate a Buzzer object
  component::Buzzer Buzzer(io::Pin(BUZZER,io::PortB));

  // Instantiate a PushButton object
  component::PushButton PushButton(io::Pin(PUSHBUTTON,io::PortB));

  // Song notes
  const uint16_t l_song[] = {
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
      // Check if PusgButton is pressed
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
          // Play note
          Buzzer.buzz(l_song[l_note], 1600);

      } else {

          l_statePushButton=0;

      }

  }
  return 0;
}
