#ifndef ADC_H
#define ADC_H
#include "ha_base.h"
#include "Pin.h"

namespace core
{

enum class resolution : uint8_t {
    res_8bit=0,
    res_9bit,
    res_10bit,
    res_11bit,
    res_12bit,
    res_13bit,
    res_14bit,
    res_15bit,
    res_16bit
};

enum class referenceVoltage : uint8_t {
    AREF=0,
    AVCC,
    internal=3
};

static_assert(static_cast<uint8_t>(referenceVoltage::AREF) == 0, "AREF must encode REFS = 0b00");
static_assert(static_cast<uint8_t>(referenceVoltage::AVCC) == 1, "AVCC must encode REFS = 0b01");
static_assert(static_cast<uint8_t>(referenceVoltage::internal) == 3, "internal must encode REFS = 0b11");

enum class clockPrescaler : uint8_t {
     PS_2 = 1,
     PS_4,
     PS_8,
     PS_16,
     PS_32,
     PS_64,
     PS_128
};

enum class autoTriggerSource : uint8_t {
    freeRunning,
    analogComparator,
    extInterrupt,
    timer0CompareA,
    timer0Overflow,
    timer1CompareB,
    timer1Overflow,
    timer1Capture
};

class ADConverter
{
public:
    static ADConverter& getInstance(const referenceVoltage& ar_refVoltage = referenceVoltage::AVCC,
                                    const clockPrescaler& ar_clockPrescaler = clockPrescaler::PS_128,
                                    const autoTriggerSource& ar_autoTriggerSource = autoTriggerSource::freeRunning,
                                    const io::Pin &ar_pin = io::Pin(0,io::PortC));

    void start();
    void stop();
    void selectReferenceVoltage(const referenceVoltage& ar_refVoltage);
    void selectAnalogInput(io::Pin a_pin);
    void selectClockPrescaler(const clockPrescaler& ar_clockPrescaler);
    void enableConversionCompleteInterrupt(const uint8_t a_enable);
    void enableAutoTrigger(const uint8_t a_enable);
    void selectAutoTriggerSource(const autoTriggerSource& ar_autoTriggerSource);

    uint16_t getConversionResult(const resolution& ar_resolution = resolution::res_10bit);
    void resetOversamplingState();

    static void conversionCompleteServiceRoutine() __asm__(STR(ADC_CONVERSION_COMPLETE_INTERRUPT)) __attribute__((__signal__, __used__, __externally_visible__));

private:
    ADConverter(const referenceVoltage& ar_refVoltage,
                const clockPrescaler& ar_clockPrescaler,
                const autoTriggerSource &ar_autoTriggerSource,
                const io::Pin &ar_pin);

    ~ADConverter();
    ADConverter(const ADConverter&);
    const ADConverter& operator=(const ADConverter&);

    static volatile uint16_t m_conversionResult;
    static volatile uint32_t m_accumulatedResult;
    static volatile uint16_t m_sampleIndex;
    static uint8_t m_resolution;
};

}
#endif
