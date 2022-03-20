########################################################################
#
#   Makfile for self_clock.
#
# - author  : Antoine Maleyrie
# - date    : 19.03.2022
#
########################################################################

########################################################################
# Start of user section
#

# Name of output file
TARGET = digital_shelf_clock

# Toolchain
PREFIX  = arm-none-eabi-
CC      = $(PREFIX)gcc
OBJCOPY = $(PREFIX)objcopy
AS      = $(PREFIX)as
SIZE    = $(PREFIX)size
OBJDUMP = $(PREFIX)objdump
GDB     = $(PREFIX)gdb

# The CPU used
MCU = cortex-m0plus
# Add -mthumb for use THUMB code
THUMB = -mthumb
# float computing method
#FPU = -mfloat-abi=softfp

#################################
## Start define flags
##

# User flags for ASM 
UASFLAGS =
# Default flags for C 
UCFLAGS  = -std=gnu99 -W -Wall -Wextra
UCFLAGS += -fmessage-length=0 -fdata-sections -ffunction-sections
# Default flags for linkage
ULDFLAGS = -Wl,-Map=$(TARGET).map,--gc-sections -specs=nano.specs -lc -lnosys
#ULDFLAGS = -Wl,-Map=$(TARGET).map,--gc-sections -specs=fpu/nosys.specs 
#ULDFLAGS = -Wl,-Map=$(TARGET).map,--gc-sections -specs=nosys.specs 
#ULDFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16

# Release flags additional for ASM 
RASFLAGS =
# Release flags additional for C 
RCFLAGS  = -O2 -g3
# Release flags additional for linkage 
RLDFLAGS = 

# Debug flags additional for ASM 
DASFLAGS =
# Debug flags additional for C 
DCFLAGS  = -g3
# Debug flags additional for linkage 
DLDFLAGS = 

##
## End define flags
#################################


# Linker script file
LDSCRIPT = linker/stm32g071rbtx_flash.ld

# List ASM source files
ASRC = app/startup_stm32g071xx.s

# List C source files
CSRC  = app/system_stm32g0xx.c
CSRC += app/main.c

# List of directories to include
UINCDIR  = app
UINCDIR += drivers/cmsis_device_g0/Include/
UINCDIR += drivers/CMSIS_5/CMSIS/Core/Include/
UINCDIR += drivers/stm32g0xx_hal_driver/Inc/

# List of user define
UDEFS  = STM32G071xx

# List of release define in more
RDEFS = 

# List of debug define in more
DDEFS =

# List of libraries directory
ULIBDIR =

# List of libraries
ULIBS = 

# Directories where find tools
TOOLDIR = tools

#
# End of user defines
########################################################################


########################################################################
# Start build define
#

# Binary objects directory
OBJS = build
# Binary ASM objects directory
DASOBJS = $(OBJS)/asm
# Binary C objects directory
DCOBJS = $(OBJS)/c

# ASM list of binary objects
ASOBJS = $(patsubst %.s,$(DASOBJS)/%.o, $(ASRC))
# C list of binary objects
COBJS = $(patsubst %.c,$(DCOBJS)/%.o, $(CSRC))

# List of include directory
INCDIR = $(patsubst %,-I%, $(UINCDIR))
# List of include library
LIBDIR = $(patsubst %,-L%, $(ULIBDIR))

# List of library
LIBS = $(patsubst %,-l%, $(ULIBS))

# List of define
_UDEFS = $(patsubst %,-D%, $(UDEFS))
_RDEFS = $(patsubst %,-D%, $(RDEFS))
_DDEFS = $(patsubst %,-D%, $(DDEFS))

#
# End build define
########################################################################


########################################################################
# Start rules section
#

all:release

# Build define for release
#.PHONY: release
release:ASFLAGS = -mcpu=$(MCU) $(THUMB) $(FPU) $(RASFLAGS) $(UASFLAGS)
release:CFLAGS  = -mcpu=$(MCU) $(THUMB) $(FPU) $(RCFLAGS) $(UCFLAGS) $(_UDEFS) $(_RDEFS) $(INCDIR)
release:LDFLAGS = -mcpu=$(MCU) $(THUMB) $(FPU) $(RLDFLAGS) $(ULDFLAGS) -T$(LDSCRIPT) $(LIBDIR) -Wl,-Map,$(TARGET).map
release:$(TARGET).elf

# Build define for debug
#.PHONY: debug
debug:ASFLAGS = -mcpu=$(MCU) $(THUMB) $(FPU) $(DASFLAGS) $(UASFLAGS)
debug:CFLAGS  = -mcpu=$(MCU) $(THUMB) $(FPU) $(DCFLAGS) $(UCFLAGS) $(_UDEFS) $(_DDEFS) $(INCDIR)
debug:LDFLAGS = -mcpu=$(MCU) $(THUMB) $(FPU) $(DLDFLAGS) $(ULDFLAGS) -T$(LDSCRIPT) $(LIBDIR) -Wl,-Map,$(TARGET).map
debug:$(TARGET).elf


# Build sources to generate elf file
$(TARGET).elf: $(ASOBJS) $(COBJS)
	$(CC) -o $@ $(LDFLAGS) $(LIBS) $(ASOBJS) $(COBJS) 
	$(SIZE) $@

# Build ASM sources
.PRECIOUS: $(DASOBJS)/%.o
$(DASOBJS)/%.o: %.s
	@mkdir -p $(patsubst /%,%, $(@D))
	$(AS) $(ASFLAGS) $< -o $@

# Build C sources
.PRECIOUS: $(DCOBJS)/%.o
$(DCOBJS)/%.o: %.c
	@mkdir -p $(patsubst /%,%, $(@D))
	$(CC) $(CFLAGS) $< -c -o $@
	
hex:
	$(OBJCOPY) -O ihex $(TARGET).elf $(TARGET).hex

bin:
	$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin
	
size:
	$(SIZE) $(TARGET).elf
	
disassemble:
	$(OBJDUMP) -hd $(TARGET).elf > $(TARGET).lss


## Reset the target
#reset:
#	openocd -f $(CONFIFDIR)/openocd/init.cfg -c "init; reset; shutdown"
##   openocd -f $(CONFIFDIR)/openocd/init.cfg -c "init; reset; shutdown" 2> /dev/null
	
## Flash the target
#flash_program:
#	openocd -f $(CONFIFDIR)/openocd/init.cfg -c "program $(TARGET).elf verify reset"
	
## Flash the target
#boot_program: bin
#	stm32flash -w $(TARGET).bin -v /dev/ttyUSB0
	
## Show the bank flash 0 erased or not
#flash_check:
#	@openocd -f $(CONFIFDIR)/openocd/init.cfg -f $(CONFIFDIR)/openocd/flash_erase_check.cfg

## Erase the flash
#flash_erase:
#	@openocd -f $(CONFIFDIR)/openocd/init.cfg -f $(CONFIFDIR)/openocd/flash_erase.cfg

## Erase the flash and eeprom
#flash_eeprom_erase:
#	@openocd -f $(CONFIFDIR)/openocd/init.cfg -f $(CONFIFDIR)/openocd/flash_eeprom_erase.cfg
	
## Run gdb/openocd for load and debug.
#gdb:debug
#	$(GDB) --command=$(CONFIFDIR)/gdb/gdbinit.cfg $(TARGET).elf
	
# Run cgdb/openocd for load and debug with color.
cgdb:debug
	cgdb -d $(GDB) --command=$(TOOLDIR)/gdb/gdbinit_stm32g0x.cfg $(TARGET).elf

	
# Clean projet
clean:
	rm -fr $(OBJS)
	rm -f *.elf
	rm -f *.map
	rm -f *.bin
	rm -f *.hex
	rm -f *.lss
	rm -f *.log

#
# End rules section
########################################################################
