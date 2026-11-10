# Hardware Telemetry Streamer Application

This application streams framed hardware sensor packets (`$ADC,...`, `$PIN,...`, `$LOG,...`) over USART0 to the TahakomAVR Web IDE Oscilloscope and Logic Analyzer dashboard.

## Hardware Connections

* Serial UART: PD0 (RX), PD1 (TX)
* Analog Sensor: PC0
* Digital Button: PD2
