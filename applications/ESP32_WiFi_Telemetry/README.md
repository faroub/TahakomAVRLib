# ESP32 Wi-Fi & Serial Telemetry Application

This application demonstrates C++ application development for the **ESP32** microcontroller using **TahakomAVRLib**.

## Description

The application performs continuous analog light sensing via ADC channel 0, toggles an onboard status LED on GPIO 2, and streams live `$ADC` and `$LOG` telemetry frames to the **TahakomAVR Web IDE** dashboard.

## Hardware Connections

| ESP32 Pin | Component | Description |
|---|---|---|
| **GPIO 2** | Onboard LED | Status Blinker |
| **GPIO 34 (ADC1_CH6)** | LDR Light Sensor | Analog Light Input |
| **GPIO 1 (TX0)** | Serial / USB | Telemetry TX to Web IDE |
| **GPIO 3 (RX0)** | Serial / USB | Telemetry RX from Web IDE |

## Telemetry Framing

The application emits NMEA-style telemetry strings parsed by [server/services/telemetry_service.py](file:///home/faroub/Documents/Projects/TahakomAVRLib/server/services/telemetry_service.py):
- `$ADC,0,<raw_value>`: Continuous ADC channel 0 reading (0 - 1023)
- `$LOG,<message>`: Status events
