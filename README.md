# GPIO Control with C++
In this project, I am learning to control GPIO pins using C++.

## Functionality 
The firmware does not provide a command-line interface (CLI).

## Clock configuration
The core clock is configured to the maximum frequency (100 MHz).

## Hardware
- **Board:** Black Pill with an STM32F411CEU6 MCU.

## Firmware behavior
After startup, the LED on PC13 starts blinking.

## Building
Requirements:
- arm-none-eabi-gcc
- arm-none-eabi-g++
- CMake >= 3.22
- Ninja
  ``` bash
  cmake --preset Release
  cmake --build build/Release
  ``` 

## Flashing
Using ST-Link probe and STM32CubeProgrammer:
``` bash
STM32_Programmer_CLI -c port=SWD -w build/Release/GPIO_CPP.elf -v -rst
```