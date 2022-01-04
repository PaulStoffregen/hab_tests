ifeq ($(PLATFORM), MACOS)
  # If not already specified somewhere else, the TOOLSET is set to GCXX
  TOOLSET ?= GCXX
  # If not already specified somewhere else, the MCUDIR is set to macos
  MCUDIR  ?= macos

  # If not already specified anywhere else, the ARCH is set to X86
  ARCH ?= X86
  DEP_ARCH ?= $(ARCH)
  OBJCOPY_ARCH ?= $(ARCH)
  # If not already specified anywhere else, the TARGET_OS is set to MACOS
  TARGET_OS  ?= MACOS
  EXE_EXT ?=
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

endif # PLATFORM is MACOS
