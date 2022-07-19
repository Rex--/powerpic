# PowerOS #
Firmware for the PowerPIC Computer Watch.

## Requirements
The following programs are required to build and flash the firmware:
- `make`
- `xc8` - Microchip's C compiler for PICx Devices. I use the AUR package
`microchip-xc8-bin` which installs version 2.36 of the compiler.
- `picchick` - Flash PICs using an AVR device. Get it 
[from the repo](https://github.com/rex--/picchick).


## Configuration
Configuration of the firmware is done mostly in the Makefile:
- `LOG_LVL` - Sets the amount of logged information. Logs above the specified
level are not compiled into the final firmware, so expect things to run
differently. A log level of '0' is recommend for builds that are going into a
watch for use.
- `XTAL_FREQ` - The frequency in Hertz of the oscillator. Changing this value
_DOES_ change the frequency of the internal oscillator!

Some configuration is not yet in the Makefile (but probably should be):
- lib/logging.h : `LOGGING_UART_BAUDRATE` - The baudrate of the debug UART connection.

### Configuring Modes
To configure which modes are built into the firmware, two things need to be changed in `mode/mode_config.h`
1. `mode_app_index` - Add or Remove enum values so that MAX_MODES coincides with the length of the mode list.
2. `mode_list` - This list contains pointers to each mode application that should be compiled in.

_Note: All mode source files will still be compiled, but only the selected will be included in the final .hex_


## Building

Compilation is achieved through the `make` utility.
```sh
make            # Build firmware with settings defined in Makefile.
make flash       # Flash firmware using picchick, building in neccessary.
make clean      # Remove built firmware files.
make fclean     # Remove all created files and folders.
```