# PowerPIC Calculator Watch

The PowerPIC is a replacement board for Casio CA-53W Databank watches.
Functionality aims to mimic a stock watch's exactly, with the only addition being
a backlight button.

## [Firmware](firmware/)
Firmware for the watch is written in C and compiled with Microchip's `xc8` compiler.
Functionality is separated into 'mode' applications, with each mode implementing
a set of procedures to be run when active. The active mode is changed by the Mode
side button, with each press cycling through the list of modes.

## [Hardware](hardware/)
The PowerPIC is built on a PIC16LF19197 with 56KB of flash memory and 4KB of RAM.
Required features included: a real time clock and calendar module for timekeeping,
a LCD controller for driving the stock Casio display, and a 10-bit PWM for driving
the piezo buzzer. Some additional useful features are: a 12-bit ADC for roughly
determing battery level, and a temperature sensor for roughly gauging temperature.

The only other componenets besides the required passives are a 32.768kHz crystal,
and a LED that is used for the backlight.
