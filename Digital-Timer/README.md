# Digital Timer Using PIC18F4580

## Overview
This project implements a digital timer and clock application using the PIC18F4580 microcontroller. It interfaces with a DS1307 Real-Time Clock (RTC), a character LCD, a matrix keypad, and external EEPROM using embedded C.

The firmware includes time and date display, time-setting functionality, menu navigation, and alarm management with persistent storage.

## Features
- Display current time and date using the DS1307 RTC
- Set time and date through a menu interface
- Navigate the interface using a matrix keypad
- Display information on a character LCD
- Store alarm configurations in external EEPROM
- Support one-time, daily, and weekly alarm configurations
- Configure and manage up to eight alarm entries
- Validate stored alarm data when loading it from EEPROM

## Hardware and Technologies
- PIC18F4580 microcontroller
- DS1307 Real-Time Clock
- Character LCD (CLCD)
- Matrix keypad
- External EEPROM
- I2C communication
- Embedded C
- MPLAB X IDE
- XC8 compiler

## Project Structure
- `main.c` — Main application, display logic, menu handling, and alarm management
- `main.h` — Main application definitions
- `clcd.c` and `clcd.h` — Character LCD interface
- `ds1307.c` and `ds1307.h` — RTC interface
- `i2c.c` and `i2c.h` — I2C communication
- `external_eeprom.c` and `external_eeprom.h` — External EEPROM interface
- `matrix_keypad.c` and `matrix_keypad.h` — Matrix keypad interface
- `nbproject/` — MPLAB X project configuration

## Development Environment
- MPLAB X IDE
- XC8 C Compiler
- PIC18F4580
- Configured oscillator frequency: 20 MHz, as defined in `main.h`

## How to Build
1. Open the `DIGITAL_TIMER.X` project in MPLAB X IDE.
2. Verify the device selection, XC8 compiler configuration, and oscillator settings.
3. Check the hardware connections for the RTC, LCD, keypad, and external EEPROM.
4. Build the project.
5. Program the generated firmware onto the PIC18F4580 using a compatible programmer.

## Concepts Demonstrated
- Microcontroller peripheral interfacing
- Embedded C programming
- I2C communication
- Real-time clock interfacing
- Character LCD interfacing
- Matrix keypad scanning
- External EEPROM read/write operations
- Menu-driven firmware design
- Persistent data storage
- Input validation

## Author
Chinmayi H K
