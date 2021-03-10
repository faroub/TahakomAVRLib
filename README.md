## Introduction

TahakomAVRLib is a C++ library to program AVR microcontrollers. 
Th library is written for easy usage and is, for now, limited 
to the ATmega48P/88P/168P/328P AVR microcontrollers family. 

## Software setup  

 Before using the library and start programming and interfacing hardware peripherals, some 
 software packages need to be installed in your system: 

* binutils:for getting tools like assembler, linker
* gcc-avr : a GNU C cross-compiler for specifically for AVR
* avr-libc: a package for AVR C library
* avrdude : is a driver program for downloading code and data to Atmel AVR microcontrollers

According to your operating system, these software packages can be installed as follows:

### Linux

```
sudo apt-get update
sudo apt-get install gcc build-essential
sudo apt-get install gcc-avr binutils-avr avr-libc gdb-avr
sudo apt-get install avrdude
```


### Windows





## Hardware setup  

* MCU chip either barebone or on a development board like an Arduino UNO
* A bunch of Leds, resistors, drivers, sensors and actuators
* A breadboard
* Jumper wires
* An AVR ISPProgrammer (optional)


## Applications

These applications demonstrate the usage of TahakomAVRLib in simple examples:

* [Blink a Led](TahakomAVRLib/applications/BlinkLed)


## Projects

These are relatively complex projects implemented using TahakomAVRLib


## Author

* Farid Oubbati
* Date: 12-May-2018
* Copyright (c) 2018

## License

This project is licensed under the MIT License - see the [LICENSE.txt](LICENSE.txt) file for more details
