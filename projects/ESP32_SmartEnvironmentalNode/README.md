# ESP32 Smart Environmental Sensing Node

Complete hardware project for the **ESP32** microcontroller built with **TahakomAVRLib**.

## Overview

The project integrates an SSD1306 I2C OLED display, LDR light sensor, Quadrature Rotary Encoder UI, PID velocity control loop, and telemetry streamer on an ESP32 board.

## Hardware Components

1. **ESP32 Board** (NodeMCU / ESP32 DevKit)
2. **SSD1306 I2C OLED Display** (128x64 pixels)
3. **LDR Light Sensor** with 5k Ohm resistor
4. **Rotary Encoder** (A/B quadrature signals)
5. **Status LED**
6. **UART Telemetry Interface** to Web IDE

## Pinout Connections

| ESP32 Pin | Component | Function |
|---|---|---|
| **GPIO 2** | Status LED | Blinker Indicator |
| **GPIO 21 (SDA)** | SSD1306 OLED SDA | I2C Data Line |
| **GPIO 22 (SCL)** | SSD1306 OLED SCL | I2C Clock Line |
| **GPIO 34 (ADC1_CH6)** | LDR Light Sensor | Analog Light Reading |
| **GPIO 4** | Rotary Encoder A | Quadrature Phase A |
| **GPIO 5** | Rotary Encoder B | Quadrature Phase B |
| **GPIO 1 (TX0)** | Serial USB | Telemetry Stream Output |
