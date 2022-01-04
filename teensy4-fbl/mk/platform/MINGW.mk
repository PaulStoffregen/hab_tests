ifeq ($(PLATFORM), MINGW)
  # If not already specified somewhere else, the TOOLSET is set to GCC
  TOOLSET ?= GCC
  # If not already specified somewhere else, the MCUDIR is set to mingw
  MCUDIR  ?= mingw

  # If not already specified anywhere else, the ARCH is set to X86
  ARCH ?= X86
  # If not already specified anywhere else, the TARGET_OS is set to WIN32
  TARGET_OS  ?= WIN32
  EXE_EXT ?= exe
  LIB_EXT ?= a

  CCOUTOPTS = -o $@
  ASOUTOPTS = -o $@
  LDOUTOPTS = -o $@
  AROUTOPTS = -rcs $@

ifneq "$(MCU)" "" # MCU is defined  
  MCUFLAGS = -march=$(MCU)
endif # MCU

  CFLAGS   += $(OPTFLAGS) $(GPROV) $(CCOV) $(DBGFLAGS) $(MCUFLAGS)
  CXXFLAGS += $(OPTFLAGS) $(GPROV) $(CCOV) $(DBGFLAGS) $(MCUFLAGS)
  ASFLAGS  += $(GPROV) $(DBGFLAGS) $(MCUFLAGS)
  CPFLAGS  += $(OPTFLAGS) $(GPROV) $(DBGFLAGS) $(MCUFLAGS)
  LFLAGS   += $(GPROV) $(LCOV) $(DBGFLAGS) $(MCUFLAGS)

endif # PLATFORM is MINGW
