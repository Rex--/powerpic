# PowerOS #
Firmware for the PowerPIC Computer Watch. Checkout 
[rexploits.com/powerpic](https://rexploits.com/powerpic)
for more info.

### Requirements
The following programs are required to build and flash the firmware:
- `make`
- `xc8` - Microchip's C compiler for PICx Devices. I use the AUR package
`microchip-xc8-bin` which installs version 2.36 of the compiler.
- `picchick` - Flash PICs using an Arduino. Get it 
[from the repo](https://github.com/rex--/picchick).


## Building the Firmware

Compilation is achieved through the `make` utility.
```sh
make            # Build firmware with settings defined in Makefile.
make prod       # (Re)Build firmware with production settings.
make dev        # (Re)Build firmware with development settings.
make flash      # Flash firmware using picchick, building in neccessary.
make clean      # Remove built firmware files.
make fclean     # Remove all created files and folders.
```