function calculatePowerBudget(mcu, voltage_v, activePeripherals, battery_mah) {
    let baseCurrent_mA = 12.0; // ATmega328P active @ 16MHz 5V
    if (mcu === 'atmega2560') baseCurrent_mA = 25.0;
    if (mcu === 'attiny85') baseCurrent_mA = 3.5;

    let totalCurrent_mA = baseCurrent_mA;

    if (activePeripherals.adc) totalCurrent_mA += 0.25;
    if (activePeripherals.usart) totalCurrent_mA += 0.30;
    if (activePeripherals.spi) totalCurrent_mA += 0.40;
    if (activePeripherals.twi) totalCurrent_mA += 0.35;
    if (activePeripherals.leds) totalCurrent_mA += activePeripherals.leds * 15.0; // ~15mA per LED

    const power_mW = totalCurrent_mA * voltage_v;
    const hours = battery_mah / totalCurrent_mA;
    const days = hours / 24.0;

    return {
        current_mA: totalCurrent_mA.toFixed(2),
        power_mW: power_mW.toFixed(2),
        lifespanHours: hours.toFixed(1),
        lifespanDays: days.toFixed(1)
    };
}
