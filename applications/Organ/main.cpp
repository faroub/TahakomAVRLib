/**
 * @file main.cpp
 *
 * Demo example: AVR Square-Wave Organ
 *
 * Usage demonstration of the TahakomAVRLib in a Buzzer based and serial driven Square-Wave Organ.
 * The example enables the user to enter a key note via serial terminal from the keyboard and play the corresponding note in a Buzzer
 * This example demonstrates the use of the component::Buzzer() and io::USART0() abstraction objects.
 *
 * - Compiler: avr-gcc (GCC) 5.4.0
 * - Supported devices:  The example compiles on the ATmega48P/88P/168P/328P AVR family
 *
 *
 * @author Farid Oubbati (farid.oubbati@outlook.com)
 * @date March 2018
*/

#include "Buzzer.h"
#include "USART0.h"
// Note pitch data
#include "buzzer_pitches_16bit.h"


#define BUZZER 0 /**< Buzzer pin number */

#define NOTE_DURATION  0xF000 /**< Note duration */

int main(void) {

  // Init

  // Instantiate a Buzzer object
  component::Buzzer Buzzer(io::Pin(BUZZER,io::PortB));

  // Instantiate a USART0 object
  io::USART0 &myUSART0 = io::USART0::getInstance();

  // List of possible keypresses
  const uint8_t l_keys[] = { 'a', 'w', 's', 'e', 'd', 'f', 't',
      'g', 'y', 'h', 'j', 'i', 'k', 'o',
      'l', 'p', ';', '\''
  };

  // List of the keypresses corresponding pitches
  const uint16_t l_notes[] = { G4, Gx4, A4, Ax4, B4, C5, Cx5,
      D5, Dx5, E5, F5, Fx5, G5, Gx5,
      A5, Ax5, B5, C6
  };

  // Character key from computer serial terminal
  unsigned char l_key;

  // Current note length
  uint16_t l_currentNoteLength = NOTE_DURATION;

  // Check if note key
  uint8_t l_isNote;

  myUSART0.sendString("----- Serial Organ ------\r\n");

  // Mainloop
  while (1) {

      // Send N to signal that AVR chip is ready for next key note input
      myUSART0.sendString("Enter key note\r\n");

      // Wait for key note input
      while (myUSART0.getNumberBytesReceived()==0)
      {
              myUSART0.receiveChar(l_key);

      }
      myUSART0.resetNumberBytesReceived();

      /********** Play key notes ***********/

      l_isNote = 0;
      // Loop through keys table
      for (uint8_t i = 0; i < sizeof(l_keys); i++) {
          // Found match in lookup table
          if (l_key == l_keys[i]) {
              Buzzer.buzz(l_notes[i], l_currentNoteLength);
              // Record that we've found a note
              l_isNote = 1;
              break;
          }
      }

      // Handle non-note keys: tempo changes and rests
      if (!l_isNote) {
          if (l_key == '-') {
              // Code for short note
              l_currentNoteLength = l_currentNoteLength / 2;
          }
          else if (l_key == '+') {
              // Code for long note
              l_currentNoteLength = l_currentNoteLength * 2;
          }
          else {
              // Unrecognized, just rest
              Buzzer.pause(l_currentNoteLength);
          }
      }

  }
  return 0;
}
