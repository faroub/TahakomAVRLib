#ifndef ADC_H
#define ADC_H
#include "ha_base.h"
#include "Pin.h"

/**
 *
 * @brief AVR chip internal core components
 *
 * The namespace englobes all internal core components that, when configured,
 * allows the AVR MCU to internally process data
 *
*/
namespace core
{
/**
 * @brief ADC resolution
 *
 * @enum core::resolution
 *
 * @author Farid Oubbati (farid.oubbati@outlook.com)
 * @date March 2018
*/
enum class resolution : uint8_t {
    res_8bit=0, /**< 8 bits resolution */
    res_9bit,   /**< 9 bits resolution */
    res_10bit,  /**< 10 bits resolution */
    res_11bit,  /**< 11 bits resolution */
    res_12bit,  /**< 12 bits resolution */
    res_13bit,  /**< 13 bits resolution */
    res_14bit,  /**< 14 bits resolution */
    res_15bit,  /**< 15 bits resolution */
    res_16bit   /**< 16 bits resolution */
};

/**
 * @brief ADC referencev voltage
 *
 * @enum core::referenceVoltage
 *
 * @author Farid Oubbati (farid.oubbati@outlook.com)
 * @date March 2018
*/
enum class referenceVoltage : uint8_t {
    AREF=0,    /**< external AREF pin voltage reference, internal 1.1V voltage reference turned off */
    AVCC,       /**< AVCC voltage reference with external capacitor at AREF pin */
    internal   /**< internal 1.1V voltage reference with external capacitor at AREF pin */
};


/**
 * @brief ADC clock prescaler
 *
 * @enum core::clockPrescaler
 *
 * @author Farid Oubbati (farid.oubbati@outlook.com)
 * @date March 2018
*/
enum class clockPrescaler : uint8_t {
     PS_2 = 1,  /**< prescaler /2 */
     PS_4,      /**< prescaler /4 */
     PS_8,      /**< prescaler /8 */
     PS_16,     /**< prescaler /16 */
     PS_32,     /**< prescaler /32 */
     PS_64,     /**< prescaler /64 */
     PS_128     /**< prescaler /128 */
};
/**
 * @brief ADC autoTrigger source
 *
 * @enum core::autoTriggerSource
 *
 * @author Farid Oubbati (farid.oubbati@outlook.com)
 * @date March 2018
*/
enum class autoTriggerSource : uint8_t {
    freeRunning,    /**< free running trigger mode */
    analogComparator, /**< analog comparator atrigger mode */
    extInterrupt, /**< external interrupt trigger mode */
    timer0CompareA, /**< timer0 compare A trigger mode */
    timer0Overflow, /**< timer0 overflow trigger mode */
    timer1CompareB, /**< timer0 compare B trigger mode */
    timer1Overflow, /**< timer1 overflow trigger mode */
    timer1Capture /**< timer1 capture trigger mode */
};
/**
 * @brief Class for handling ADC
 *
 * The class implements ADC operations
 *
 * @author Farid Oubbati (farid.oubbati@outlook.com)
 * @date March 2018
*/
class ADConverter
{
public:
    /**
     *  Create a single instance of the ADC object
     *
     *  @param ar_refVoltage Defines reference voltage
     *  @param ar_clockPrescaler Defines clock prescaler
     *  @param ar_autoTriggerSource Defines auto trigger source
     *  @param ar_pin Defines a Pin object
     */
    static ADConverter& getInstance(const referenceVoltage& ar_refVoltage = referenceVoltage::AVCC,
                                    const clockPrescaler& ar_clockPrescaler = clockPrescaler::PS_128,
                                    const autoTriggerSource& ar_autoTriggerSource = autoTriggerSource::freeRunning,
                                    const io::Pin &ar_pin = io::Pin(0,io::PortC));
    /** Start ADC conversion
         */
    void start();
    /** Stop ADC conversion
         */
    void stop();
    /** Select reference voltage
     *
     *  @param ar_refVoltage Defines reference voltage
     */
    void selectReferenceVoltage(const referenceVoltage& ar_refVoltage);
    /** Select analog input
     *
     *  @param a_pin Defines Pin object
     */
    void selectAnalogInput(io::Pin a_pin);
    /** Select clock prescaler
     *
     *  @param ar_clockPrescaler Defines clock prescaler
     */
    void selectClockPrescaler(const clockPrescaler& ar_clockPrescaler);
    /** Enable conversion complete interrupt
     *
     *  @param a_enable Defines enable flag
     */
    void enableConversionCompleteInterrupt(const uint8_t a_enable);
    /** Enable autotrigger mode
     *
     *  @param a_enable Defines enable flag
     */
    void enableAutoTrigger(const uint8_t a_enable);
    /** Select autotrigger source
     *
     *  @param ar_autoTriggerSource Defines autotrigger source
     */
    void selectAutoTriggerSource(const autoTriggerSource& ar_autoTriggerSource);
    /**
     * Get ADC conversion result
     *
     * @return  Conversion result
     */
    uint16_t getConversionResult(const resolution& ar_resolution = resolution::res_10bit);
    /** Conversion complete ISR
         */
    static void conversionCompleteServiceRoutine() __asm__(STR(ADC_CONVERSION_COMPLETE_INTERRUPT)) __attribute__((__signal__, __used__, __externally_visible__));


protected:



private:
    /** Constructor.
     *
     *  Initalizes the ADC object
     *
     *  @param ar_refVoltage Defines reference voltage
     *  @param ar_clockPrescaler Defines clock prescaler
     *  @param ar_autoTriggerSource Defines auto trigger source
     *  @param ar_pin Defines a Pin object
     */
    ADConverter(const referenceVoltage& ar_refVoltage,
                const clockPrescaler& ar_clockPrescaler,
                const autoTriggerSource &ar_autoTriggerSource,
                const io::Pin &ar_pin);

    /** Destructor
        */
    ~ADConverter();

    /** Overried Copy constructor
        */
    ADConverter(const ADConverter&);

    /** Override assign operator
        */
    const ADConverter& operator=(const ADConverter&);

    static volatile uint16_t m_conversionResult; /**< Conversion result */

    static uint8_t m_resolution; /**< Conversion resolution */

};

}
#endif
