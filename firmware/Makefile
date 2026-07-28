###############################################################################
#    Firmware Configuration    #

## List of modes to compile
MODES := clock timer power uptime alarmclock settings

## External mode file
MODE_FILE := src/modes/modes.cfg

## PCB Major version number
PCB_REV := 2

## Set the log level 0-4 [Disabled, Error, Warning, Info, Debug]
LOG_LVL := 0

## Frequency of the clock in Hz
XTAL_FREQ := 4000000

## Bootloader offset in hex
BOOT_OFFSET := 0x400

## Date
DT_DATE := $(shell printf ' -DDTINIT_YEAR=0x%d -DDTINIT_MONTH=0x%d -DDTINIT_DAY=0x%d -DDTINIT_WDAY=0x%d' `date +%-y` `date +%-m` `date +%-d` `date +%-w`)
DT_TIME := $(shell printf ' -DDTINIT_HOUR=0x%d -DDTINIT_MIN=0x%d -DDTINIT_SEC=0x%d' `date +%-H` `date +%-M` `date +%-S`)
DTINIT := $(DT_DATE) $(DT_TIME)


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

## Vendor directory
VENDOR_DIR := vendor


################################################################################
#    Programmer options    #

## Serial port of a device with the flipflop bootloader
PROGRAMMER_PORT := /dev/ttyUSB0
PROGRAMMER_BAUD := 500000
PROGRAMMER_FLAGS := -d $(MCU) -c flipflop -P $(PROGRAMMER_PORT) -B $(PROGRAMMER_BAUD)
PROGRAMMER_FLAGS += --pagesize 1024


################################################################################
#    Compiler Setup    #

## Microchip's XC8 compiler
CC := xc8-cc

## Target Arch flag
TARGET_ARCH := -mcpu=$(MCU)

## Device Family Pack
DFP := PIC16F1xxxx_DFP
DFP_DIR := $(VENDOR_DIR)/microchip/$(DFP)
DFP_URL := https://packs.download.microchip.com/Microchip.PIC16F1xxxx_DFP.1.31.465.atpack

## Firmware build options
FWFLAGS = -D_XTAL_FREQ=${XTAL_FREQ} -DPCB_REV=${PCB_REV} -DLOG_LVL=$(LOG_LVL) $(DTINIT)

DFPFLAGS := -mdfp=$(DFP_DIR)/xc8

## Options for the xc8 compiler
CFLAGS := -c -O3

## Options for xc8 compiler linker
LFLAGS := -O3 -mcodeoffset=${BOOT_OFFSET}

################################################################################
#    User Config Override    #

# -include config.mk

-include $(MODE_FILE)

################################################################################
#    Match n' Making    #

# Recursively find all *.c files in our SOURCE_DIR exluding the 'modes' directory
SOURCES := $(shell find $(SOURCE_DIR) -name '*.c' -not -path '$(SOURCE_DIR)/modes/*')
# Add modes to build sources
SOURCES += $(foreach mode,$(MODES),$(SOURCE_DIR)/modes/$(mode).c)

# Mode config file template path
MODE_CONFIG_TEMPLATE := $(SOURCE_DIR)/modes/mode_config_template.h
# Mode config file path
MODE_CONFIG := $(SOURCE_DIR)/modes/mode_config.h

# Generate list of p-code object files from sources
OBJECTS := $(SOURCES:$(SOURCE_DIR)/%.c=$(BUILD_DIR)/%.p1)

# Generate list of dependency files
# DEPENDS := $(SOURCES:%.c=$(BUILD_DIR)/%.d)
# Generate list from actual files in directory to get around a bug introduced in make 4.4
DEPENDS := $(shell [ -d $(BUILD_DIR) ] && find $(BUILD_DIR) -name '*.d')

## Generate list of folders in build directory
# BUILD_FILE_DIRS = $(shell find $(BUILD_DIR) -type d)

# Generate p-code files
$(BUILD_DIR)/%.p1: $(SOURCE_DIR)/%.c Makefile
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	@$(CC) $(TARGET_ARCH) $(DFPFLAGS) $(CFLAGS) -I$(INCLUDE_DIR)/ ${FWFLAGS} $< -o $@

## Generate bin files (.hex and .elf)
# Note: This command builds everythings,
# maybe we can find a way to split up the actions.
$(BUILD_DIR)/$(TARGET).hex: $(DFP_DIR) $(MODE_CONFIG) $(OBJECTS) Makefile
	@mkdir -p $(dir $@)
	@echo "Building hexfile..."
	@$(CC) $(TARGET_ARCH) $(DFPFLAGS) $(LFLAGS) $(OBJECTS) -o $@

## Generate mode_config.h file
$(MODE_CONFIG): $(MODE_FILE) $(MODE_CONFIG_TEMPLATE) Makefile
	@echo "Generating '$(MODE_CONFIG)'..."
	@echo -e -n '' $(foreach mode,$(MODES),$(mode)\\n)
	@cat $(MODE_CONFIG_TEMPLATE) | \
	sed -e 's/\/\/MODE_INCLUDE/$(foreach mode,$(MODES),#include "modes\/$(mode).h"NEWLINE)/' | \
	sed -e 's/\/\/MODE_INDEX/$(foreach mode,$(MODES),$(mode)_index,NEWLINE)/' | \
	sed -e 's/\/\/MODE_LIST/$(foreach mode,$(MODES),\&$(mode)_mode,NEWLINE)/' | \
	sed -e 's/NEWLINE /\n/g' | \
	sed -e 's/NEWLINE//' | \
	sed -e 's/mode_config_template/mode_config/' \
	> $(MODE_CONFIG)

## Download and extract Device Family Pack
$(DFP_DIR):
	@echo "Downloading PIC16F1xxxx_DFP..."
	@mkdir -p $(BUILD_DIR)
	@curl --progress-bar -o $(BUILD_DIR)/$(DFP).atpack $(DFP_URL)
	@echo -n "Extracting PIC16F1xxxx_DFP"
	@mkdir -p $(DFP_DIR)
	@unzip $(BUILD_DIR)/$(DFP).atpack -d $(VENDOR_DIR)/microchip/$(DFP) | awk 'BEGIN {ORS="."} {if(NR%50==0)print "."}'
	@echo "done"


################################################################################
#    Make Commands    #

## Commands to use
.PHONY: fw config clean fclean flash upload sync info help

########################
#    Build Commands    #

## Build firmware with settings defined in Makefile. Equal to a plain 'make'
fw: $(BUILD_DIR)/$(TARGET).hex

# Gen config
config: $(SOURCE_DIR)/modes/mode_config.h


########################
#    Clean Commands    #

## Clean-up build files.
clean:
	@echo "Removing firmware build files..."
	${eval BUILD_FILE_DIRS = $(shell find $(BUILD_DIR) -type d)}
	@rm -fv $(BUILD_FILE_DIRS:%=%/*.*)
	@echo ""

## Clean-up generated files
cclean:
	@echo "Removing generated config files..."
	@rm -fv $(SOURCE_DIR)/modes/mode_config.h
	@echo ""

## Clean-up build directories
fclean: clean cclean
	@echo "Removing firmware build directories..."
	@rm -rfv $(BUILD_DIR)
	@echo ""

## Clean-up vendor directory
vclean:
	@echo "Removing firmware vendor directory..."
	@rm -rf $(VENDOR_DIR)
	@echo ""


###########################
#    Flashing Commands    #

## Uploads the built hexfile onto a device with flipflop using picchick
upload: fw
	@echo "Uploading firmware..."
	@picchick \
		$(PROGRAMMER_FLAGS) \
		--flash \
		$(BUILD_DIR)/$(TARGET).hex

## Updates the the time compiled into the firmware to the current time
sync:
	@echo "Current time:" $(shell date)
	touch src/lib/datetime.c

#######################
#    Help Commands    #

info:
	@echo "Sources:"
	@echo $(SOURCES)
	@echo "Objects:"
	@echo $(OBJECTS)

help:
	@echo "'make [fw]'   - Build firmware"
	@echo "'make config' - Generate modes/mode_config.h header file"
	@echo "'make upload' - Upload firmware using picchick, building if neccessary"
	@echo "'make sync'   - Update time next time firmware is built"
	@echo "'make clean'  - Remove built firmware files"
	@echo "'make fclean' - Remove all build files and folders"
	@echo "'make vclean' - Remove vendor folder"
	@echo " "

.DEFAULT:
	@echo "Error: Unknown command"
	@make help

# Include dependency rules
include $(DEPENDS)
