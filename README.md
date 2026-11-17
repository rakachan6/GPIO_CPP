# UART-Controlled LED Blinker with C++
In this project, I am learning to control GPIO pins and UART interface using C++.

## Functionality 
The firmware supports command-line interface (CLI).

## UART commands

Connect a 3.3 V USB-to-UART adapter to USART1:
- Adapter TX → PA10 (USART1 RX)
- Adapter RX → PA9 (USART1 TX)
- Adapter GND → board GND

Open a serial terminal at 115200 baud, 8N1, with no flow control.
Send one command at a time. Send `?` to check the current settings.

| Command | Action |
| --- | --- |
| `2` | Start flash blinking|
| `1` | Start steady blinking. |
| `0` | Stop blinking and turn the LED off. |

## Clock configuration
The core clock is configured to the maximum frequency (100 MHz).

## Hardware
- **Board:** Black Pill with an STM32F411CEU6 MCU.
- **USB to UART controller:** FT-232.  

## Firmware behavior
On startup, the LED on PC13 begins blinking, toggling every 500 ms

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