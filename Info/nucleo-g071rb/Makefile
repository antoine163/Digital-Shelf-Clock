########################################################################
#
#   Makfile for self clock.
#
# - author  : antoine163
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
#ULDFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16

# Release flags additional for ASM 
RASFLAGS =
# Release flags additional for C 
RCFLAGS  = -O2 -g3 
#RCFLAGS  = -O
# Release flags additional for linkage 
RLDFLAGS = 

# Debug flags additional for ASM 
DASFLAGS =
# Debug flags additional for C 
DCFLAGS  = -Og -g3
# Debug flags additional for linkage 
DLDFLAGS = 

##
## End define flags
#################################
PROGRAM_IN = FLASH
# Linker script file
ifeq ($(PROGRAM_IN),SRAM)
	LDSCRIPT = linker/stm32g071rbtx_sram.ld
else
	LDSCRIPT = linker/stm32g071rbtx_flash.ld
endif


# List ASM source files
ASRC = app/startup_stm32g071xx.s

# List C source files
CSRC  = app/system_stm32g0xx.c
CSRC += app/board/nucleo_g071x.c
CSRC += app/main.c
CSRC += app/tasks/task_test.c

## ST source files
CSRC += drivers/stm32g0xx_hal_driver/Src/stm32g0xx_ll_utils.c

## mpLib source files
CSRC += mpLib/mp.c
CSRC += mpLib/utils/fifo.c
CSRC += mpLib/portable/FreeRtos/stm32g0xx/mp_interrupt_port.c
CSRC += mpLib/portable/FreeRtos/stm32g0xx/mp_gpio_port.c
CSRC += mpLib/portable/FreeRtos/stm32g0xx/mp_uart_port.c
#CSRC += mpLib/portable/FreeRtos/stm32g0xx/mp_adc_port.c
#CSRC += mpLib/portable/FreeRtos/stm32g0xx/mp_spi_port.c

## FreeRTOS source files
#CSRC += middlewares/FreeRTOS-Kernel/tasks.c
#CSRC += middlewares/FreeRTOS-Kernel/list.c
#CSRC += middlewares/FreeRTOS-Kernel/portable/GCC/ARM_CM0/port.c

# List of directories to include
UINCDIR  = app
UINCDIR += drivers/cmsis_device_g0/Include/
UINCDIR += drivers/CMSIS_5/CMSIS/Core/Include/
UINCDIR += drivers/stm32g0xx_hal_driver/Inc/
UINCDIR += middlewares/FreeRTOS-Kernel/include/
UINCDIR += middlewares/FreeRTOS-Kernel/portable/GCC/ARM_CM0/
UINCDIR += mpLib/include/
UINCDIR += mpLib/portable/FreeRtos/stm32g0xx/

# List of user define
UDEFS  = STM32G071xx
UDEFS += BOARD_NUCLEO_G071X

ifeq ($(PROGRAM_IN),SRAM)
	UDEFS += USER_VECT_TAB_ADDRESS
	UDEFS += VECT_TAB_SRAM
endif


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
OBJS = /tmp/$(TARGET)/build
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
	$(CC) -o $@ $(ASOBJS) $(COBJS) $(LDFLAGS) $(LIBS) 
	$(SIZE) $@

# Build ASM sources
.PRECIOUS: $(DASOBJS)/%.o
$(DASOBJS)/%.o: %.s
#	@mkdir -p $(patsubst /%,%, $(@D))
	@mkdir -p $(@D)
	$(AS) $(ASFLAGS) $< -o $@

# Build C sources
.PRECIOUS: $(DCOBJS)/%.o
$(DCOBJS)/%.o: %.c
#	@mkdir -p $(patsubst /%,%, $(@D))
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $< -c -o $@
	
hex:
	$(OBJCOPY) -O ihex $(TARGET).elf $(TARGET).hex

bin:
	$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin
	
size:
	$(SIZE) $(TARGET).elf
	
disassemble:
	$(OBJDUMP) -hd $(TARGET).elf > $(TARGET).lss


# Reset target with openocd
reset:
	openocd -f tools/openocd/st_nucleo_g0.cfg -c "init; reset; shutdown"
	
# Flash target with openocd
flash:
	openocd -f tools/openocd/st_nucleo_g0.cfg -c "program $(TARGET).elf verify; reset; shutdown"
	
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
	
# Load (flash or ram) the program to target and run with gdb/openocd
load:
	$(GDB) --command=$(TOOLDIR)/gdb/gdbinit_stm32g0x_load.cfg $(TARGET).elf
	
# Run gdb/openocd for load and debug.
gdb:debug
	$(GDB) --command=$(TOOLDIR)/gdb/gdbinit_stm32g0x.cfg $(TARGET).elf
	
# Run cgdb/openocd for load and debug with color.
stflash:bin
	st-flash --connect-under-reset write $(TARGET).bin 0x8000000
	
# Run cgdb/openocd for load and debug with color.
cgdb:debug
	cgdb -d $(GDB) --command=$(TOOLDIR)/gdb/gdbinit_stm32g0x.cfg $(TARGET).elf
	
# Run minicom on /dev/ttyACM0 for nucleo.
minicom:
	minicom -b 115200 -D /dev/ttyACM0 -c on
	
# Build documentation with doxygen
doc:
	cd mpLib/doc/ && doxygen Doxyfile
	
cleandoc:
	rm -fr mpLib/doc/html/
	
# Clean projet
clean: cleandoc
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
