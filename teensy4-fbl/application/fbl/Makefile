MOD_NAME = FBL
EXE_NAME = fbl
BIN_NAME = fbl
LIB_NAME =


# Source Directories
PRJDIR  = .
MKDIR   = $(PRJDIR)/../../mk
EXTDIR  = $(PRJDIR)/../../ext
DRVDIR  = $(PRJDIR)/../../driver
SERVDIR = $(PRJDIR)/../../service
CMNDIR  = $(PRJDIR)/../../common
UTILDIR = $(PRJDIR)/../../tools

INCDIR  = .
INCDIR += $(CMNDIR)                # bsp.h, reg.h, typedefs.h
INCDIR += $(CMNDIR)/emb_inc        # emb_std_types.h
INCDIR += $(CMNDIR)/generic        # svc_call.h

INCDIR += $(DRVDIR)/irqc
INCDIR += $(DRVDIR)/sio
INCDIR += $(DRVDIR)/startup
INCDIR += $(DRVDIR)/exception
INCDIR += $(DRVDIR)/util
INCDIR += $(DRVDIR)/inc
INCDIR += $(DRVDIR)/inc/arm/armv7m


ASMDIR  = . $(DRVDIR)/startup

LIBDIR  = .
LINKDIR = ./link


ifeq ($(PLATFORM), IMXRT)
  MCUDIR = imxrt

  SRCDIR         =
  SRCDIR        += .

  SRC_EXE       += sw_release.c
  SRC_EXE       += swinfo.c

  INCDIR        += $(PRJDIR)/specific
  SRCDIR        += $(PRJDIR)/specific
  SRC_EXE       += flex_trc.c

  INCDIR        += $(DRVDIR)/startup/imxrt
  SRCDIR        += $(DRVDIR)/startup/imxrt
  SRC_EXE       += armv7m_entry.S
  SRC_EXE       += flexspi_config_tbl.c
  SRC_EXE       += startup.c
  SRC_EXE       += misc.c

  # IVT and HAB helper
  INCDIR        += $(DRVDIR)/hab            # hab.h, hab_api.h
  SRCDIR        += $(DRVDIR)/hab
  SRC_EXE       += hab_api.c
  SRC_EXE       += hab_info.c

  SRCDIR        += $(DRVDIR)/fwdesc/imx
  SRC_EXE       += ivt.c

  # OCOTP
  INCDIR        += $(DRVDIR)/ocotp
  SRCDIR        += $(DRVDIR)/ocotp
  SRC_EXE       += ocotp.c
  SRC_EXE       += ocotp_info.c

  # ROM API
  INCDIR        += $(DRVDIR)/rom_api        # rom_api.h
  SRCDIR        += $(DRVDIR)/rom_api
  SRC_EXE       += rom_api.c

  # IRQ Controller Driver
  INCDIR        += $(DRVDIR)/irqc
  SRCDIR        += $(DRVDIR)/irqc
  SRC           += irqc.c
  SRC           += arm_nvic.c

  # IO-Mux Driver
  INCDIR        += $(DRVDIR)/iomux/imx6
  SRCDIR        += $(DRVDIR)/iomux/imx6
  SRC           += iomux.c

  # GPIO Driver
  INCDIR        += $(DRVDIR)/gpio
  INCDIR        += $(DRVDIR)/gpio/specific
  SRCDIR        += $(DRVDIR)/gpio
  SRC           += gpio.c

  # Clock Control Module Driver
  INCDIR        += $(DRVDIR)/ccm
  SRCDIR        += $(DRVDIR)/ccm
  SRC_EXE       += ccm.c

  INCDIR        += $(DRVDIR)/timer/armv7m
  SRCDIR        += $(DRVDIR)/timer/armv7m
  SRC_EXE       += arm_sys_timer.c

  # External Flash Driver
  INCDIR        += $(DRVDIR)/ext_flash
  SRCDIR        += $(DRVDIR)/ext_flash
  SRC_EXE       += ext_flash.c

  INCDIR        += $(SERVDIR)/trace
  SRCDIR        += $(SERVDIR)/trace
  SRC_EXE       += trace_core.c
  SRC_EXE       += trace_flex.c

  INCDIR        += $(SERVDIR)/dlcf
  SRCDIR        += $(SERVDIR)/dlcf
  SRC_EXE       += dlcf.c
  SRC_EXE       += dlcf_uart.c
  
  INCDIR        += $(SERVDIR)/crc
  SRCDIR        += $(SERVDIR)/crc
  SRC_EXE       += crc16.c
  
  SRC_EXE       += main.c
  SRC_EXE       += cmdl.c
  SRC_EXE       += noinit.c
  SRC_EXE       += bmgr.c
  SRC_EXE       += bcp.c
  SRC_EXE       += fbl.c

  SRCDIR        += $(CMNDIR)/generic
  SRC_EXE       += cpu_asm.S

  INCDIR        += $(CMNDIR)/generic/armv7m
  SRCDIR        += $(CMNDIR)/generic/armv7m
  SRC_EXE       += cpu_irq.c

  # C-lib
  INCDIR        += $(SERVDIR)/libc
  SRCDIR        += $(SERVDIR)/libc
  SRC_EXE       += libc.c

  # Commandline interface
  INCDIR        += $(SERVDIR)/cli
  SRCDIR        += $(SERVDIR)/cli
  SRC_EXE       += cli.c

  # Ring buffer
  INCDIR        += $(SERVDIR)/rbuf
  SRCDIR        += $(SERVDIR)/rbuf
  SRC_EXE       += rbuf.c
  

  # UART driver
  INCDIR        += $(DRVDIR)/uart
  SRCDIR        += $(DRVDIR)/uart
  SRC_EXE       += uart.c
  SRC_EXE       += uart_irq.c

  INCDIR        += $(DRVDIR)/uart/imxrt/specific
  SRCDIR        += $(DRVDIR)/uart/imxrt/specific
  SRC_EXE       += uart_ddm.c

  # ALPHA/HEX helper
  INCDIR        += $(SERVDIR)/alpha_hex
  SRCDIR        += $(SERVDIR)/alpha_hex
  SRC_EXE       += alpha_hex.c

  SRC_LNK       += fbl.ld.S

  LIBS          +=



  MCU ?= cortex-m4
  #XTAL ?= ...

  TARGET_OS = NONE
  ASM_EXT   = S
  OPTIMIZE  = 1

  CFLAGS   += -c -std=gnu99 -Wall -mthumb -mcpu=cortex-m4
  ASFLAGS  += -std=gnu99 -Wall -mthumb -mcpu=cortex-m4
  CPFLAGS  +=
  LFLAGS   += -Wl,-Map -Wl,$(MAPFILE) -nostdlib -T$(LINK_SCRIPT)

  INCLUDES += 
  DEFINES  +=
#  DEFINES  += -DBSP_BOARD_TYPE=BSP_BRD_MIMXRT1060_EVK
  DEFINES  += -DBSP_BOARD_TYPE=BSP_BRD_TEENSY40

endif # PLATFORM is IMXRT
PLATFORMS += IMXRT-map
PLATFORMS += IMXRT-exe
PLATFORMS += IMXRT-bin
PLATFORMS += IMXRT-hex


ifeq "$(PLATFORM)" "" # PLATFORM is not set

help:
	@ echo "Targets:"
	@ echo "all"
	@ echo
	@ echo "map"
	@ echo
	@ echo "Options:"
	@ echo "DEBUG=1"
	@ echo
	@ echo "Parameters:"
	@ echo "PLATFORM=IMXRT"

endif # PLATFORM

include $(MKDIR)/generic.mk
include $(MKDIR)/Makefile.version


SIGN_OPTS  = --image-type=MMC
SIGN_OPTS += --ivt-offs=0x1000
SIGN_OPTS += --input-file=__out__/imxrt/fbl.bin
SIGN_OPTS += --crc-offs=0x208
SIGN_OPTS += --srk-idx=0
SIGN_OPTS += --srk-tbl=SRK_table.bin
SIGN_OPTS += --noca-key=SRK1_sha256_2048_65537_v3_usr_crt.pem

#SIGN_OPTS += --srk-tbl=imxrt-srk-table.bin
#SIGN_OPTS += --noca-key=imxrt-srk1-ca-cert.pem

#SIGN_OPTS += --srk-tbl=imxrt-srk-table.bin
#SIGN_OPTS += --csf-key=imxrt-csf1-cert.pem
#SIGN_OPTS += --img-key=imxrt-img1-cert.pem
#SIGN_OPTS += --imgk-slot=2

CRC_OPTS  = --crc-offs=0x208
CRC_OPTS += --input-file=__out__/imxrt/image_exp.bin
CRC_OPTS += --output-file=__out__/imxrt/image_exp.fbl

postlink: bin
	@perl -w $(UTILDIR)/hab-sign/genSignCfg.pl $(SIGN_OPTS)
	@perl -w $(UTILDIR)/patchCrc/patchCrc.pl $(CRC_OPTS)
