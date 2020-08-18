/**
 * @file ADC.h
 * @brief Header file of the ADC class
 *
 * Basic class for abstraction of the Analog to Digital Converter.
 *
 * Usage example:
 *
  #include "ADC.h"
  #include "USART0.h"

  #define TRANSMIT_BUFFER_SIZE 5


  int main(void) {

  // Init

  // instantiate the USART0 object
  io::USART0 &myUSART0 = io::USART0::getInstance();
  // transmit data buffer
  char l_data2Send[TRANSMIT_BUFFER_SIZE];
  // ready to send flag
  uint8_t l_ready2Send = 0;

  // instantiate the ADC object
  core::ADConverter &myADC = core::ADConverter::getInstance();
  // use 8 bit resolution
  myADC.adjustResult(core::resultAdjust::left);
  // flag to test multiplexing between channel 0 and channel 1
  uint8_t channel = 0;
  // select analog input
  myADC.selectAnalogInput(io::Pin(channel,io::PortC));

  // variable to hold conversion result
  uint16_t l_conversionResult = 0;
  uint16_t l_analogInput[ADC_CHANNEL_MUX_SIZE] = {0,0};

  // enable and start conversion
  myADC.start();

  // Mainloop
  while (1) {

      if (l_ready2Send && !myUSART0.getNumberBytesSent())
      {
          myUSART0.sendFrame(reinterpret_cast<uint8_t*>(l_data2Send),TRANSMIT_BUFFER_SIZE);
          l_ready2Send = 0;
      }

      myADC.getConversionResult(&l_conversionResult);

      if (myADC.conversionComplete())
      {

          l_analogInput[channel] = l_conversionResult;
          l_data2Send[0] = '0' + ((l_analogInput[channel] >> 8) / 100);
          l_data2Send[1] = '0' + (((l_analogInput[channel] >> 8) / 10) % 10);
          l_data2Send[2] = '0' + ((l_analogInput[channel] >> 8) % 10);
          l_data2Send[3] = '\n';
          l_data2Send[4] = '\r';
          // switch between channel 0 and channel 1
          channel = 1 - channel;
          myADC.selectAnalogInput(io::Pin(channel,io::PortC));

          l_ready2Send = 1;
      }


//          uint16_t value = 12345;
//          char lo = value & 0xFF;
//          char hi = value >> 8;
   }
   return 0;
 }

 *
 * @author Farid Oubbati (https://github.com/faroub)
 * @date March 2020
*/
#ifndef ADC_H
#define ADC_H
#include "ha_base.h"
#include "Pin.h"

namespace core
{

enum class referenceVoltage : uint8_t {
    AREF=0,    /**< external AREF pin voltage reference, internal 1.1V voltage reference turned off */
    AVCC,       /**< AVCC voltage reference with external capacitor at AREF pin */
    internal   /**< internal 1.1V voltage reference with external capacitor at AREF pin */
};

enum class resultAdjust : uint8_t {
    left=0,    /**< left adjust the result  */
    right       /**< right adjust the result */
};


enum class clockPrescaler {
     PS_2 = 1,
     PS_4,
     PS_8,
     PS_16,
     PS_32,
     PS_64,
     PS_128
};

enum class autoTriggerSource : uint8_t {
    FreeRunning,
    AnalogComparator,
    ExtInterrupt,
    Timer0Compare,
    Timer0Overflow,
    Timer1CompareB,
    Timer1Overflow,
    Timer1Capture
};

class ADConverter
{
public:

    static ADConverter& getInstance(const referenceVoltage& ar_refVoltage = referenceVoltage::AVCC,
                                    const resultAdjust& ar_adjustResult = resultAdjust::right,
                                    const clockPrescaler& ar_clockPrescaler = clockPrescaler::PS_128,
                                    const autoTriggerSource& ar_autoTriggerSource = autoTriggerSource::FreeRunning,
                                    const io::Pin &ar_pin = io::Pin(0,io::PortC));

    void start();

    void stop();

    void selectReferenceVoltage(const referenceVoltage& ar_refVoltage);

    void adjustResult(const resultAdjust &ar_adjustResult);

    void selectAnalogInput(io::Pin a_pin);

    void selectClockPrescaler(const clockPrescaler& ar_clockPrescaler);

    void getConversionResult(uint16_t *ap_dataBuffer);

    void enableConversionCompleteInterrupt(const uint8_t a_enable);

    void enableAutoTrigger(const uint8_t a_enable);

    void selectAutoTriggerSource(const autoTriggerSource& ar_autoTriggerSource);

    uint8_t conversionComplete();

    static void conversionCompleteServiceRoutine() __asm__(STR(ADC_CONVERSION_COMPLETE_INTERRUPT)) __attribute__((__signal__, __used__, __externally_visible__));


protected:



private:

    ADConverter(const referenceVoltage& ar_refVoltage,
                const resultAdjust& ar_adjustResult,
                const clockPrescaler& ar_clockPrescaler,
                const autoTriggerSource &ar_autoTriggerSource,
                const io::Pin &ar_pin);

    /** Destructor.
        */
    ~ADConverter();

    /** Overried Copy constructor.
        */
    ADConverter(const ADConverter&);

    /** Override assign operator.
        */
    const ADConverter& operator=(const ADConverter&);


    static volatile uint16_t *mp_conversionResult; /**< pointer to receiver buffer */


    static uint8_t m_conversionComplete; /**< ready to receive flag */






};





}
#endif
