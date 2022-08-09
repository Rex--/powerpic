# PowerPIC Hardware

The PowerPIC is built on a PIC16LF19197 with 56KB of flash memory and 4KB of RAM.
Required features included: a real time clock and calendar module for timekeeping,
a LCD controller for driving the stock Casio display, and a 10-bit PWM for driving
the piezo buzzer. Some additional useful features are: a 12-bit ADC for roughly
determining battery level, and a temperature sensor for roughly gauging temperature.

The only other components besides the required passives are a 32.768kHz crystal,
and a LED that is used for the backlight.


## Changelog

**Version 2.0**
- Added experimental backlight.
- Added RTC crystal for accurate timekeeping.

**Version 1.1**
- Switched buzzer from RF0 to RG7 so we can use the PWM peripheral.
- Switched keypad rows to RA0-RA3. Row 1 was using RA5 which is VBAT and unusable as digital output
- Added additional test points for the side buttons, allowing buttons to be simulated for development
- Added additional GND test point

**Version 1.0**
 - First hardware version
