MOD_NAME = dlcf
EXE_NAME = dlcf_tester
LIB_NAME = dlcf

export STR_DOC_NAME   = DLCF
export STR_DOC_STATUS = Draft
export STR_DOC_VER    = v0.1


# Source Directories
PRJDIR  = .
MKDIR   = $(PRJDIR)/../../mk
EXTDIR  = $(PRJDIR)/../../ext
DRVDIR  = $(PRJDIR)/../../driver
SERVDIR = $(PRJDIR)/../../service
CMNDIR  = $(PRJDIR)/../../common
TMPLDIR = $(PRJDIR)/../../doc_templ

SRCDIR  = .
SRCDIR += ./test
SRCDIR += $(SERVDIR)/rbuf

INCDIR  = .
INCDIR += ./test
INCDIR += $(PRJDIR)
INCDIR += $(CMNDIR)
INCDIR += $(CMNDIR)/emb_inc
INCDIR += $(CMNDIR)/emb_trace
INCDIR += $(DRVDIR)/driver/util
INCDIR += $(SERVDIR)/test
INCDIR += $(SERVDIR)/rbuf

ASMDIR  =
LIBDIR  = 
LINKDIR = ../link


ifeq ($(PLATFORM), MINGW)
  TOOLSET = GCC

  CROSSCOMPILE =
  CROSSDEPEND =
  CROSSOBJCOPY =

  MCUDIR  = mingw

  SRC     += dlcf.c
  SRC_EXE += main.c
  SRC_EXE += rbuf.c
  SRC_EXE += dlcf_dev.c
  
  LIBS    += gcc

  MCU ?=

  CFLAGS   += -c -std=gnu99 -Wall
  ASFLAGS  += -ahls
  CPFLAGS  +=
  LFLAGS   += -static

  INCLUDES +=
  DEFINES  += -DBSP_BOARD_TYPE=BSP_BRD_NONE -DBSP_SOC_TYPE=BSP_SOC_GENERIC -DBSP_CPU_TYPE=BSP_CPU_X86

endif # MINGW
PLATFORMS += MINGW-exe
ifeq "$(PLATFORM)" "" # PLATFORM is not set
help:
	@ echo "Targets:"
	@ echo "all"
	@ echo "exe"
	@ echo "lib"
	@ echo
	@ echo "map"
	@ echo "hex"
	@ echo "bin"
	@ echo "Testing targets"
	@ echo "test"
	@ echo "cover"
	@ echo "Documentation targets"
	@ echo "apidoc"
	@ echo "designdoc"
	@ echo
	@ echo "Options:"
	@ echo "DEBUG=1"
	@ echo "COVER=1"
	@ echo "PROFLE=1"
	@ echo
	@ echo "Parameters:"
	@ echo "PLATFORM=MINGW"
endif # PLATFORM

include $(MKDIR)/generic.mk
