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

## Documentations build directories
DOCS_BUILD_DIR := docs/html docs/diagrams/img


################################################################################
#    Programmer options    #

## Serial port of a picchick compatible programmer
PROGRAMMER_PORT = /dev/ttyACM0

################################################################################
#    Compiler Setup    #

## Microchip's XC8 compiler
CC := xc8-cc

# Target Arch flag
TARGET_ARCH := -mcpu=$(MCU)

# Options for the xc8 compiler:
# -c for compile
CFLAGS := -O2 -c

# Options for xc8 compiler linker
LFLAGS := -O2

################################################################################
#    Match n' Making    #

# Recursively find all *.c files in our SOURCE_DIRECTORY
SOURCES := $(shell find $(SOURCE_DIR) -name '*.c')

# Generate list of p-code object files from sources
OBJECTS := $(SOURCES:%.c=$(BUILD_DIR)/%.p1)

## Generate list of dependency files
DEPS := $(OBJECTS:%.p1=%.d)

## Generate list of folders in build directory
BUILD_FILE_DIRS = $(shell find $(BUILD_DIR) -type d)

## Generate list of folder in docs directory
DOCS_BUILD_FILE_DIRS = $(foreach dir,$(DOCS_BUILD_DIR),$(shell find $(dir) -type d))

# Generate list of header files from sources
#HEADERS := $(wildcard *.h)

# Generate p-code files
$(BUILD_DIR)/%.p1: %.c
	@mkdir -p $(dir $@)
	$(CC) $(TARGET_ARCH) -I $(INCLUDE_DIR) $(CFLAGS) $< -o $@

## Generate bin files (.hex and .elf)
# Note: This command builds everythings,
# maybe we can find a way to split up the actions.
$(BUILD_DIR)/$(TARGET).hex: $(OBJECTS)
	@mkdir -p $(dir $@)
	$(CC) $(TARGET_ARCH) $(LFLAGS) $(OBJECTS) -o $@


################################################################################
#    Make Commands    #

.PHONY: all info clean clean-all delete flash pro info map imap diagrams documentation docs

# Generate our documentation everytime we build as per Barr's Coding Standards
all: firmware docs

########################
#    Build Commands    #

## Build firmware hexfile
firmware: $(BUILD_DIR)/$(TARGET).hex

## Generate images from plantuml diagrams
diagrams:
	plantuml docs/diagrams/*.puml -tsvg -o img/

## Generate documentation using doxygen
dox:
	doxygen

## First build diagrams, then gen documentation because we use some diagrams
## in the docs (doxygen will automatically copy any images used)
docs: diagrams dox

########################
#    Clean Commands    #

## Clean-up build files.
clean:
	@echo "Removing firmware build files..."
	rm -f $(BUILD_FILE_DIRS:%=%/*.*)
	@echo "Removing documentation build files..."
	rm -f $(DOCS_BUILD_FILE_DIRS:%=%/*.*)

## Clean-up build directories
fclean:
	@echo "Removing firmware build directories..."
	rm -rf $(BUILD_DIR)
	@echo "Removing documentation build directories..."
	rm -rf $(DOCS_BUILD_DIR)

###########################
#    Flashing Commands    #

## Flashes the built hexfile onto a device using picchick
flash:
	picchick -d $(MCU) -p $(PROGRAMMER_PORT) --erase --flash $(BUILD_DIR)/$(TARGET).hex

## Build and flash [pro]ject
pro: firmware flash

##############################
#    Information Commands    #

## Display some information about the build
info:
	@echo
	@echo "Name:" $(TARGET)
	@echo "Target MCU:" $(MCU)
	@echo "Source directory:" $(SOURCE_DIR)/
	@echo "Source files:" $(SOURCES)
	@echo "Build directory:" $(BUILD_DIR)/
	@echo "Build files:" $(OBJECTS)

## Display a map of the hexfile to PIC memory addresses.
map:
	@picchick -d $(MCU) --map $(BUILD_DIR)/$(TARGET).hex

## Display Info about the build and a map.
imap: info map
