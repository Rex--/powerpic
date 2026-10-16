# PowerPIC Hardware

The PowerPIC is built on a PIC16LF19197 with 56KB of flash memory and 4KB of RAM.
Required features included: a real time clock and calendar module for timekeeping,
a LCD controller for driving the stock Casio display, and a 10-bit PWM for driving
the piezoelectric buzzer. Some additional useful features are: a 12-bit ADC for roughly
determining battery level, and a temperature sensor for roughly gauging temperature.


## Bill of Materials

The bill of materials changes from board to board, even among versions, based on different LEDs/Crystals.
Care should be taken to match the associated passives with each component.

| Ref | Value | Footprint |
|-----|-------|:----------|
| C1, C2 | 0.1μF | 0402 |
| C3, C4 | | 0603 |
| C5, C6, C7 | 0.27μF | 0402 |
| C8 | 1μF | 0402 |
| D1 | | 0402 |
| D2 | CFSH05-20L | SOD-882 |
| R1 | | 0603 |
| R2 | 680Ω | 0402 |
| U1 | PIC16LF19197-x/MR | QFN-64 |
| Y1 | 32.768kHz | 2-3.2x1.5 |

**C3, C4** - These capacitors should be chosen based upon the crystal. \
**D1** - Most 0402 LEDs should work. Maximum height is unknown, just keep it thin. I've found that diffused R/A LEDs work well, such as `LT VH9G` \
**R1** - Based upon the forward voltage of the chosen LED. Should keep max current under 2mA, preferably 1mA.


## Changelog

**Version 3.0**
- Switch to 4 layer PCB.
- Add charging circuit for ML2016 batteries.

**Version 2.1**
- Added POGO pin for buzzer + to replace the spring.
- Added POGO pin for buzzer - to replace ground tab.

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
