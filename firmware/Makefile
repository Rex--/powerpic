###############################################################################
#    Firmware Configuration    #

## PCB Major version number
PCB_REV := 2

## Set the log level 0-4 [Disabled, Error, Warning, Info, Debug]
LOG_LVL := 4

## Frequency of the clock in Hz
XTAL_FREQ := 4000000


################################################################################
#    Programmer options    #

## Serial port of a picchick compatible programmer
PROGRAMMER_PORT := /dev/ttyUSB0




###############################################################################
#    Project Configuration    #  

## The name of the project
TARGET := PowerOS

## The device to compile for
MCU := 16LF19197

## Source directory
SOURCE_DIR := src

## Include directory
INCLUDE_DIR := src

## Build directory
BUILD_DIR := build


################################################################################
#    Compiler Setup    #

## Microchip's XC8 compiler
CC := xc8-cc

## Target Arch flag
TARGET_ARCH := -mcpu=$(MCU)

## Firmware build options
FWFLAGS = -D_XTAL_FREQ=${XTAL_FREQ} -DPCB_REV=${PCB_REV} -DLOG_LVL=$(LOG_LVL)

## Options for the xc8 compiler
CFLAGS := -O2 -c

## Options for xc8 compiler linker
LFLAGS := -O2


################################################################################
#    Match n' Making    #

# Recursively find all *.c files in our SOURCE_DIRECTORY
SOURCES := $(shell find $(SOURCE_DIR) -name '*.c')

# Generate list of p-code object files from sources
OBJECTS := $(SOURCES:%.c=$(BUILD_DIR)/%.p1)

## Generate list of folders in build directory
# BUILD_FILE_DIRS = $(shell find $(BUILD_DIR) -type d)

# Generate p-code files
$(BUILD_DIR)/%.p1: %.c
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	@$(CC) $(TARGET_ARCH) $(CFLAGS) -I$(INCLUDE_DIR)/ ${FWFLAGS} $< -o $@

## Generate bin files (.hex and .elf)
# Note: This command builds everythings,
# maybe we can find a way to split up the actions.
$(BUILD_DIR)/$(TARGET).hex: $(OBJECTS)
	@mkdir -p $(dir $@)
	@echo "Building hexfile..."
	@$(CC) $(TARGET_ARCH) $(LFLAGS) $(OBJECTS) -o $@


################################################################################
#    Make Commands    #

## Commands to use
.PHONY: prod dev clean fclean flash help

## Helper rules
.PHONY: fw prod_flags dev_flags

########################
#    Build Commands    #

## Build firmware with settings defined in Makefile. Equal to a plain 'make'
fw: $(BUILD_DIR)/$(TARGET).hex

## Build firmware in production mode
prod_flags:
	$(eval DEV_BUILD = 0)
	@echo "Building firmware for production..."
prod: clean prod_flags fw

## Build firmware in development mode
dev_flags:
	$(eval DEV_BUILD = 1)
	@echo "Building firmware for development..."
dev: clean dev_flags fw


########################
#    Clean Commands    #

## Clean-up build files.
clean:
	@echo "Removing firmware build files..."
	${eval BUILD_FILE_DIRS = $(shell find $(BUILD_DIR) -type d)}
	@rm -f $(BUILD_FILE_DIRS:%=%/*.*)
	@echo ""

## Clean-up build directories
fclean:
	@echo "Removing firmware build directories..."
	@rm -rf $(BUILD_DIR)
	@echo ""


###########################
#    Flashing Commands    #

## Flashes the built hexfile onto a device using picchick
flash: fw
	@echo "Flashing..."
	picchick \
		-d $(MCU) \
		-P $(PROGRAMMER_PORT) \
		-B 115200 \
		--erase \
		--flash \
		$(BUILD_DIR)/$(TARGET).hex


#######################
#    Help Commands    #

help:
	@echo "'make'        - Build firmware with settings defined in Makefile."
	@echo "'make prod'   - Build firmware with production settings."
	@echo "'make dev'    - Build firmware with development settings."
	@echo " "
	@echo "'make flash'  - Flash firmware using picchick, building if neccessary."
	@echo " "
	@echo "'make clean'  - Remove built firmware files."
	@echo "'make fclean' - Remove all files and folders created."
	@echo " "

.DEFAULT:
	@echo "Error: Unknown command"
	@echo " "
	@echo "'make'        - Build firmware with settings defined in Makefile."
	@echo "'make prod'   - Build firmware with production settings."
	@echo "'make dev'    - Build firmware with development settings."
	@echo " "
	@echo "'make flash'  - Flash firmware using picchick, building if neccessary."
	@echo " "
	@echo "'make clean'  - Remove built firmware files."
	@echo "'make fclean' - Remove all files and folders created."
	@echo " "