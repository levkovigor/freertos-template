#	Generic makefile

#-------------------------------------------------------------------------------
#		User set options
#-------------------------------------------------------------------------------

CHIP	= at91sam9g20
BOARD	= at91sam9g20-ek

# Trace level used for compilation
# (can be overriden by adding TRACE_LEVEL=#number to the command-line)
# TRACE_LEVEL_DEBUG      5
# TRACE_LEVEL_INFO       4
# TRACE_LEVEL_WARNING    3
# TRACE_LEVEL_ERROR      2
# TRACE_LEVEL_FATAL      1
# TRACE_LEVEL_NO_TRACE   0
# TRACE_LEVEL	= 5

AT91LIB		= src/at91lib
FREERTOS	= src/freertos

# Output file basename
OUTPUT		= img

# Compile with specific chip features
CHIP_CORE	= arm926ej_s
CHIP_IP_MCI	= MCI_PDC
CHIP_IP_UDP	= USB_UDP
CHIP_IP_UHP	= USB_OHCI

# Compile for all available memories
MEMORIES 	= sdram

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
INCLUDES	= -I$(AT91LIB)/boards/$(BOARD)
INCLUDES	+= -I$(AT91LIB)/peripherals
INCLUDES	+= -I$(AT91LIB)/components
INCLUDES	+= -I$(AT91LIB)
INCLUDES	+= -I$(FREERTOS)/portable/GCC/ARM9_AT91SAM9G20
INCLUDES	+= -I$(FREERTOS)/portable/MemMang
INCLUDES	+= -I$(FREERTOS)/portable/
INCLUDES	+= -I$(FREERTOS)/include
INCLUDES	+= -I$(FREERTOS)
INCLUDES	+= -Isrc/

OPTIM	= -O3

CFLAGS	= -Wall -mlong-calls -ffunction-sections
CFLAGS	+= -fomit-frame-pointer -fno-strict-aliasing -fno-dwarf2-cfi-asm
CFLAGS  += -mthumb-interwork -D THUMB_INTERWORK
CFLAGS	+= -g $(OPTIM) $(INCLUDES) -D$(CHIP)
# -DTRACE_LEVE=$(TRACE_LEVEL)
ASFLAGS	= -Wall -g $(OPTIM) $(INCLUDES) -D$(CHIP) -D__ASSEMBLY__
LDFLAGS	= -mthumb -g $(OPTIM) -nostartfiles -Wl,--gc-sections

#-------------------------------------------------------------------------------
#		FILES
#-------------------------------------------------------------------------------

# Directories where source files can be found
UTILITY	= $(AT91LIB)/utility
PERIPH	= $(AT91LIB)/peripherals
BOARDS	= $(AT91LIB)/boards

PORT	= $(FREERTOS)/portable/GCC/ARM9_AT91SAM9G20
MEM_MGT	= $(FREERTOS)/portable/MemMang	

VPATH   = src/
VPATH	+= $(UTILITY)
VPATH	+= $(PERIPH)/dbgu
VPATH	+= $(PERIPH)/irq
VPATH	+= $(PERIPH)/pio
VPATH	+= $(PERIPH)/pit
VPATH	+= $(PERIPH)/tc
VPATH	+= $(PERIPH)/pmc
VPATH	+= $(PERIPH)/cp15
VPATH	+= $(PERIPH)/systicks
VPATH	+= $(BOARDS)/$(BOARD)
VPATH	+= $(BOARDS)/$(BOARD)/$(CHIP)
VPATH	+= $(MEM_MGT)
VPATH	+= $(PORT)
VPATH	+= $(FREERTOS)


# Objects build from C source files
C_OBJECTS	= main.o
C_OBJECTS	+= led.o
C_OBJECTS	+= stdio.o
C_OBJECTS	+= dbgu.o
C_OBJECTS	+= pio.o
C_OBJECTS	+= pio_it.o
C_OBJECTS	+= tc.o
C_OBJECTS	+= pmc.o
C_OBJECTS	+= board_lowlevel.o
C_OBJECTS	+= trace.o
C_OBJECTS	+= board_memories.o
C_OBJECTS	+= aic.o
C_OBJECTS	+= cp15.o
C_OBJECTS	+= pit.o
C_OBJECTS	+= port.o
C_OBJECTS	+= tasks.o
C_OBJECTS	+= queue.o
C_OBJECTS	+= event_groups.o
C_OBJECTS	+= list.o
C_OBJECTS	+= stream_buffer.o
C_OBJECTS	+= timers.o
C_OBJECTS	+= heap_2.o

# Objects build with ARM instruction set from C source files
ARM_OBJECTS 	= portISR.o


# Objects build form assembly source files
ASM_OBJECTS	= board_cstartup.o
ASM_OBJECTS	+= cp15_asm_gcc.o

#Append OBJ and BIN directories to output filename
OUTPUT	:= $(BIN)/$(OUTPUT)

#-------------------------------------------------------------------------------
#		Rules
#-------------------------------------------------------------------------------
all: $(BIN) $(OBJ) $(MEMORIES)

phony: flash clean

$(BIN) $(OBJ):
	mkdir $@

define RULES
ARM_OBJECTS_$(1) = $(addprefix $(OBJ)/$(1)_, $(ARM_OBJECTS))
C_OBJECTS_$(1) = $(addprefix $(OBJ)/$(1)_, $(C_OBJECTS))
ASM_OBJECTS_$(1) = $(addprefix $(OBJ)/$(1)_, $(ASM_OBJECTS))

$(1): $$(ARM_OBJECTS_$(1)) $$(ASM_OBJECTS_$(1)) $$(C_OBJECTS_$(1))
	$(CC) $(LDFLAGS) -T"$(AT91LIB)/boards/$(BOARD)/$(CHIP)/$$@.lds" \
	-o $(OUTPUT)-$$@.elf $$^
	$(OBJCOPY) -O binary $(OUTPUT)-$$@.elf $(OUTPUT)-$$@.bin
	$(SIZE) $$^ $(OUTPUT)-$$@.elf

$$(ARM_OBJECTS_$(1)): $(OBJ)/$(1)_%.o: %.c Makefile $(OBJ) $(BIN)
	$(CC) $(CFLAGS) -D$(1) -c -o $$@ $$<

$$(C_OBJECTS_$(1)): $(OBJ)/$(1)_%.o: %.c Makefile $(OBJ) $(BIN)
	$(CC) $(CFLAGS) -mthumb -D$(1) -c -o $$@ $$<

$$(ASM_OBJECTS_$(1)): $(OBJ)/$(1)_%.o: %.S Makefile $(OBJ) $(BIN)
	$(CC) $(ASFLAGS) -D$(1) -c -o $$@ $$<

debug_$(1): $(1)
	perl ../resources/gdb/debug.pl $(OUTPUT)-$(1).elf

endef

$(foreach MEMORY, $(MEMORIES), $(eval $(call RULES,$(MEMORY))))

flash:
	./src/flashing/flash.sh

clean:
	-rm -f $(OBJ)/*.o $(BIN)/*.bin $(BIN)/*.elf flash.log


