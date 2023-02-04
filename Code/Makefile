# You should at least check the settings for
# DEVICE ....... The AVR device you compile for
# CLOCK ........ Target AVR clock rate in Hertz
# OBJECTS ...... The object files created from your source files. This list is
#                usually the same as the list of source files with suffix ".o".
# PROGRAMMER ... Options to avrdude which define the hardware you use for
#                uploading to the AVR and the interface where this hardware
#                is connected.
# FUSES ........ Parameters for avrdude to flash the fuses appropriately.

DEVICE     ?= atmega32
CLOCK      = 8000000UL
PROGRAMMER ?= -c avrisp2 -P usb
SOURCE    = main.c DIO_program.c KPD_program.c LCD_program.c EEPROM_program.c
BUILDDIR = Build
SOURCEDIR = ../Code
# FUSES      = -U hfuse:w:0xd9:m -U lfuse:w:0x24:m
FUSES      = -U hfuse:w:0xd2:m -U lfuse:w:0xff:m

# Tune the lines below only if you know what you are doing:

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE) -B 10 -F

# Compile flags for avr-gcc v4.8.1. Does not produce -flto warnings.
COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) -I. -ffunction-sections

# Compile flags for avr-gcc v4.9.2 compatible with the IDE. Or if you don't care about the warnings. 
# COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) -I. -ffunction-sections -flto


OBJECTS = $(addprefix $(BUILDDIR)/,$(notdir $(SOURCE:.c=.o)))

# symbolic targets:
all:	main.hex

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c
	$(COMPILE) -MMD -MP -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $(BUILDDIR)/$@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

#.c.s:
	$(COMPILE) -S $< -o $(BUILDDIR)/$@

flash:	all
	$(AVRDUDE) -U flash:w:main.hex:i

fuse:
	$(AVRDUDE) $(FUSES)

# Xcode uses the Makefile targets "", "clean" and "install"
install: flash fuse

# if you use a bootloader, change the command below appropriately:
load: all
	bootloadHID main.hex

clean:
	rm -f main.hex $(BUILDDIR)/*.o $(BUILDDIR)/*.d $(BUILDDIR)/*.elf

# file targets:
$(BUILDDIR)/main.elf: $(OBJECTS)
	$(COMPILE) -o $(BUILDDIR)/main.elf $(OBJECTS) -lm -Wl,--gc-sections

main.hex: $(BUILDDIR)/main.elf
	rm -f main.hex
	avr-objcopy -j .text -j .data -O ihex $(BUILDDIR)/main.elf main.hex
	avr-size --format=berkeley $(BUILDDIR)/main.elf
# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.

# Targets for code debugging and analysis:
disasm:	main.elf
	avr-objdump -d $(BUILDDIR)/main.elf

cpp:
	$(COMPILE) -E $(SOURCEDIR)/main.c

# include generated header dependencies
-include $(BUILDDIR)/$(OBJECTS:.o=.d)