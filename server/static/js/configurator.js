function generatePinoutConfig(pin, type) {
    if (type === 'LED') {
        return `component::Led myLed(io::Pin(${pin}, io::PortB));\n`;
    } else if (type === 'BUTTON') {
        return `component::PushButton myButton(io::Pin(${pin}, io::PortD), 1, 1);\n`;
    } else if (type === 'ADC') {
        return `core::ADConverter &myADC = core::ADConverter::getInstance(core::referenceVoltage::AVCC, core::clockPrescaler::PS_128, core::autoTriggerSource::freeRunning, io::Pin(${pin}, io::PortC));\n`;
    }
    return '';
}
