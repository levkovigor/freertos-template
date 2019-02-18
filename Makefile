# ------------------------------------------------------------------------------
#         ATMEL Microcontroller Software Support 
# ------------------------------------------------------------------------------
# Copyright (c) 2008, Atmel Corporation
#
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# - Redistributions of source code must retain the above copyright notice,
# this list of conditions and the disclaimer below.
#
# Atmel's name may not be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
# DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
# OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
# EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# ------------------------------------------------------------------------------

#	Generic makefile

#-------------------------------------------------------------------------------
#		User set options
#-------------------------------------------------------------------------------

CHIP	= at91sam9g20
BOARD	= ISIS_OBC_G20

# Trace level used for compilation
# (can be overriden by adding TRACE_LEVEL=#number to the command-line)
# TRACE_LEVEL_DEBUG      5
# TRACE_LEVEL_INFO       4
# TRACE_LEVEL_WARNING    3
# TRACE_LEVEL_ERROR      2
# TRACE_LEVEL_FATAL      1
# TRACE_LEVEL_NO_TRACE   0
TRACE_LEVEL	= 5

# Optimization level, put in comment for debugging
OPTIMIZATION 	= -O0

AT91LIB		= ./at91lib
FREERTOS	= ./freertos

# Output file basename
OUTPUT		= img-$(BOARD)-$(CHIP)

CHIP_CORE	= arm926ej_s
CHIP_IP_MCI  	= MCI_PDC
CHIP_IP_UDP  	= USB_UDP
CHIP_IP_UHP  	= USB_OHCI

MEMORIES	= sdram norflash sram	


# Output directories
BIN		= bin
OBJ		= obj

#-------------------------------------------------------------------------------
#		Tools
#-------------------------------------------------------------------------------

# Tools suffix when cross-compiling
CROSS_COMPILE=arm-none-eabi-

# Compilation tools
CC 	= $(CROSS_COMPILE)gcc
SIZE	= $(CROSS_COMPILE)size
STRIP	= $(CROSS_COMPILE)strip
OBJCOPY	= $(CROSS_COMPILE)objcopy

# Flags

INCLUDES	=  -I$(AT91LIB)/include/at91/boards/$(BOARD)
INCLUDES	+= -I$(AT91LIB)/include/at91/peripherals
INCLUDES	+= -I$(AT91LIB)/include/at91/components
INCLUDES	+= -I$(AT91LIB)/include/at91
INCLUDES	+= -I$(AT91LIB)/include/


INCLUDES	+= -I$(FREERTOS)/portable/GCC/ARM9_AT91SAM9G20
INCLUDES	+= -I$(FREERTOS)/portable/MemMang
INCLUDES	+= -I$(FREERTOS)/portable/
INCLUDES	+= -I$(FREERTOS)/include
INCLUDES	+= -I$(FREERTOS)

INCLUDES	+= -Ihal/include/hal/
INCLUDES	+= -Ihal/include/

INCLUDES	+= -Ihcc/include/hcc/
INCLUDES	+= -Ihcc/include/psp/include
INCLUDES	+= -Ihcc/include/

INCLUDES	+= -Isatellite-subsystems/include/satellite-subsystems/
INCLUDES	+= -Isatellite-subsystems/include/

INCLUDES	+= -Imission-support/include/mission-support/
INCLUDES	+= -Imission-support/include/

INCLUDES	+= -Isrc/

TARGET_OPTS = -mcpu=arm926ej-s

CFLAGS	=  $(TARGET_OPTS)
CFLAGS	+= -Wall -Wextra -ffunction-sections -fmessage-length=0  -D"IOBC_REV=C"
CFLAGS	+= -DDEBUG=1 
CFLAGS	+= -g $(OPTIMIZATION) $(INCLUDES) -D$(CHIP) -DTRACE_LEVEL=$(TRACE_LEVEL)
ASFLAGS	=  $(TARGET_OPTS)
ASFLAGS += -Wall -Wextra -ffunction-sections -fmessage-length=0  -D"IOBC_REV=C"
ASFLAGS += -DDEBUG=1
ASFLAGS += $(OPTIMIZATION) $(INCLUDES) -D$(CHIP) -DTRACE_LEVEL=$(TRACE_LEVEL) -D__ASSEMBLY__
LDFLAGS	=  $(TARGET_OPTS)  $(OPTIMIZATION) -fmessage-length=0 -ffunction-sections -Wall -Wextra
LDFLAGS += -g -Wl,-Map,$(OUTPUT).map -nostartfiles -Xlinker --gc-sections


#-------------------------------------------------------------------------------
#		FILES
#-------------------------------------------------------------------------------

# Directories where source files can be found
VPATH   = src/
VPATH  	+= $(AT91LIB)/lib
VPATH	+= $(FREERTOS)/lib
VPATH 	+= hal/lib/
VPATH 	+= hcc/lib/
VPATH 	+= satellite-subsystems/lib/
VPATH	+= mission-support/lib/

# Objects build from C source files
C_OBJECTS	 = main.o


# Objects build from assembly source files
ASM_OBJECTS	= board_fstartup.o

#Append OBJ and BIN directories to output filename
OUTPUT	:= $(BIN)/$(OUTPUT)

#-------------------------------------------------------------------------------
#		Rules
#-------------------------------------------------------------------------------
all: $(BIN) $(OBJ) $(MEMORIES)

$(BIN) $(OBJ):
	mkdir $@

define RULES
C_OBJECTS_$(1) = $(addprefix $(OBJ)/$(1)_, $(C_OBJECTS))
ASM_OBJECTS_$(1) = $(addprefix $(OBJ)/$(1)_, $(ASM_OBJECTS))

$(1): $$(ASM_OBJECTS_$(1)) $$(C_OBJECTS_$(1)) -lMissionSupportD -lSatelliteSubsystemsD -lHCCD -lHALD -lFreeRTOSD -lAt91D
	$(CC) $(LDFLAGS) -Tsrc/sdram.lds \
	--specs=nano.specs -lc -u _printf_float -u _scanf_float -o $(OUTPUT)-$$@.elf $$^
	$(OBJCOPY) -O binary $(OUTPUT)-$$@.elf $(OUTPUT)-$$@.bin
	$(SIZE) --format=berkeley $$^ $(OUTPUT)-$$@.elf

$$(C_OBJECTS_$(1)): $(OBJ)/$(1)_%.o: %.c Makefile $(OBJ) $(BIN)
	@echo $(CFLAGS)
	$(CC) -E -C $(CFLAGS) -D$(1) -MMD -MP -MF$$(@:%o=%d) -MT$$(@:%o=%d) -o $$(@:%o=%i) $$<
	$(CC) $(CFLAGS) -D$(1) -MMD -MP -MF$$(@:%o=%d) -MT$$(@:%o=%d) -c -o $$@ $$<

$$(ASM_OBJECTS_$(1)): $(OBJ)/$(1)_%.o: %.S Makefile $(OBJ) $(BIN)
	$(CC) $(ASFLAGS) -D$(1) -c -o $$@ $$<

debug_$(1): $(1)
	@echo "Starting a remote gdb session."
	./opt/gdb/debug.sh $(OUTPUT)-$(1).elf

endef

ifeq (flash, $(firstword $(MAKECMDGOALS)))
	FLASH_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
endif

.PHONY: nand ram jlink flash connect clean

jlink: sdram
	@echo "Starting a J-Link connection."
	./opt/jlink/jlink.sh

connect:
	@echo "Connectiong to debug port."
	./opt/tty/connect.sh

flash: sdram
	@echo "Flashing binary to "$(FLASH_ARGS)"."
	./opt/flashing/flash.sh $(FLASH_ARGS)

nand:
	@:

ram :
	@:

$(foreach MEMORY, $(MEMORIES), $(eval $(call RULES,$(MEMORY))))

clean:
	-rm -f $(OBJ)/*.o $(OBJ)/*.d $(BIN)/*.bin $(BIN)/*.elf $(BIN)/*.map flash.log cmd.gdb


