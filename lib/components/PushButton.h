/**
 * @file PushButton.h
 * @brief Header file of the Push Button class
 *
 * class to monitor a Push Button
 *
 * Usage example (current state):
 *
   #include "PushButton.h"
   #include "Led.h"

   #define PUSHBUTTON_NUMBER 1
   #define LED_NUMBER 0

   int main(void) {

   // Init


   // instantiate a Led object
   component::Led Led(io::Pin(LED_NUMBER,io::PortB));

   // instantiate a Led object
   component::PushButton PushButton(io::Pin(PUSHBUTTON_NUMBER,io::PortB));

   // Mainloop
   while (1) {

      if (PushButton.isPressed())
      {
          Led.on();
      } else {
          Led.off();
      }


   }
   return 0;
   }
 *
 * Usage example (changing state):
 *
   #include "PushButton.h"
   #include "Led.h"

   #define PUSHBUTTON_NUMBER 1
   #define LED_NUMBER 0

   int main(void) {

   // Init

   uint8_t l_statePushButton = 0;

   // instantiate a Led object
   component::Led Led(io::Pin(LED_NUMBER,io::PortB));

   // instantiate a Led object
   component::PushButton PushButton(io::Pin(PUSHBUTTON_NUMBER,io::PortB));





   // Mainloop
   while (1) {

      if (PushButton.isPressed())
      {
          if (l_statePushButton == 0) {

              Led.toggle();
              l_statePushButton = 1;
          }
      } else {
          l_statePushButton = 0;
      }

   }
   return 0;
   }
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/
#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H
#include "Pin.h"

// TODO: check the use PUSHBUTTON_SAMPLING in ha_m328p.h


namespace component
{
class PushButton
{    
public:
    /** Constructor. Initializes pushbutton object
         *
         *  @param ar_pin pin object
         *  @param ar_useInternalPullUp indicates if internal pull up resistor used

         */
    PushButton(const io::Pin &ar_pin, const uint8_t &ar_useInternalPullUp=1, const uint8_t &ar_isActiveLow=1);
    /** Destructor.
        */
    ~PushButton();
    /** Is pushbutton pressed.
         */
    uint8_t isPressed();
    /** Get pushbutton pressed count.
         */
    uint8_t getPressedCount() const;
    /** Reset pushbutton pressed count.
         */
    void resetPressedCount();


protected:

private:
    io::Pin m_pin; /**< pin object */
    const uint8_t &mr_isActiveLow; /**< indicates led active state */
    const uint8_t &mr_useInternalPullUp; /**< indicates if internal pullup resistor used  */
    uint8_t m_buttonPressed;    /**< pushbutton pressed count */



};
}




#endif
