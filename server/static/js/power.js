/**
 * Power Consumption & Battery Lifespan Estimator
 *
 * NOTE: Estimates are theoretical models based on Microchip/Atmel datasheet nominal values.
 * Real hardware current draw varies with operating temperature, manufacturing tolerances,
 * pin output impedance, and voltage regulator efficiency.
 */

function calculatePowerBudget(mcu, voltage_v = 5.0, activePeripherals = {}, battery_mah = 220) {
    // Validate inputs
    voltage_v = Math.max(1.8, Math.min(5.5, Number(voltage_v) || 5.0));
    battery_mah = Math.max(1, Number(battery_mah) || 220);

    // Datasheet active current baselines @ 16 MHz 5V
    let baseCurrent_mA = 12.0; // ATmega328P (~12 mA @ 16 MHz, 5V active)
    if (mcu === 'atmega2560') baseCurrent_mA = 25.0; // ATmega2560 (~25 mA @ 16 MHz, 5V active)
    if (mcu === 'attiny85') baseCurrent_mA = 3.5;    // ATtiny85 (~3.5 mA @ 8 MHz, 5V active)

    // Adjust baseline for operating voltage (approximate linear scale)
    baseCurrent_mA = baseCurrent_mA * (voltage_v / 5.0);

    let totalCurrent_mA = baseCurrent_mA;

    // Peripheral Current Draw Additions (Datasheet Typical Values)
    if (activePeripherals.adc) totalCurrent_mA += 0.25;      // ADC active: ~250 uA
    if (activePeripherals.usart) totalCurrent_mA += 0.30;    // USART0 active: ~300 uA
    if (activePeripherals.spi) totalCurrent_mA += 0.40;      // SPI active: ~400 uA
    if (activePeripherals.twi) totalCurrent_mA += 0.35;      // TWI/I2C active: ~350 uA
    if (activePeripherals.leds) {
        const count = Math.max(0, Number(activePeripherals.leds) || 0);
        totalCurrent_mA += count * 15.0;                      // Standard LED: ~15 mA @ 220 Ohm resistor
    }

    const power_mW = totalCurrent_mA * voltage_v;
    const hours = battery_mah / totalCurrent_mA;
    const days = hours / 24.0;

    return {
        current_mA: totalCurrent_mA.toFixed(2),
        power_mW: power_mW.toFixed(2),
        lifespanHours: hours.toFixed(1),
        lifespanDays: days.toFixed(1),
        disclaimer: "Estimated typical value. Physical measurement with multimeter recommended."
    };
}
