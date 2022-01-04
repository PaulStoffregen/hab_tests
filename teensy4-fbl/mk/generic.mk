
###### Check make extra flags ######

ifneq "$(OPTIMIZE)" "" # OPTIMIZE is defined
  OPTFLAGS ?= -O$(OPTIMIZE)
endif # OPTIMIZE


DRV_LC := $(shell echo "$(subst :,,$(strip $(DRIVE)))" | tr '[:upper:]' '[:lower:]')
DRV_UC := $(shell echo "$(subst :,,$(strip $(DRIVE)))" | tr '[:lower:]' '[:upper:]')

ifeq "$(findstring ;,$(PATH))" ";"
# PATH variable contains ";"

# path-conv(dir)
define path-conv
$(if $(findstring /$(DRV_LC)/,$1), $(subst \,/,$(subst /$(DRV_LC)/,$(DRV_LC):/,$1)))
$(if $(findstring /$(DRV_UC)/,$1), $(subst \,/,$(subst /$(DRV_UC)/,$(DRV_UC):/,$1)))
endef

else

# path-conv(dir)
define path-conv
$(if $(findstring $(DRV_LC):\,$1), $(subst \,/,$(subst $(DRV_LC):\,/$(DRV_LC)/,$1)))
$(if $(findstring $(DRV_LC):/,$1), $(subst \,/,$(subst $(DRV_LC):/,/$(DRV_LC)/,$1)))
$(if $(findstring $(DRV_UC):\,$1), $(subst \,/,$(subst $(DRV_UC):\,/$(DRV_UC)/,$1)))
$(if $(findstring $(DRV_UC):/,$1), $(subst \,/,$(subst $(DRV_UC):/,/$(DRV_UC)/,$1)))
endef

endif # PATH


# Detect on which host system and which processor architecture the build is executed
ifeq "$(OS)" "Windows_NT"
  # WIN32
  HOST_EXEC_SUFFIX := .exe
else
  UNAME_S := $(shell uname -s)
  ifeq "$(UNAME_S)" "Linux"
  # LINUX OS
  HOST_EXEC_SUFFIX :=
  endif

  ifeq "$(UNAME_S)" "Darwin"
  # MAC OS
  HOST_EXEC_SUFFIX :=
  endif
endif



ifneq "$(PLATFORM)" "" # PLATFORM is defined
  include $(MKDIR)/platform/$(PLATFORM).mk
endif # PLATFORM

export PATH := $(strip $(call path-conv, $(TOOL_PATH/bin))):$(PATH)

# General tools
RM      = rm -rf             # remove
CP      = cp                 # copy
MAKE    = make -r $(SILENT)  # make
MD      = mkdir              # mkdir

OBJCOPY  = $(addsuffix $(HOST_EXEC_SUFFIX), objcopy)
DEPENDER = $(addsuffix $(HOST_EXEC_SUFFIX), gcc)
COMPILER = $(addsuffix $(HOST_EXEC_SUFFIX), gcc)

TARGET_OS ?= NONE
ARCH ?= ARM

# Compiler tools for GNU C compiler tools
ifeq ($(TOOLSET),GCC)
  CC      = $(CROSSCOMPILE)gcc#-$(CC_VERS)
  CXX     = $(CROSSCOMPILE)g++#-$(CC_VERS)
  LD      = $(CC)
  AR      = $(CROSSCOMPILE)ar
  AS      = $(CC) -c -x assembler-with-cpp
  CPP     = $(CC) -E
  MKDEP   = $(CROSSCOMPILE)gcc
  OCP     = $(CROSSCOMPILE)objcopy
  OBJDUMP = $(CROSSCOMPILE)objdump
  GCOV    = $(CROSSCOMPILE)gcov
  NM      = $(CROSSCOMPILE)nm
  SIZE    = $(CROSSCOMPILE)size

  MAP_OPTS = -n -S -l -s $< > $@
  INC_DIR  += $(TOOL_PATH)/$(patsubst %-,%,$(CROSSCOMPILE))/include
  INCLUDES += $(addprefix -I,$(INC_DIR))
  DEFINES  += -DPLATFORM=PF_$(PLATFORM)
endif # TOOLSET

# Compiler tools for GNU C++ compiler tools
ifeq ($(TOOLSET),GCXX)
  CXX     = $(CROSSCOMPILE)g++#-$(CC_VERS)
  CC      = $(CXX)
  LD      = $(CC)
  AR      = $(CROSSCOMPILE)ar
  AS      = $(CC) -c -x assembler-with-cpp
  CPP     = $(CC) -E
  MKDEP   = $(CROSSCOMPILE)g++ -objC++
  OCP     = $(CROSSCOMPILE)objcopy
  OBJDUMP = $(CROSSCOMPILE)objdump
  GCOV    = $(CROSSCOMPILE)gcov
  NM      = $(CROSSCOMPILE)nm
  SIZE    = $(CROSSCOMPILE)size

  MAP_OPTS = -n -S -l -s $< > $@
  INC_DIR  += $(TOOL_PATH)/$(patsubst %-,%,$(CROSSCOMPILE))/include
  INCLUDES += $(addprefix -I,$(INC_DIR))
endif # TOOLSET


# Where to find *.c / *.cpp sources
SRC_DIR  = $(foreach name,$(SRCDIR),$(name)/$(MCUDIR) $(name) )

# Where to find includes
INC_DIR += $(foreach name,$(INCDIR),$(name)/$(MCUDIR) $(name) )

# Where to find assembly sources
ASM_DIR  = $(foreach name,$(ASMDIR),$(name)/$(MCUDIR) $(name) )

# Where to find libs to be linked
LIB_DIR = $(foreach name,$(SRCDIR),$(name)/lib/$(MCUDIR) $(name)/lib $(name) ) $(LIBDIR)

# Where to find linker scripts
LINK_DIR = $(foreach name,$(LINKDIR),$(name)/$(MCUDIR))


OBJ_SUFFIX    = $(SEPARATOR)$(PROF_SUFFIX)$(COVER_SUFFIX)$(DEBUG_SUFFIX)

ifneq "$(PLATFORM)" "" # PLATFORM is set

  # Where to put objects in general
  OBJDIR   =  obj/$(MCUDIR)


ifneq "$(SRC)" "" # SRC is defined
  # Where to put common objects
  OBJDIR_CMN = $(OBJDIR)/common$(DBGDIR)
endif # SRC


ifneq "$(SRC_EXE)" "" # SRC_EXE is defined
  # Where to put exe objects
  OBJDIR_EXE =  $(OBJDIR)/exe$(DBGDIR)
endif # SRC_EXE


ifneq "$(SRC_LIB)" "" # SRC_LIB is defined
  # Where to put lib objects
  OBJDIR_LIB =  $(OBJDIR)/lib$(DBGDIR)
endif # SRC_LIB


ifneq "$(EXE_NAME)" "" # An EXE_NAME is defined
  # Where to put executables
  EXE_DIR  =  __out__/$(MCUDIR)
endif # EXE_NAME


ifneq "$(LIB_NAME)" "" # A LIB_NAME is defined
  # Where to put archives
  ARCH_DIR =  lib/$(MCUDIR)
endif # LIB_NAME


  # Where to put dependency files
  DEP_DIR  =  dep/$(MCUDIR)

endif # PLATFORM

HEX_NAME    ?= $(EXE_NAME)
BIN_NAME    ?= $(EXE_NAME)
SREC_NAME   ?= $(EXE_NAME)

export IMAGE_PATH  = $(IMAGE_DIR)

EXE_TARGET = $(EXE_NAME)$(OBJ_SUFFIX)$(addprefix .,$(EXE_EXT))
LIB_TARGET = $(LIB_PREFIX)$(LIB_NAME)$(OBJ_SUFFIX)$(addprefix .,$(LIB_EXT))

MAPFILE = $(MAP_DIR)/$(EXE_NAME)$(OBJ_SUFFIX).map
LINK_SCRIPT ?= $(addprefix $(LINK_DIR)/,$(subst .S,,$(SRC_LNK)))

HEX_TARGET  = $(HEX_NAME)$(OBJ_SUFFIX)$(addprefix .,$(HEX_EXT))
BIN_TARGET  = $(BIN_NAME)$(OBJ_SUFFIX)$(addprefix .,$(BIN_EXT))
SREC_TARGET = $(SREC_NAME)$(OBJ_SUFFIX)$(addprefix .,$(SREC_EXT))

LINK_DIR   ?= $(EXE_DIR)
HEX_DIR    ?= $(EXE_DIR)
BIN_DIR    ?= $(EXE_DIR)
SREC_DIR   ?= $(EXE_DIR)
MAP_DIR    ?= $(EXE_DIR)

DEP_EXT    ?= d
EXE_EXT    ?= elf
OBJ_EXT    ?= o
LIB_EXT    ?= a
ASM_EXT    ?= asm
HEX_EXT    ?= hex
BIN_EXT    ?= bin
SREC_EXT   ?= srec
LIB_PREFIX ?= lib

OBJDIRS    = $(OBJDIR) $(OBJDIR_CMN) $(OBJDIR_EXE) $(OBJDIR_LIB) $(DEP_DIR)
EXEDIRS    = $(sort $(EXE_DIR) $(MAP_DIR) $(HEX_DIR) $(BIN_DIR) $(SREC_DIR))
LIBDIRS    = $(ARCH_DIR)
ALLDIRS    = $(OBJDIR) $(OBJDIR_CMN) $(OBJDIR_EXE) $(OBJDIR_LIB) $(DEP_DIR) $(EXE_DIR) $(ARCH_DIR)

QUIET      = @

ODFLAGS	   = -x --syms


OBJS_CMN        = $(foreach file, $(SRC),$(dir $(file))$(OBJDIR_CMN)/$(patsubst %,%$(OBJ_SUFFIX).o,$(subst .c,,$(subst .cpp,,$(subst .mm,,$(subst .$(ASM_EXT),,$(notdir $(file))))))))
OBJS_EXE        = $(foreach file, $(SRC_EXE),$(dir $(file))$(OBJDIR_EXE)/$(patsubst %,%$(OBJ_SUFFIX).o,$(subst .c,,$(subst .cpp,,$(subst .mm,,$(subst .$(ASM_EXT),,$(notdir $(file))))))))
OBJS_LIB        = $(foreach file, $(SRC_LIB),$(dir $(file))$(OBJDIR_LIB)/$(patsubst %,%$(OBJ_SUFFIX).o,$(subst .c,,$(subst .cpp,,$(subst .mm,,$(subst .$(ASM_EXT),,$(notdir $(file))))))))

CMN_OBJS        = $(patsubst ./%,%,$(strip $(OBJS_CMN)))
EXE_OBJS        = $(patsubst ./%,%,$(strip $(OBJS_EXE)))
LIB_OBJS        = $(patsubst ./%,%,$(strip $(OBJS_LIB)))

COMPILE_OBJS    =  $(strip $(EXE_OBJS) $(CMN_OBJS) $(LIB_OBJS))

ALL_OBJS        =  $(strip $(COMPILE_OBJS) $(COV_OBJS))


# Search paths
vpath %.h $(INC_DIR)
vpath %.c $(SRC_DIR)
vpath %.cpp $(SRC_DIR)
vpath %.mm $(SRC_DIR)
vpath %.$(ASM_EXT) $(ASM_DIR) $(SRC_DIR)
vpath %.d $(DEP_DIR)
vpath %.ld $(LINK_DIR)


LIB-PLATFORMS = $(subst -lib,,$(filter %-lib,$(PLATFORMS)))
EXE-PLATFORMS = $(subst -exe,,$(filter %-exe,$(PLATFORMS)))
MAP-PLATFORMS = $(subst -exe,,$(filter %-exe,$(PLATFORMS)))
BIN-PLATFORMS = $(subst -bin,,$(filter %-bin,$(PLATFORMS)))
HEX-PLATFORMS = $(subst -hex,,$(filter %-hex,$(PLATFORMS)))
SREC-PLATFORMS = $(subst -srec,,$(filter %-srec,$(PLATFORMS)))
CLEAN-PLATFORMS = $(basename $(subst -,.,$(PLATFORMS)))


DEPENDENCIES    = $(patsubst %.$(ASM_EXT),./$(DEP_DIR)/%.d,$(notdir $(SRC_LNK)))
DEPENDENCIES   += $(patsubst %.o,./$(DEP_DIR)/%.d,$(notdir $(COMPILE_OBJS)))
LNK_DEPS       := %.ld.d
OBJ_DEPS       := %$(OBJ_SUFFIX).d


# Definition of recipes
ifeq ($(TOOLSET),GCC) # TOOLSET is GCC
MAKEDEP_C       = $(QUIET) $(MKDEP) $(CFLAGS) $(DEFINES) $(INCLUDES) -MQ $@ -MM -MP -MF $(DEP_DIR)/$*.d $<
MAKEDEP_S       = $(QUIET) $(MKDEP) $(CFLAGS) $(DEFINES) $(INCLUDES) -MQ $@ -MM -MP -MF $(DEP_DIR)/$*.d $<
MAKEDEP_LDS     = $(QUIET) $(MKDEP) $(CFLAGS) $(DEFINES) $(INCLUDES) -MQ $@ -MM -MP -MF $(DEP_DIR)/$*.ld.d $<
endif # TOOLSET

ifeq ($(TOOLSET),GCXX) # TOOLSET is GCXX
MAKEDEP_OCXX    = $(QUIET) $(MKDEP) $(CXXFLAGS) $(DEFINES) $(INCLUDES) -MQ $@ -MM -MP -MF $(DEP_DIR)/$*.d $<
MAKEDEP_CXX     = $(QUIET) $(MKDEP) $(CXXFLAGS) $(DEFINES) $(INCLUDES) -MQ $@ -MM -MP -MF $(DEP_DIR)/$*.d $<
MAKEDEP_C       = $(QUIET) $(MKDEP) $(CFLAGS) $(DEFINES) $(INCLUDES) -MQ $@ -MM -MP -MF $(DEP_DIR)/$*.d $<
MAKEDEP_S       = $(QUIET) $(MKDEP) $(CFLAGS) $(DEFINES) $(INCLUDES) -MQ $@ -MM -MP -MF $(DEP_DIR)/$*.d $<
MAKEDEP_LDS     = $(QUIET) $(MKDEP) $(CFLAGS) $(DEFINES) $(INCLUDES) -MQ $@ -MM -MP -MF $(DEP_DIR)/$*.ld.d $<
endif # TOOLSET


ifeq ($(TOOLSET),GCC) # TOOLSET is GCC
PREPROC_C       = $(QUIET) $(CPP) $(CFLAGS) $(DEFINES) $(INCLUDES) $< $(CCOUTOPTS)
PREPROC_S       = $(QUIET) $(AS) $(ASFLAGS) $(DEFINES) $(INCLUDES) $< $(ASOUTOPTS)
PREPROC_LD      = $(QUIET) $(CPP) -P $(CPPFLAGS) $(MCUFLAGS) $(DEFINES) $(INCLUDES) $< -o $@
endif # TOOLSET

ifeq ($(TOOLSET),GCXX) # TOOLSET is GCXX
PREPROC_C       = $(QUIET) $(CPP) $(CFLAGS) $(DEFINES) $(INCLUDES) $< $(CCOUTOPTS)
PREPROC_CXX     = $(QUIET) $(CPP) $(CXXFLAGS) $(DEFINES) $(INCLUDES) $< $(CCOUTOPTS)
PREPROC_S       = $(QUIET) $(AS) $(ASFLAGS) $(DEFINES) $(INCLUDES) $< $(ASOUTOPTS)
PREPROC_LD      = $(QUIET) $(CPP) -P $(CPPFLAGS) $(MCUFLAGS) $(DEFINES) $(INCLUDES) $< -o $@
endif # TOOLSET

ifeq ($(TOOLSET),GCC) # TOOLSET is GCC
COMPILE_C       = $(QUIET) $(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) $< $(CCOUTOPTS)
COMPILE_S       = $(QUIET) $(AS) $(ASFLAGS) $(DEFINES) $(INCLUDES) $< $(ASOUTOPTS)
COMPILE_CXX     = $(QUIET) $(CXX) $(CXXFLAGS) $(DEFINES) $(INCLUDES) $< $(CCOUTOPTS)
endif # TOOLSET

ifeq ($(TOOLSET),GCXX) # TOOLSET is GCXX
COMPILE_OCXX    = $(QUIET) $(CXX) -objC++ $(CXXFLAGS) $(DEFINES) $(INCLUDES) $< $(CCOUTOPTS)
COMPILE_CXX     = $(QUIET) $(CXX) $(CXXFLAGS) $(DEFINES) $(INCLUDES) $< $(CCOUTOPTS)
COMPILE_C       = $(QUIET) $(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) $< $(CCOUTOPTS)
COMPILE_S       = $(QUIET) $(AS) $(ASFLAGS) $(DEFINES) $(INCLUDES) $< $(ASOUTOPTS)
endif # TOOLSET


ifeq ($(TOOLSET),GCC) # TOOLSET is GCC
LINK_O         ?= $(QUIET) $(LD) $(LFLAGS) $(addprefix -L,$(ARCH_DIR) $(LIB_DIR)) $(LDOUTOPTS) $(EXE_OBJS) $(CMN_OBJS) $(addprefix -l,$(LIBS))
endif # TOOLSET

ifeq ($(TOOLSET),GCXX) # TOOLSET is GCXX
LINK_O         ?= $(QUIET) $(LD) $(LFLAGS) $(addprefix -L,$(ARCH_DIR) $(LIB_DIR)) $(LDOUTOPTS) $(EXE_OBJS) $(CMN_OBJS) $(addprefix -l,$(LIBS))
endif # TOOLSET


ifeq ($(TOOLSET),GCC) # TOOLSET is GCC
ARCH_O          = $(QUIET) $(AR) $(AROUTOPTS) $(LIB_OBJS) $(CMN_OBJS)
endif # TOOLSET

ifeq ($(TOOLSET),GCXX) # TOOLSET is GCXX
ARCH_O          = $(QUIET) $(AR) $(AROUTOPTS) $(LIB_OBJS) $(CMN_OBJS)
endif # TOOLSET


# mk-lib(platform)
define mk-lib
echo ""

echo "...building $1 library"

$(if $(findstring MSVC,$(1)), cmd /k "$(subst /,\,$(MKDIR))\mmake.bat PLATFORM=$1 mklib", $(MAKE) PLATFORM=$1 mklib)
#### keep this empty line below ####

endef # mk-lib

 
# mk-exe(platform)
define mk-exe
echo ""
echo "...building $1 executable"
$(if $(findstring MSVC,$(1)), cmd /k "$(subst /,\,$(MKDIR))\mmake.bat PLATFORM=$1 mkexe", $(MAKE) PLATFORM=$1 mkexe)
#### keep this empty line below ####

endef # mk-exe


# mk-bin(platform)
define mk-bin
echo ""
echo "...building $1 binary"
$(if $(findstring MSVC,$(1)), cmd /k "$(subst /,\,$(MKDIR))\mmake.bat PLATFORM=$1 mkbin", $(MAKE) PLATFORM=$1 mkbin)
#### keep this empty line below ####

endef # mk-bin


# mk-hex(platform)
define mk-hex
echo ""
echo "...building $1 hex file"
$(if $(findstring MSVC,$(1)), cmd /k "$(subst /,\,$(MKDIR))\mmake.bat PLATFORM=$1 mkhex", $(MAKE) PLATFORM=$1 mkhex)
#### keep this empty line below ####

endef # mk-hex


# mk-srec(platform)
define mk-srec
echo ""
echo "...building $1 S-record"
$(if $(findstring MSVC,$(1)), cmd /k "$(subst /,\,$(MKDIR))\mmake.bat PLATFORM=$1 mksrec", $(MAKE) PLATFORM=$1 mksrec)
#### keep this empty line below ####

endef # mk-srec


# mk-map(platform)
define mk-map
echo ""
echo "...creating $1 mapfile"
$(if $(findstring MSVC,$(1)), cmd /k "$(subst /,\,$(MKDIR))\mmake.bat PLATFORM=$1 mkmap", $(MAKE) PLATFORM=$1 mkmap)
#### keep this empty line below ####

endef # mk-map


# mk-clean(platform)
define mk-clean
echo ""
echo "...cleaning $1"
$(MAKE) PLATFORM=$1 mkclean
#### keep this empty line below ####

endef # mk-clean


# mk-none(platfrom)
define mk-none
echo "";
echo "...node $1";
#### keep this empty line below ####

endef # mk-none



ifeq "$(PLATFORM)" "" # PLATFORM is not set

# As the PLATFORM is not defined, build for all platforms
# mk-traget(target, platforms)
define mk-target

#@echo "mk-target TRG=$1, PFs=$2, PF=none"

$(if $(findstring lib,$1),   $(foreach pf,$(sort $(patsubst %-lib,%,$(patsubst %-exe, ,$2))),$(call mk-lib,$(pf))),#@echo "no lib")
$(if $(findstring exe,$1),   $(foreach pf,$(sort $2),$(call mk-exe,$(pf))),#@echo "no exe")
$(if $(findstring map,$1),   $(foreach pf,$(sort $2),$(call mk-map,$(pf))),#@echo "no map")
$(if $(findstring bin,$1),   $(foreach pf,$(sort $2),$(call mk-bin,$(pf))),#@echo "no bin")
$(if $(findstring hex,$1),   $(foreach pf,$(sort $2),$(call mk-hex,$(pf))),#@echo "no hex")
$(if $(findstring srec,$1),  $(foreach pf,$(sort $2),$(call mk-srec,$(pf))),#@echo "no srec")
$(if $(findstring clean,$1), $(foreach pf,$(sort $2),$(call mk-clean,$(pf))),#@echo "no clean")
endef # mk-target

else # PLATFORM is set

# As the PLATFORM is defined, build for only this platform
# mk-target(target, platforms)
define mk-target

#@echo "mk-target TRG=$1, PFS=$2, PF=$(PLATFORM)"

# If "lib" is the first arg (target) and the second arg (platforms) contains
# the string combined of the defined platform $(PLATFORM) suffixed by "-lib",
# then call macro mk-lib with the defined platform $(PLATFORM) as arg
$(if $(findstring lib,$1),   $(if $(findstring $(PLATFORM), $2), $(call mk-lib,$(PLATFORM)),),)

# If "exe" is the first arg (target) and the second arg (platforms) contains
# the string combined of the defined platform $(PLATFORM) suffixed by "-exe",
# then call macro mk-exe with the defined platform $(PLATFORM) as arg
$(if $(findstring exe,$1),   $(if $(filter $(PLATFORM), $2),     $(call mk-exe,$(PLATFORM)),),)

# If "map" is the first arg (target) and the second arg (platforms) contains
# the string combined of the defined platform $(PLATFORM) suffixed by "-map",
# then call macro mk-map with the defined platform $(PLATFORM) as arg
$(if $(findstring map,$1),   $(if $(filter $(PLATFORM), $2),     $(call mk-map,$(PLATFORM)),),)

# If "bin" is the first arg (target) and the second arg (platforms) contains
# the string combined of the defined platform $(PLATFORM) suffixed by "-bin",
# then call macro mk-bin with the defined platform $(PLATFORM) as arg
$(if $(findstring bin,$1),   $(if $(filter $(PLATFORM), $2),     $(call mk-bin,$(PLATFORM)),),)

# If "hex" is the first arg (target) and the second arg (platforms) contains
# the string combined of the defined platform $(PLATFORM) suffixed by "-hex",
# then call macro mk-hex with the defined platform $(PLATFORM) as arg
$(if $(findstring hex,$1),   $(if $(filter $(PLATFORM), $2),     $(call mk-hex,$(PLATFORM)),),)

# If "srec" is the first arg (target) and the second arg (platforms) contains
# the string combined of the defined platform $(PLATFORM) suffixed by "-srec",
# then call macro mk-srec with the defined platform $(PLATFORM) as arg
$(if $(findstring srec,$1),   $(if $(filter $(PLATFORM), $2),    $(call mk-srec,$(PLATFORM)),),)

# If "apidoc" is the first arg (target) and the second arg (platforms) contains
# the string combined of the defined platform $(PLATFORM) suffixed by "-apidoc",
# then call macro mk-apidoc with the defined platform $(PLATFORM) as arg
$(if $(findstring apidoc,$1), $(if $(filter $(PLATFORM), $2),    $(call mk-apidoc,$(PLATFORM)),),)

# If "test" is the first arg (target) and the second arg (platforms) contains
# the string combined of the defined platform $(PLATFORM) suffixed by "-test",
# then call macro mk-test with the defined platform $(PLATFORM) as arg
$(if $(findstring test,$1), $(if $(filter $(PLATFORM), $2),      $(call mk-test,$(PLATFORM)),),)

# If "cover" is the first arg (target) and the second arg (platforms) contains
# the string combined of the defined platform $(PLATFORM) suffixed by "-cover",
# then call macro mk-cover with the defined platform $(PLATFORM) as arg
$(if $(findstring cover,$1), $(if $(filter $(PLATFORM), $2),     $(call mk-cover,$(PLATFORM)),),)

# If "clean" is the first arg (target) and the second arg (platforms) contains
# the string combined of the defined platform $(PLATFORM) suffixed by "-<anything>",
# then call macro mk-clean with the defined platform $(PLATFORM) as arg
$(if $(findstring clean,$1), $(if $(filter $(PLATFORM), $2),     $(call mk-clean,$(PLATFORM)),),)

endef # mk-target

endif # PLATFORM



.PHONY: lib exe map test cover help clean bin hex srec

all: lib exe

lib:
	@ $(call mk-target,$@,$(LIB-PLATFORMS))

exe:
	@ $(call mk-target,$@,$(EXE-PLATFORMS))

bin:
	@ $(call mk-target,$@,$(BIN-PLATFORMS))

hex:
	@ $(call mk-target,$@,$(HEX-PLATFORMS))

srec:
	@ $(call mk-target,$@,$(SREC-PLATFORMS))

map:
	@ $(call mk-target,$@,$(MAP-PLATFORMS))

depend: $(DEPENDENCIES)

clean:
	@ echo Making clean
	@ $(call mk-target,$@,$(CLEAN-PLATFORMS))

help-platform:
	@ echo -n "Supported Platforms:"
	@ echo -e "$(foreach str,$(ALL_PLATFORMS),\n $(str))"
	@ echo -n "Platform Targets:"
	@ echo -e "$(foreach str,$(PLATFORMS),\n $(str))"
	@ echo -n "EXE-Platforms:"
	@ echo -e "$(foreach str,$(EXE-PLATFORMS),\n $(str))"


ifneq "$(PLATFORM)" "" # PLATFORM is set

ifneq "$(LIB_NAME)" "" # LIB_NAME is defined
mklib: $(ARCH_DIR)/$(LIB_TARGET)
else
mklib:
	@echo "No lib name defined!"
	@echo "Define LIB_NAME in your makefile!"
endif # LIB_NAME


ifneq "$(EXE_NAME)" "" # EXE_NAME is defined
mkexe: $(EXE_DIR)/$(EXE_TARGET)
else
mkexe:
	@echo "No exe name defined!"
	@echo "Define EXE_NAME in your makefile!"
endif # EXE_NAME


ifneq "$(EXE_NAME)" "" # EXE_NAME is defined
mkmap: $(MAPFILE)
endif # EXE_NAME


ifneq "$(EXE_NAME)" "" # EXE_NAME is defined
mkbin: $(BIN_DIR)/$(BIN_TARGET)
endif # EXE_NAME


ifneq "$(EXE_NAME)" "" # EXE_NAME is defined
mkhex: $(HEX_DIR)/$(HEX_TARGET)
endif # EXE_NAME


ifneq "$(EXE_NAME)" "" # EXE_NAME is defined
mksrec: $(SREC_DIR)/$(SREC_TARGET)
endif # EXE_NAME


mkclean:
	$(QUIET)-rm -f $(ALL_OBJS) $(COV_FILES)
	$(QUIET)-rm -f $(LINK_DIR)/*.ld
	$(QUIET)-rm -f $(DEP_DIR)/*.d
	$(QUIET)-rm -f $(EXE_DIR)/*.$(EXE_EXT) $(EXE_DIR)/*.map $(EXE_DIR)/*.lst $(EXE_DIR)/*.$(HEX_EXT) $(EXE_DIR)/*.$(BIN_EXT)

endif # PLATFORM


mrproper: clean
	$(QUIET) rm -rf ./dep
	$(QUIET) rm -rf ./obj
	$(QUIET) rm -rf ./lib


# dump-var(name)
define dump-var
@ echo "$$""($1) = <$($1)>"
endef

# dump-vars()
define dump-vars
@ $(MAKE) --print-data-base -q |  \
  gawk  '/^[-A-Za-z0-9_]+ [?:]*=/ \
          { print "<"$$1">" }' |  \
  sort
endef

# dump-targets()
define dump-targets
@ $(MAKE) --print-data-base -q |                      \
  gawk  '/^[^.%][-A-Za-z0-9_]*:[^\/]/                 \
        {                                             \
          gsub(/\r/,"");                              \
          print "<"substr($$1, 1, length($$1) - 1)">" \
        }' |                                          \
  sort
endef

tool-echo:
	@ echo "Tools:"
	$(call dump-var,PLATFORM)
	$(call dump-var,TOOL_SELECT)
	$(call dump-var,TOOLSET)
	$(call dump-var,COMPILER)
	$(call dump-var,CXX)
	$(call dump-var,CC)
	$(call dump-var,AS)
	$(call dump-var,LD)
	$(call dump-var,AR)
	$(call dump-var,TOOL_PATH)
	$(call dump-var,MKDEP)
	$(call dump-var,ARCH)
	$(call dump-var,TARGET_OS)
	$(call dump-var,CROSSCOMPILE)
	$(call dump-var,DEP_ARCH)
	$(call dump-var,CROSSDEPEND)
	$(call dump-var,OBJCOPY_ARCH)
	$(call dump-var,CROSSOBJCOPY)

dep-echo:
	@ echo "DEP-build:"
	$(call dump-var,PLATFORM)
	$(call dump-var,DEPENDENCIES)
	@ echo "$$""(DEP_DIR)      = <$(DEP_DIR)>"
	@ echo "$$""(SRC_DIR)      = <$(SRC_DIR)>"
	@ echo "$$""(INC_DIR)      = <$(INC_DIR)>"
	@ echo "$$""(MCU_DIR)      = <$(MCUDIR)>"
	@ echo "$$""(TOOL_PATH)    = <$(TOOL_PATH)>"

exe-echo:
	@ echo "EXE-build:"
	@ echo "$$""(PLATFORM)   = <$(PLATFORM)>"
	@ echo "$$""(EXE_DIR)    = <$(EXE_DIR)>"
	@ echo "$$""(EXE_TARGET) = <$(EXE_TARGET)>"
	@ echo "$$""(SRC_EXE)    = <$(SRC_EXE)>"
	@ echo "$$""(SRC_CMN)    = <$(SRC_CMN)>"
	@ echo "$$""(OBJS_EXE)   = <$(OBJS_EXE)>"
	@ echo "$$""(LIBS)       = <$(LIBS)>"
	@ echo "$$""(EXE_OBJS)   = <$(EXE_OBJS)>"
	@ echo "$$""(OBJDIR_EXE) = <$(OBJDIR_EXE)>"
	@ echo "$$""(EXEDIRS)    = <$(EXEDIRS)>"
	@ echo "$$""(OBJDIRS)    = <$(OBJDIRS)>"
	@ echo "$$""(DEP_DIR)    = <$(DEP_DIR)>"
	@ echo "$$""(ASM_DIR)    = <$(ASM_DIR)>"
	@ echo "$$""(SRC_DIR)    = <$(SRC_DIR)>"
	@ echo "$$""(INC_DIR)    = <$(INC_DIR)>"
	@ echo "$$""(MCU_DIR)    = <$(MCUDIR)>"
	@ echo "$$""(LIB_DIR)    = <$(LIB_DIR)>"
	@ echo "$$""(TOOL_PATH)  = <$(TOOL_PATH)>"
	@ echo "$$""(LINK_SCRIPT)= <$(LINK_SCRIPT)>"
#	@ echo "$(path)"

lib-echo:
	@ echo "LIB-build:"
	@ echo "$$""(PLATFORM)   = <$(PLATFORM)>"
	@ echo "$$""(ARCH_DIR)   = <$(ARCH_DIR)>"
	@ echo "$$""(LIB_TARGET) = <$(LIB_TARGET)>"
	@ echo "$$""(LIB_OBJS)   = <$(LIB_OBJS)>"
	@ echo "$$""(SRC_LIB)    = <$(SRC_LIB)>"
	@ echo "$$""(SRC_CMN)    = <$(SRC_CMN)>"
	@ echo "$$""(OBJS_LIB)   = <$(OBJS_LIB)>"
	@ echo "$$""(OBJDIR)     = <$(OBJDIR)>"
	@ echo "$$""(DEP_DIR)    = <$(DEP_DIR)>"
	@ echo "$$""(ASM_DIR)    = <$(ASM_DIR)>"
	@ echo "$$""(SRC_DIR)    = <$(SRC_DIR)>"
	@ echo "$$""(INC_DIR)    = <$(INC_DIR)>"
	@ echo "$$""(MCU_DIR)    = <$(MCUDIR)>"
	@ echo "$$""(TOOL_PATH)  = <$(TOOL_PATH)>"
#	@ echo "$(path)"


bin-echo:
	@ echo "$$""(BIN_TARGET) = <$(BIN_TARGET)>"
	@ echo "$$""(BIN_NAME)   = <$(BIN_NAME)>"
	@ echo "$$""(BIN_EXT)    = <$(BIN_EXT)>"

hex-echo:
	@ echo "$$""(HEX_TARGET) = <$(HEX_TARGET)>"
	@ echo "$$""(HEX_NAME)   = <$(HEX_NAME)>"
	@ echo "$$""(BIN_EXT)    = <$(HEX_EXT)>"
	
srec-echo:
	@ echo "$$""(SREC_TARGET) = <$(SREC_TARGET)>"
	@ echo "$$""(SREC_NAME)   = <$(SREC_NAME)>"
	@ echo "$$""(SREC_EXT)    = <$(SREC_EXT)>"

dirs-echo:
	@ echo "$$""(ALLDIRS)    = <$(ALLDIRS)>"	


ifneq ($(SHELL),cmd) # SHELL is not CMD
$(OBJDIRS):
	@ echo "... creating directory $@"
	$(QUIET) mkdir -p $@

$(EXEDIRS):
	@ echo "... creating directory $@"
	$(QUIET) mkdir -p $@

$(LIBDIRS):
	@ echo "... creating directory $@"
	$(QUIET) mkdir -p $@

$(LINK_DIR):
	@ echo "... creating directory $@"
	$(QUIET) mkdir -p $@

else # SHELL is not CMD

$(OBJDIRS):
	@ echo "... creating directory $@"
	$(QUIET) md $(subst /,\,$@)

$(EXEDIRS):
	@ echo "... creating directory $@"
	$(QUIET) md $(subst /,\,$@)

$(LIBDIRS):
	@ echo "... creating directory $@"
	$(QUIET) md $(subst /,\,$@)
	
$(LINK_DIR):
	@ echo "... creating directory $<"
	$(QUIET) md $(subst /,\,$@)

endif # SHELL is CMD



$(LINK_DIR)/%.ld: $(SRC_LNK) $(LINK_DIR)
	@echo "(MKDEP) $<"
	$(MAKEDEP_LDS)
	@echo "(CPP)   $<"
	$(PREPROC_LD)

$(ARCH_DIR)/$(LIB_TARGET): $(OBJDIRS) $(LIBDIRS) $(CMN_OBJS) $(LIB_OBJS)
	@ echo "... archiving $@"
	$(QUIET) rm -f $@
	$(ARCH_O)

$(EXE_DIR)/$(EXE_TARGET): $(OBJDIRS) $(EXEDIRS) $(CMN_OBJS) $(EXE_OBJS) $(LINK_SCRIPT)
	@ echo "... linking $@"
	$(LINK_O)

$(HEX_DIR)/%.$(HEX_EXT): $(EXE_DIR)/$(EXE_TARGET)
	@ echo "... creating $@"
	$(QUIET) $(OCP) $(HEXFLAGS) $< $@

$(BIN_DIR)/%$(addprefix .,$(BIN_EXT)): $(EXE_DIR)/$(EXE_TARGET)
	@ echo "... creating $@"
	$(QUIET) $(OCP) -I elf32-little -O binary $< $@

$(SREC_DIR)/%.$(SREC_EXT): $(EXE_DIR)/$(EXE_TARGET)
	@ echo "... creating $@"
	$(QUIET) $(OCP) -O srec $< $@

$(MAP_DIR)/%.map: $(EXE_DIR)/$(EXE_TARGET)
	@ echo "... creating $@"
	$(QUIET) $(NM) $(MAP_OPTS)

ifneq "$(PLATFORM)" "" # PLATFORM is defined
ifeq "$(MAKECMDGOALS)" "clean"
# Do not include the list of dependencies generated for each object file
else ifeq "$(MAKECMDGOALS)" "mkclean"
# Do not include the list of dependencies generated for each object file
else
# Include the list of dependencies generated for each object file
-include $(DEPENDENCIES)
endif # MAKECMDGOALS 
endif # PLATFORM


# Compilation of CoMmoN objects from C files
$(OBJDIR_CMN)/%$(OBJ_SUFFIX).o: %.c
	@echo "(MKDEP) $<"
	$(MAKEDEP_C)
	@echo "(CC)    $<"
	$(COMPILE_C)

# Compilation of objects for EXEcutable from C files
$(OBJDIR_EXE)/%$(OBJ_SUFFIX).o: %.c
	@echo "(MKDEP) $<"
	$(MAKEDEP_C)
	@echo "(CC)    $<"
	$(COMPILE_C)

# Compilation of objects for LIBrary from C files
$(OBJDIR_LIB)/%$(OBJ_SUFFIX).o: %.c $(OBJ_DEPS)
	@echo "(MKDEP) $<"
	$(MAKEDEP_C)
	@echo "(CC)    $<"
	$(COMPILE_C)


# Compilation of CoMmoN objects from CPP files
$(OBJDIR_CMN)/%$(OBJ_SUFFIX).o: %.cpp
	@echo "(MKDEP) $<"
	$(MAKEDEP_CPP)
	@echo "(CXX)   $<"
	$(COMPILE_CXX)

# Compilation of objects for EXEcutable from CPP files
$(OBJDIR_EXE)/%$(OBJ_SUFFIX).o: %.cpp
	@echo "(MKDEP) $<"
	$(MAKEDEP_CPP)
	@echo "(CXX)   $<"
	$(COMPILE_CXX)

# Compilation of objects for LIBrary from CPP files
$(OBJDIR_LIB)/%$(OBJ_SUFFIX).o: %.cpp
	@echo "(MKDEP) $<"
	$(MAKEDEP_CPP)
	@echo "(CXX)   $<"
	$(COMPILE_CXX)


# Compilation of CoMmoN objects from ObjCPP files
$(OBJDIR_CMN)/%$(OBJ_SUFFIX).o: %.mm
	@echo "(MKDEP) $<"
	$(MAKEDEP_OCPP)
	@echo "(ObjXX) $<"
	$(COMPILE_OCXX)

# Compilation of objects for EXEcutable from ObjCPP files
$(OBJDIR_EXE)/%$(OBJ_SUFFIX).o: %.mm
	@echo "(MKDEP) $<"
	$(MAKEDEP_OCPP)
	@echo "(ObjXX) $<"
	$(COMPILE_OCXX)

# Compilation of objects for LIBrary from CPP files
$(OBJDIR_LIB)/%$(OBJ_SUFFIX).o: %.mm
	@echo "(MKDEP) $<"
	$(MAKEDEP_OCPP)
	@echo "(ObjXX) $<"
	$(COMPILE_OCXX)


# Assembly of CoMoN objects from assembly files
$(OBJDIR_EXE)/%$(OBJ_SUFFIX).o: %.$(ASM_EXT)
	@echo "(MKDEP) $<"
	$(MAKEDEP_S)
	@echo "(AS)    $<"
	$(COMPILE_S)

# Assembly of objects for EXEcutable from assembly files
$(OBJDIR_EXE)/%$(OBJ_SUFFIX).o: %.$(ASM_EXT)
	@echo "(MKDEP) $<"
	$(MAKEDEP_S)
	@echo "(AS)    $<"
	$(COMPILE_S)

# Assembly of objects for LIBrary from assembly files
$(OBJDIR_LIB)/%$(OBJ_SUFFIX).o: %.$(ASM_EXT)
	@echo "(MKDEP) $<"
	$(MAKEDEP_S)
	@echo "(AS)    $<"
	$(COMPILE_S)

