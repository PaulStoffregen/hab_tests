MOD_NAME = 
EXE_NAME = app
BIN_NAME = app
LIB_NAME =

# Source Directories
PRJDIR  = ../..
MKDIR   = $(PRJDIR)/../../mk
DRVDIR  = $(PRJDIR)/../../driver
SERVDIR = $(PRJDIR)/../../service
CMNDIR  = $(PRJDIR)/../../common
UTILDIR = $(PRJDIR)/../../tools


INCDIR  = .
INCDIR += ..
INCDIR += $(PRJDIR)
INCDIR += $(CMNDIR)                # bsp.h, reg.h, typedefs.h
INCDIR += $(CMNDIR)/generic        # svc_call.h
INCDIR += $(SERVDIR)/libc          # libc.h
INCDIR += $(DRVDIR)/inc            # imxrt.h
INCDIR += $(DRVDIR)/inc/arm/armv7m # armv7m_regmap.h
INCDIR += $(DRVDIR)/uart           # uart.h

ASMDIR  = ..
LINKDIR = ./link



ifeq ($(PLATFORM), IMXRT)
  MCUDIR = imxrt

  SRCDIR         =
  SRCDIR        += .
  SRCDIR        += ..

  SRC_EXE       += sw_release.c
  SRC_EXE       += swinfo.c

  SRC_EXE       += armv7m_entry.S
  SRC_EXE       += startup.c
  SRC_EXE       += main.c
  SRC_EXE       += misc.c

  INCDIR        += ../specific      # config.h, trace_cfg.h, trace_feature_cfg.h
  SRCDIR        += ../specific
  SRC_EXE       += flex_trc.c
  
  SRCDIR        += $(DRVDIR)/fwdesc/imx
  SRC_EXE       += ivt.c

  INCDIR        += $(DRVDIR)/hab
  SRCDIR        += $(DRVDIR)/hab
  SRC_EXE       += hab_api.c
  SRC_EXE       += hab_info.c

  INCDIR        += $(DRVDIR)/ocotp
  SRCDIR        += $(DRVDIR)/ocotp
  SRC_EXE       += ocotp_info.c
  SRC_EXE       += ocotp.c

  INCDIR        += $(DRVDIR)/timer/armv7m
  SRCDIR        += $(DRVDIR)/timer/armv7m
  SRC_EXE       += arm_sys_timer.c

  INCDIR        += $(DRVDIR)/iomux/imx6
  SRCDIR        += $(DRVDIR)/iomux/imx6
  SRC_EXE       += iomux.c

  INCDIR        += $(DRVDIR)/ccm
  SRCDIR        += $(DRVDIR)/ccm
  SRC_EXE       += ccm.c

  INCDIR        += $(DRVDIR)/gpio
  INCDIR        += $(DRVDIR)/gpio/specific
  SRCDIR        += $(DRVDIR)/gpio
  SRC_EXE       += gpio.c

  INCDIR        += $(SERVDIR)/trace
  SRCDIR        += $(SERVDIR)/trace
  SRC_EXE       += trace_core.c
  SRC_EXE       += trace_flex.c

  INCDIR        += $(SERVDIR)/libc
  SRCDIR        += $(SERVDIR)/libc
  SRC_EXE       += libc.c

  SRCDIR        += $(CMNDIR)/generic
  SRC_EXE       += cpu_asm.S

  SRC_LNK       += flash-app.ld.S

  LIBS          += 

  MCU ?= cortex-m4

  TARGET_OS = NONE
  ASM_EXT   = S
  OPTIMIZE  = 1
#  CXXFLAGS += -Wall -g -O2 -std=gnu++11 -mcpu=cortex-m4 -mthumb
  CFLAGS   += -c -std=gnu99 -Wall -mthumb
  ASFLAGS  += -mcpu=cortex-m4 -mthumb -Wall
  CPFLAGS  +=
  LFLAGS   += -Wl,-Map -Wl,$(MAPFILE) -nostdlib -T$(LINK_SCRIPT)

  INCLUDES += 
#  DEFINES  += -DBSP_BOARD_TYPE=BSP_BRD_TEENSY40
  DEFINES  += -DBSP_BOARD_TYPE=BSP_BRD_MIMXRT1060_EVK

endif # PLATFORM is IMXRT
PLATFORMS += IMXRT-exe
PLATFORMS += IMXRT-bin
PLATFORMS += IMXRT-hex


ifeq "$(PLATFORM)" "" # PLATFORM is not set
help:
	@ echo "Targets:"
	@ echo "exe"
	@ echo "bin"
	@ echo "hex"
	@ echo
	@ echo "Options:"
	@ echo "none"
	@ echo
	@ echo "Parameters:"
	@ echo "PLATFORM=IMXRT"
	
endif # PLATFORM

include $(MKDIR)/generic.mk
include $(MKDIR)/Makefile.version

SIGN_OPTS  = --image-type=MMC
SIGN_OPTS += --ivt-offs=0x0
SIGN_OPTS += --input-file=__out__/imxrt/boot.bin
SIGN_OPTS += --crc-offs=0x208
SIGN_OPTS += --srk-idx=0

SIGN_OPTS += --srk-tbl=SRK_table.bin
SIGN_OPTS += --noca-key=SRK1_sha256_2048_65537_v3_usr_crt.pem

postlink: bin
	@perl -w $(UTILDIR)/hab-sign/genSignCfg.pl $(SIGN_OPTS)
	@perl -w $(UTILDIR)/patchCrc/patchCrc.pl --input-file=__out__/imxrt/image_exp.bin --crc-offs=0x208 --output-file=__out__/imxrt/image_exp.fbl
