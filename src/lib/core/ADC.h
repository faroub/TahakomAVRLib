/**
 * @file ADC.h
 * @brief Header file of the ADC class
 *
 * Basic class for abstraction of the Analog to Digital Converter.
 *
 * Usage example:
   #include "ADC.h"
   #include "USART0.h"




   #define TRANSMIT_BUFFER_SIZE 7



   int main(void) {

    // Init


    // instantiate the USART0 object
    io::USART0 &myUSART0 = io::USART0::getInstance();
    // transmit data buffer
    char l_data2Send[TRANSMIT_BUFFER_SIZE];

    // instantiate the ADC object
    core::ADConverter &myADC = core::ADConverter::getInstance();


    // select analog input
    myADC.selectAnalogInput(io::Pin(0,io::PortC));

    // variable to hold conversion result
    uint16_t l_conversionResult = 0;

    // enable and start conversion
    myADC.start();

    // ------ Event loop ------ //
    while (1) {



       myADC.getConversionResult(&l_conversionResult, core::resolution::RES_16bit);

       if (myADC.conversionComplete())
       {
           l_data2Send[0] = '0' + (l_conversionResult / 10000);
           l_data2Send[1] = '0' + ((l_conversionResult / 1000) % 10);
           l_data2Send[2] = '0' + ((l_conversionResult / 100) % 10);
           l_data2Send[3] = '0' + ((l_conversionResult / 10) % 10);
           l_data2Send[4] = '0' + (l_conversionResult % 10);
           l_data2Send[5] = '\n';
           l_data2Send[6] = '\r';

           if (myUSART0.ready2Send())
           {
               myUSART0.sendFrame(reinterpret_cast<uint8_t*>(l_data2Send),TRANSMIT_BUFFER_SIZE);
           }

       }
    }
    return 0;
    }
 *
 *
 *
 *
 //          uint16_t value = 12345;
 //          char lo = value & 0xFF;
 //          char hi = value >> 8;
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

enum class resolution : uint8_t {
    RES_8bit=0,
    RES_9bit,
    RES_10bit,
    RES_11bit,
    RES_12bit,
    RES_13bit,
    RES_14bit,
    RES_15bit,
    RES_16bit
};


enum class referenceVoltage : uint8_t {
    AREF=0,    /**< external AREF pin voltage reference, internal 1.1V voltage reference turned off */
    AVCC,       /**< AVCC voltage reference with external capacitor at AREF pin */
    internal   /**< internal 1.1V voltage reference with external capacitor at AREF pin */
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
                                    const clockPrescaler& ar_clockPrescaler = clockPrescaler::PS_128,
                                    const autoTriggerSource& ar_autoTriggerSource = autoTriggerSource::FreeRunning,
                                    const io::Pin &ar_pin = io::Pin(0,io::PortC));

    void start();

    void stop();

    void selectReferenceVoltage(const referenceVoltage& ar_refVoltage);

    void selectAnalogInput(io::Pin a_pin);

    void selectClockPrescaler(const clockPrescaler& ar_clockPrescaler);

    void enableConversionCompleteInterrupt(const uint8_t a_enable);

    void enableAutoTrigger(const uint8_t a_enable);

    void selectAutoTriggerSource(const autoTriggerSource& ar_autoTriggerSource);

    uint8_t conversionComplete();


    void getConversionResult(uint16_t *ap_resultData, const resolution& ar_resolution = resolution::RES_10bit);


    static void conversionCompleteServiceRoutine() __asm__(STR(ADC_CONVERSION_COMPLETE_INTERRUPT)) __attribute__((__signal__, __used__, __externally_visible__));


protected:



private:

    ADConverter(const referenceVoltage& ar_refVoltage,
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


    static uint8_t m_resolution; /**< pointer to receiver buffer */

    static volatile uint8_t m_conversionComplete; /**< ready to receive flag */






};





}
#endif
