# PowerOS
The watch firmware is written in C and compiled with Microchip's `xc8` compiler.
Functionality is separated into 'mode' applications, with each mode implementing
a set of procedures to be run when active. The active mode is changed by the Mode
side button, with each press cycling through an array of modes.


## Requirements
The following programs are required to build and flash the firmware:
- `make`
- `xc8` - Microchip's C compiler for PICx Devices. I use the AUR package
    _microchip-xc8-bin_ which installs version 2.36 of the compiler.
  - NOTE: Firmware has not been tested with v2.40
- `picchick` - Flash PICs using an AVR device. Get it 
[from the repo](https://github.com/rex--/picchick).


## Configuration

### Build Options
These options modify things about the firmware at build time and can be found
in the Makefile:
- `LOG_LVL` - Sets the amount of logged information. Logs above the specified
    level are not compiled into the final firmware, so expect things to run
    differently. A log level of '0' is recommend for builds that are going into a
    watch for use.
- `XTAL_FREQ` - The frequency in Hertz of the oscillator. Changing this value
    _DOES_ change the frequency of the internal oscillator!

### Library Config
Some libraries have their own configuration options. These can be found in
the corresponding heading files:
- lib/logging.h
  - `LOGGING_UART_BAUDRATE` - The baud rate of the debug UART connection.

### Configuring Modes
To configure which modes are built into the firmware, two things need to be
changed in `mode/mode_config.h`
1. `mode_app_index` - Add or Remove enum values so that MAX_MODES coincides with
    the length of the mode list.
2. `mode_list` - This list contains pointers to each mode application that
    should be compiled in.

_Note: All mode source files will still be compiled, but only the selected will
be included in the final .hex_


## Building
Compilation is achieved with the _make_ utility. A run of the `make fw` command
will produce a PowerOS.hex file in the sub-directory `build/`. The command
`make clean` will delete all built files, and a `make fclean` will remove all
files and created directories. A `make clean fw` is suggested after changing any
macros, to verify everything will be referencing the same value.
```sh
make [fw]       # Build firmware with settings defined in Makefile.
make clean      # Remove built firmware files.
make fclean     # Remove all created files and folders.
```


## Flash
> The pins needed for ICSP are broken out so any PIC programmer that 
> implements the right protocol can flash the built hex file to the watch.

Python utility `picchick` is used by the Makefile to flash the firmware. It
requires a compatible programmer connected to one of the computer's (virtual)
serial ports. Three connections (not including ground) are required between the
programmer and the PowerPIC: MCLR, CLK, and DAT. These connections are broken
out in test points that can be accessed with just removing the back of the watch.

### Programmer Config
- `PROGRAMMER_PORT` - Serial port of picchick compatible programmer. 
    e.g. _/dev/ttyUSB0_ or _COM3_
- `PROGRAMMER_BAUD` - Baud rate of programmer serial connection.

```sh
make flash      # Flash firmware using picchick, (re)building in necessary.
```
