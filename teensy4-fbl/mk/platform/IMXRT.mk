ifeq ($(PLATFORM), IMXRT)
  TOOLSET = GCC
  # If not already specified somewhere else, the MCUDIR is set to imxrt
  MCUDIR    ?= imxrt
  MCU       ?= cortex-m7

  # If not already specified anywhere else, the ARCH is set to ARM
  ARCH      ?= ARM
  # If not already specified anywhere else, the TARGET_OS is set to LINUX
  TARGET_OS ?= NONE
  EXE_EXT   ?= elf
  BIN_EXT   ?= bin
  LIB_EXT   ?= a
  ASM_EXT   ?= S

  LIBS      +=

  OPTIMIZE  ?= 1
  CFLAGS    ?= -c -std=gnu99 -mthumb -Wall
  ASFLAGS   ?= -std=gnu99 -mthumb -Wall
  CPPFLAGS  ?= -mthumb
  LFLAGS    ?= -nostartfiles --static -nostdlib -T$(LINK_SCRIPT)
  HEXFLAGS  ?= -O ihex

  #DEFINES   += -DBSP_BOARD_TYPE=BSP_BRD_MIMXRT1060_EVK
  #DEFINES   += -DBSP_SOC_TYPE=BSP_SOC_IMXRT
  #DEFINES   += -DBSP_CPU_TYPE=BSP_CPU_CORTEX_M7

  CCOUTOPTS = -o $@
  ASOUTOPTS = -o $@
  LDOUTOPTS = -o $@
  AROUTOPTS = -rcs $@

ifneq "$(MCU)" "" # MCU is defined  
  MCUFLAGS = -mcpu=$(MCU)
endif # MCU

  CFLAGS   += -mlittle-endian -mthumb $(OPTFLAGS) $(GPROV) $(CCOV) $(DBGFLAGS) $(MCUFLAGS)
  ASFLAGS  += -mlittle-endian -mthumb $(GPROV) $(DBGFLAGS) $(MCUFLAGS)
  CPFLAGS  += -mlittle-endian $(OPTFLAGS) $(GPROV) $(DBGFLAGS) $(MCUFLAGS)
  LFLAGS   += -mlittle-endian $(GPROV) $(LCOV) $(DBGFLAGS) $(MCUFLAGS)

  ARM_MODE   = -marm -DTHUMB_MODE=0
  THUMB_MODE = -mthumb -DTHUMB_MODE=1

endif # PLATFORM is IMXRT
