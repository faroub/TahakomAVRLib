# LCD Environment Light Monitor Application

This application monitors ambient light lux using an LDR light sensor and renders values on a character LCD1602 via a PCF8574 I2C backpack.

## Hardware Connections

* LCD I2C Address: `0x27` (SDA = PC4, SCL = PC5)
* LDR Sensor: PC0
