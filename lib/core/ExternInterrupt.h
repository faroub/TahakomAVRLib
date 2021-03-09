/**
 * @file ExternInterrupt.h
 * @brief Header file of the ExternInterrupt class
 *
 * class to handle externally triggered interrupts.
 *
 * Usage example (external interrupt):
 *
   #include "PushButton.h"
   #include "Led.h"
   #include "ExternInterrupt.h"

   #define PUSHBUTTON_NUMBER 2
   #define MAIN_LED_NUMBER 0
   #define INTERRUPT_LED_NUMBER 1
   #define DELAYTIME 1000



   // instantiate a Led object
   // declaration of global variables (shut up warning)
   extern component::Led MainLed;
   extern component::Led InterruptLed;
   component::Led MainLed(io::Pin(MAIN_LED_NUMBER,io::PortB));
   component::Led InterruptLed(io::Pin(INTERRUPT_LED_NUMBER,io::PortB));

   // instantiate a Led object
   // declaration of global variable (shut up warning)
   extern component::PushButton PushButton;
   component::PushButton PushButton(io::Pin(PUSHBUTTON_NUMBER,io::PortD));

   int main(void) {

   // Init

   // instantiate the external interrupt manager
   core::ExternInterrupt &myExternInterrupt = core::ExternInterrupt::getInstance();
   myExternInterrupt.enableInt0(1);
   myExternInterrupt.setInt0SenseControl(core::senseControl::logicalChange);

   // Mainloop
   while (1) {

      MainLed.toggle();
      _delay_ms(DELAYTIME);

   }
   return 0;
   }

   void core::ExternInterrupt::Int0ServiceRoutine()
   {
        if (PushButton.isPressed())
        {
            InterruptLed.on();
        } else {
            InterruptLed.off();
        }

   }
 *
 * Usage example (pin change interrupt):
 *
   #include "PushButton.h"
   #include "Led.h"
   #include "ExternInterrupt.h"

   #define PUSHBUTTON_1_NUMBER 2
   #define PUSHBUTTON_2_NUMBER 3
   #define MAIN_LED_NUMBER 0
   #define INTERRUPT_LED_1_NUMBER 1
   #define INTERRUPT_LED_2_NUMBER 2
   #define DELAYTIME 1000



   // instantiate a Led objects
   // declaration of global variables (shut up warning)
   extern component::Led MainLed;
   extern component::Led InterruptLed1;
   extern component::Led InterruptLed2;
   component::Led MainLed(io::Pin(MAIN_LED_NUMBER,io::PortB));
   component::Led InterruptLed1(io::Pin(INTERRUPT_LED_1_NUMBER,io::PortB));
   component::Led InterruptLed2(io::Pin(INTERRUPT_LED_2_NUMBER,io::PortB));


   // instantiate a PushButton objects
   // declaration of global variable (shut up warning)
   extern component::PushButton PushButton1;
   extern component::PushButton PushButton2;
   component::PushButton PushButton1(io::Pin(PUSHBUTTON_1_NUMBER,io::PortD));
   component::PushButton PushButton2(io::Pin(PUSHBUTTON_2_NUMBER,io::PortB));

   int main(void) {

   // Init

   // instantiate the external interrupt manager
   core::ExternInterrupt &myExternInterrupt = core::ExternInterrupt::getInstance();
   myExternInterrupt.enablePinChange(core::pinChangePort::PCINTD,1);
   myExternInterrupt.enablePinChange(core::pinChangePort::PCINTB,1);
   myExternInterrupt.enablePinChangeMaskPortD(PUSHBUTTON_1_NUMBER,1);
   myExternInterrupt.enablePinChangeMaskPortB(PUSHBUTTON_2_NUMBER,1);

   // Mainloop
   while (1) {

      MainLed.toggle();
      _delay_ms(DELAYTIME);

   }
   return 0;
   }

   void core::ExternInterrupt::pinChangePortDServiceRoutine()
   {
        if (PushButton1.isPressed())
        {
            InterruptLed1.on();

        } else {
            InterruptLed1.off();

        }
   }
   void core::ExternInterrupt::pinChangePortBServiceRoutine()
   {
        if (PushButton2.isPressed())
        {
            InterruptLed2.on();
        } else {
            InterruptLed2.off();
        }

   }
 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/
#ifndef EXTERN_INTERRUPTS_H
#define EXTERN_INTERRUPTS_H
#include "ha_base.h"



namespace core
{

enum class senseControl : uint8_t {
    lowLevel=0,    /**<  */
    logicalChange,       /**<  */
    fallingEdge,   /**<  */
    risingEdge   /**<  */
};

enum class pinChangePort : uint8_t {
    PCINTB=0,    /**<  */
    PCINTC,       /**<  */
    PCINTD,   /**<  */
};

class ExternInterrupt
{

public:

    static ExternInterrupt& getInstance();

    void setInt0SenseControl(const senseControl& ar_senseControl);

    void setInt1SenseControl(const senseControl& ar_senseControl);

    void enableInt0(const uint8_t a_enable);

    void enableInt1(const uint8_t a_enable);

    void enablePinChange(const pinChangePort& ar_pinChangePort, const uint8_t a_enable);

    void enablePinChangeMaskPortB(const uint8_t a_pinNumber, const uint8_t a_enable);

    void enablePinChangeMaskPortC(const uint8_t a_pinNumber, const uint8_t a_enable);

    void enablePinChangeMaskPortD(const uint8_t a_pinNumber, const uint8_t a_enable);

    static void Int0ServiceRoutine() __asm__(STR(EXT_INT_INT0_INTERRUPT)) __attribute__((__signal__, __used__, __externally_visible__));

    static void Int1ServiceRoutine() __asm__(STR(EXT_INT_INT1_INTERRUPT)) __attribute__((__signal__, __used__, __externally_visible__));

    static void pinChangePortBServiceRoutine() __asm__(STR(EXT_INT_PIN_CHANGE_PORTB_INTERRUPT)) __attribute__((__signal__, __used__, __externally_visible__));

    static void pinChangePortCServiceRoutine() __asm__(STR(EXT_INT_PIN_CHANGE_PORTC_INTERRUPT)) __attribute__((__signal__, __used__, __externally_visible__));

    static void pinChangePortDServiceRoutine() __asm__(STR(EXT_INT_PIN_CHANGE_PORTD_INTERRUPT)) __attribute__((__signal__, __used__, __externally_visible__));


protected:

private:

    ExternInterrupt();

    /** Destructor.
        */
    ~ExternInterrupt();

    /** Overried Copy constructor.
        */
    ExternInterrupt(const ExternInterrupt&);

    /** Override assign operator.
        */
    const ExternInterrupt& operator=(const ExternInterrupt&);

};
}




#endif
