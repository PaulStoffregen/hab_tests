ifeq ($(PLATFORM), NONE)
  MCUDIR  ?= none
  MCU ?=

  ARCH ?= NONE
  TARGET_OS  ?= NONE
  EXE_EXT = elf
  LIB_EXT = a

ifneq "$(MCU)" "" # MCU is defined  
  MCUFLAGS = -mmcu=$(MCU)
endif # MCU

  CFLAGS   += $(OPTFLAGS) $(GPROV) $(CCOV) $(DBGFLAGS) $(MCUFLAGS)
  ASFLAGS  += $(GPROV) $(DBGFLAGS) $(MCUFLAGS)
  CPFLAGS  += $(OPTFLAGS) $(GPROV) $(DBGFLAGS) $(MCUFLAGS)
  LFLAGS   += $(GPROV) $(LCOV) $(DBGFLAGS) $(MCUFLAGS)


endif # PLATFORM is NONE
