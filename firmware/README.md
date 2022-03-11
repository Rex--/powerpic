# PowerOS #
Firmware for the PowerPIC Computer Watch. Checkout [rexploits.com/powerpic](rexploits.com/powerpic)
for more info.

### Requirements
The following programs are required to build and flash the firmware:
- `make`
- `xc8` - Microchip's C compiler for PICx Devices. I use the AUR package
`microchip-xc8-bin` which installs version 2.36 of the compiler.
- `picchick` - Flash PICs using an Arduino. Get it 
[from the repo](https://github.com/rex--/picchick).

The following programs are required to build the documentation:
- `doxygen`
- `plantuml` - Generate various types of diagrams.


## Building the Firmware

Compilation is achieved through the `make` utility.
```sh
make all        # Build firmware and documentation
make [firmware] # Build firmware
make flash      # Flash firmware onto device
make pro        # Build and Flash firmware
make diagrams   # Build diagrams
make dox        # Build documentation
make docs       # Build diagrams and documentation
make clean      # Delete build files, keeping directories
make fclean     # Delete build directories
make info       # Display information about the build
make map        # Display map of device memory
make imap       # Display information and map
```
