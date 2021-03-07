MOD_NAME = 
EXE_NAME = fbltool
LIB_NAME = fbltool

# Source Directories
PRJDIR  = .
MKDIR   = $(PRJDIR)/../../mk
EXTDIR  = $(PRJDIR)/../../ext
APPDIR  = $(PRJDIR)/../../application
DRVDIR  = $(PRJDIR)/../../driver
SERVDIR = $(PRJDIR)/../../service
CMNDIR  = $(PRJDIR)/../../common

SRCDIR  = $(PRJDIR)
SRCDIR += $(EXTDIR)/getopt
SRCDIR += $(SERVDIR)/libc
SRCDIR += $(SERVDIR)/cli

INCDIR  = .
INCDIR += $(CMNDIR)                # bsp.h, reg.h, typedefs.h
INCDIR += $(CMNDIR)/generic        # svc_call.h

INCDIR += $(PRJDIR)
INCDIR += $(EXTDIR)/getopt
INCDIR += $(SERVDIR)/defopt
INCDIR += $(SERVDIR)/libc
INCDIR += $(SERVDIR)/cli

ASMDIR  = . 
LIBDIR  =
LINKDIR = $(DRVDIR)/link


ifeq ($(PLATFORM), MACOS)
  TOOLSET = GCC

  CROSSCOMPILE =
  CROSSDEPEND =
  CROSSOBJCOPY =

  MCUDIR  = macos
  
  SRC     += sock.c
  SRC_EXE += main.c

  SRC_API    = 
  SRC_DESIGN = 
  SRC_COVER  = 


  MCU ?=

  CFLAGS   += -c -std=gnu99 -Wall
  ASFLAGS  += -ahls
  CPFLAGS  +=
  LFLAGS   += #-static

  INCLUDES += 
  DEFINES  += -DBSP_BOARD_TYPE=BSP_BRD_NONE -DBSP_SOC_TYPE=BSP_SOC_GENERIC -DBSP_CPU_TYPE=BSP_CPU_X86
  DEFINES  +=

endif # PLATFORM is MACOS
PLATFORMS += MACOS-exe


ifeq ($(PLATFORM), LINUX)
  TOOLSET = GCC

  CROSSCOMPILE =
  CROSSDEPEND =
  CROSSOBJCOPY =

  MCUDIR  = linux

  SRC     +=
  SRC_EXE += main.c

  SRC_API    = 
  SRC_DESIGN = 
  SRC_COVER  = 


  MCU ?=

  CFLAGS   += -c -std=gnu99 -Wall
  ASFLAGS  += -ahls
  CPFLAGS  +=
  LFLAGS   += #-static

  INCLUDES += 
  DEFINES  += -DBSP_BOARD_TYPE=BSP_BRD_NONE -DBSP_SOC_TYPE=BSP_SOC_GENERIC -DBSP_CPU_TYPE=BSP_CPU_X86
  DEFINES  +=

endif # PLATFORM is LINUX
PLATFORMS += LINUX-exe


ifeq ($(PLATFORM), MINGW)
  TOOLSET = GCC

  CROSSCOMPILE =
  CROSSDEPEND =
  CROSSOBJCOPY =

  MCUDIR  = mingw
  
  INCDIR  += $(PRJDIR)/specific

  SRC     += getopt_long.c
  SRC_EXE += main.c
  SRC_EXE += libc.c
  SRC_EXE += cmdio.c
  SRC_EXE += cmdl.c
  SRC_EXE += cli.c
  SRC_EXE += fbl_cmd.c
  SRC_EXE += upd_seq.c
  
  INCDIR  += $(APPDIR)/fbl
  SRCDIR  += $(APPDIR)/fbl
  SRC_EXE += bcp.c
  
  INCDIR        += $(SERVDIR)/trace
  SRCDIR        += $(SERVDIR)/trace
  SRC_EXE       += trace_core.c
  SRC_EXE       += trace_stdout.c

  INCDIR  += $(SERVDIR)/dlcf
  SRCDIR  += $(SERVDIR)/dlcf
  SRC_EXE += dlcf.c
  SRC_EXE += dlcf_uart.c

  INCDIR  += $(SERVDIR)/crc
  SRCDIR  += $(SERVDIR)/crc
  SRC_EXE += crc16.c
  SRC_EXE += crc32.c

#  INCDIR  += $(DRVDIR)/uart
#  SRCDIR  += $(DRVDIR)/uart
  SRC_EXE += uart.c

  INCDIR  += $(DRVDIR)/timer
  SRCDIR  += $(DRVDIR)/timer
  SRC_EXE += win_sys_timer.c

  INCDIR  += $(DRVDIR)/hab

  SRC_API    = 
  SRC_DESIGN = 
  SRC_COVER  = 


  MCU ?=

  CFLAGS   += -c -std=gnu99 -Wall
  ASFLAGS  += -ahls
  CPFLAGS  +=
  LFLAGS   += -static

  INCLUDES += 
  DEFINES  += -DBSP_BOARD_TYPE=BSP_BRD_NONE -DBSP_SOC_TYPE=BSP_SOC_GENERIC -DBSP_CPU_TYPE=BSP_CPU_X86
  DEFINES  += -DWINVER=0x0502

endif # PLATFORM is MINGW
PLATFORMS += MINGW-exe


ifeq "$(PLATFORM)" "" # PLATFORM is not set
help:
	@ echo "Targets:"
	@ echo "all"
	@ echo "Options:"
	@ echo "DEBUG=1"
	@ echo
	@ echo "Parameters:"
	@ echo "PLATFORM=LINUX"
	@ echo "PLATFORM=MINGW"
	@ echo "PLATFORM=MACOS"
	
endif # PLATFORM

include $(MKDIR)/generic.mk
