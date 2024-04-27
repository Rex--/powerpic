# CasiOS
The watch firmware is written in C and compiled with Microchip's `xc8` compiler.
Functionality is separated into 'mode' applications, with each mode implementing
a set of procedures to be run when active. The active mode is changed by the Mode
side button, with each press cycling through an array of modes.


## Requirements
The following programs are required to build and flash the firmware:
- `make`
- `xc8` - Microchip's C compiler for PICx Devices
- `picchick` - Flash PICs using an AVR device. Get it 
[from the repo](https://github.com/rex--/picchick).


## Configuration

### Build Options
These options modify things about the firmware at build time and can be found
in the Makefile:
- `MODES` - A list of modes to compile into the firmware.
- `PCB_REV` - PCB Major version number. Changes some pins and which clock to use.
- `LOG_LVL` - Sets the amount of logged information. Logs above the specified
    level are not compiled into the final firmware, so expect things to run
    differently. A log level of '0' is recommend for builds that are going into a
    watch for use.
- `XTAL_FREQ` - The frequency in Hertz of the oscillator. Changing this value
    _DOES_ change the frequency of the internal oscillator!
- `BOOT_OFFSET` - Offset for bootloader. Set to 0 if not using a bootloader.

### Library Config
Some libraries have their own configuration options. These can be found in
the corresponding header files in `src/lib/`:
- **logging.h**
  - `LOGGING_UART_BAUDRATE` - The baud rate of the debug UART connection.

### Programmer Config
Configure the serial device to use in the Makefile. This should be a
1-wire capable serial converter.
- `PROGRAMMER_PORT` - Serial port of picchick compatible programmer. 
    e.g. _/dev/ttyUSB0_ or _COM3_
- `PROGRAMMER_BAUD` - Baud rate of programmer serial connection.
- `PROGRAMMER_FLAGS` - Additional programmer flags to use.

### Configuring Modes
To configre which modes are built into the firmware, edit the `modes.cfg`
file located in `src/modes/`. Each mode's source files, header files, and
mode_app_t struct should have the same name which corresponds to the name used
in the config file.


## Building
Compilation is achieved with the _make_ utility. A run of the `make fw` command
will produce a CasiOS.hex file in the sub-directory `build/`. The command
`make clean` will delete all built files, and a `make fclean` will remove all
files and created directories.
```sh
make [fw]       # Build firmware with settings defined in Makefile
make clean      # Remove built firmware files
make fclean     # Remove all created files and folders
```

The header file `src/modes/mode_config.h` is automatically generated with the
`make config` command. The generated files can be removed with `make cclean`.
Note that the config is generated automatically with `make fw` and removed with
a `make fclean`.
```sh
make config     # Generate config files
make cclean     # Remove generated files
```

## Upload
Python utility `picchick` is used by the Makefile in conjuction with
**flipflop** to upload firmware. It requires a serial device connected to
the PowerPIC's Debug and Ground pins. For further information about uploading
firmware checkout [flipflop](https://github.com/Rex--/flipflop), the bootloader
used for the PowerPIC.

After configuring the serial device in the Makefile, `make upload` should
upload the firmware to the watch. Don't forget to enter the bootloader by
resetting the watch!


```sh
make upload      # Upload firmware using picchick
```

## Copying
Copyright © 2022-2024 Rex McKinnon \
This software is released under the University of Illinois/NCSA Open Source
License. Check the LICENSE file for more details.
