# Introduction

TahakomAVRLib is a C++ library to program AVR microcontrollers. 
Th library is written for easy usage and is, for now, limited 
to the ATmega48P/88P/168P/328P AVR microcontrollers family. 

# Software setup  

 Before using the library and start programming and interfacing hardware peripherals, some 
 software packages need to be installed in your system: 

* binutils:for getting tools like assembler, linker
* gcc-avr : a GNU C cross-compiler for specifically for AVR
* avr-libc: a package for AVR C library
* avrdude : is a driver program for downloading code and data to Atmel AVR microcontrollers

To install these software packages according to your operating system:

## Linux

```
sudo apt-get update
sudo apt-get install gcc build-essential
sudo apt-get install gcc-avr binutils-avr avr-libc gdb-avr
$sudo apt-get install avrdude
```


## Windows



<img src="/pics/simulation-scene.png" alt="alt text" width="570">

Video of the simulation can be seen seen [here](https://youtu.be/OncJcgg6dec)

## The software tools used:

* Matlab 8.5.0.197613 (R2015a)
* V-REP PRO EDU version 3.5.0

## The simulation robot used:

* ePuck

## To run the simulation:

1. Start the V-REP simulator and open the simulation scene (epuck-dyn-obs-tar.ttt) provided in /src-vrep
2. Start Matlab and change to simulation files folder or add its path
3. Start simulation with: 

```
>> moveEpuck
```
runs the simulation without plotting the dynamics of the heading direction

```
>> moveEpuck(1)
```
runs the simulation and plots the dynamics of the heading direction

<img src="/pics/dynamics-plot.png" alt="alt text" width="370">

4. If necessary, change the parameters of the dynamics and re-run the simulation

## Author

* Farid Oubbati
* Date: 12-May-2017
* Copyright (c) 2017

## License

This project is licensed under the MIT License - see the [LICENSE.txt](LICENSE.txt) file for more details
