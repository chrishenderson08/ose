CC = c:/PROGRA~1/MICROS~1.NET/VC7/BIN/cl.exe
CXX = c:/PROGRA~1/MICROS~1.NET/VC7/BIN/cl.exe
LD = c:/PROGRA~1/MICROS~1.NET/VC7/BIN/cl.exe

AR = c:/PROGRA~1/MICROS~1.NET/VC7/BIN/lib.exe

BINEXT = .exe
LIBPRE =
LIBEXT = .lib
OBJEXT = .obj
SHLPRE =
SHLEXT = .dll

COMPILER_PLATFORM = i386-pc-win32

COMPILER_VARIANTS = dbg opt
COMPILER_PIC_VARIANTS = opt

ifneq "$(filter $(VARIANT),$(COMPILER_PIC_VARIANTS))" ""
ifneq "$(filter shared_library,$(PIC_OPTIONS))" ""
LIB_OPTIONS += suppress_archive
endif
endif

COMPILER_CPPFLAGS += -I$(dir $(CC))../INCLUDE
COMPILER_CPPFLAGS += -I$(dir $(CC))../PlatformSDK/Include

COMPILER_CFLAGS = -nologo
COMPILER_CXXFLAGS = -nologo -GX

ifeq "$(filter disable_warnings,$(COMPILER_OPTIONS))" ""
COMPILER_CXXFLAGS += -W3
endif

COMPILER_EXE_CPPFLAGS =
COMPILER_EXE_CFLAGS =
COMPILER_EXE_CXXFLAGS =

COMPILER_LDFLAGS =
COMPILER_LDFLAGS += -L$(dir $(CC))../LIB
COMPILER_LDFLAGS += -L$(dir $(CC))../PlatformSDK/Lib

COMPILER_LDLIBS = -lws2_32

COMPILER_PIC_CPPFLAGS =
COMPILER_PIC_CFLAGS =
COMPILER_PIC_CXXFLAGS =
COMPILER_PIC_LDFLAGS =
COMPILER_PIC_LDLIBS =

COMPILER_LIB_CPPFLAGS_dbg =
COMPILER_LIB_CFLAGS_dbg = -MTd
COMPILER_LIB_CXXFLAGS_dbg = -MTd

COMPILER_LIB_CPPFLAGS_opt =
COMPILER_LIB_CFLAGS_opt = -MT
COMPILER_LIB_CXXFLAGS_opt = -MT

COMPILER_PIC_CPPFLAGS_dbg =
COMPILER_PIC_CFLAGS_dbg = -MDd
COMPILER_PIC_CXXFLAGS_dbg = -MDd

COMPILER_PIC_CPPFLAGS_opt =
COMPILER_PIC_CFLAGS_opt = -MD
COMPILER_PIC_CXXFLAGS_opt = -MD

COMPILER_OBJ_CPPFLAGS_dbg =
COMPILER_OBJ_CFLAGS_dbg = -MTd
COMPILER_OBJ_CXXFLAGS_dbg = -MTd

COMPILER_OBJ_CPPFLAGS_opt =
COMPILER_OBJ_CFLAGS_opt = -MT
COMPILER_OBJ_CXXFLAGS_opt = -MT

COMPILER_EXE_CPPFLAGS_dbg =
COMPILER_EXE_CFLAGS_dbg = -MTd
COMPILER_EXE_CXXFLAGS_dbg = -MTd

COMPILER_EXE_CPPFLAGS_opt =
COMPILER_EXE_CFLAGS_opt = -MT
COMPILER_EXE_CXXFLAGS_opt = -MT

COMPILER_CPPFLAGS_dbg =
COMPILER_CFLAGS_dbg = -Zi -Od
COMPILER_CXXFLAGS_dbg = -Zi -Od
COMPILER_LDFLAGS_dbg =
COMPILER_LDLIBS_dbg =

COMPILER_CPPFLAGS_opt = -DNDEBUG
COMPILER_CFLAGS_opt = -O
COMPILER_CXXFLAGS_opt = -O
COMPILER_LDFLAGS_opt =
COMPILER_LDLIBS_opt =

define COMPILER_LIB_ARCHIVE  
$(AR) -out:@TARGET@ @ALLOBJECTS@
endef

define COMPILER_LIB_RANLIB
endef

define COMPILER_PIC_LIBRARY
LIB="$(COMPILER_LIB.pic)" $(CXX) -nologo -LD -o @TARGET@ @OBJECTS@ \
 $(patsubst -l%,%$(LIBEXT),$(PIC_LDLIBS.pic) $(LDLIBS.pic))
endef

COMPILER_LIB.pic = \
 $(subst /,\,$(subst $(empty) $(empty),;,$(patsubst -L%,%, \
  $(shell echo $(filter -L%,$(PIC_LDFLAGS.pic) $(LDFLAGS.pic)) | \
  sed -e 's%-L//\([a-zA-Z]\)/%-L\1:/%g' \
  -e 's%-L/cygdrive/\([a-zA-Z]\)/%-L\1:/%g'))))

override RUN-COMPILE-LIB.c = \
 $(COMPILE-LIB.c) $< && mv $*$(OBJEXT) $@

override RUN-COMPILE-PIC.c = \
 $(COMPILE-PIC.c) $< && mv $*$(OBJEXT) $@

override RUN-COMPILE-OBJ.c = \
 $(COMPILE-OBJ.c) $< && mv $*$(OBJEXT) $@

override RUN-COMPILE-EXE.c = \
 $(COMPILE-EXE.c) $< && mv $*$(OBJEXT) $@

override RUN-COMPILE-LINK-EXE.c = \
 LIB="$(COMPILER_LIB.c)" $(COMPILE-LINK-EXE.c) $< -o $@ \
  -link $(filter-out -L%,$(LDFLAGS.c)) \
  $(patsubst -l%,%$(LIBEXT),$(LDLIBS.c)) || ($(RM) $@; exit 1)

override RUN-LINK-EXE.c = \
 LIB="$(COMPILER_LIB.c)" $(LINK-EXE.c) $< -o $@ \
  -link $(filter-out -L%,$(LDFLAGS.c)) \
  $(patsubst -l%,%$(LIBEXT),$(LDLIBS.c)) || ($(RM) $@; exit 1)

COMPILER_LIB.cc = \
 $(subst /,\,$(subst $(empty) $(empty),;,$(patsubst -L%,%, \
  $(shell echo $(filter -L%,$(LDFLAGS.c)) | \
  sed -e 's%-L//\([a-zA-Z]\)/%-L\1:/%g' \
  -e 's%-L/cygdrive/\([a-zA-Z]\)/%-L\1:/%g'))))

override RUN-COMPILE-LIB.cxx = \
 $(COMPILE-LIB.cxx) -Tp $< && mv $*$(OBJEXT) $@

override RUN-COMPILE-PIC.cxx = \
 $(COMPILE-PIC.cxx) -Tp $< && mv $*$(OBJEXT) $@

override RUN-COMPILE-OBJ.cxx = \
 $(COMPILE-OBJ.cxx) -Tp $< && mv $*$(OBJEXT) $@

override RUN-COMPILE-EXE.cxx = \
 $(COMPILE-EXE.cxx) -Tp $< && mv $*$(OBJEXT) $@

override RUN-COMPILE-LINK-EXE.cxx = \
 LIB="$(COMPILER_LIB.cxx)" $(COMPILE-LINK.cxx) -Tp $< -o $@ \
  -link $(filter-out -L%,$(LDFLAGS.cxx)) \
  $(patsubst -l%,%$(LIBEXT),$(LDLIBS.cxx)) || ($(RM) $@; exit 1)

override RUN-LINK-EXE.cxx = \
 LIB="$(COMPILER_LIB.cxx)" $(LINK-EXE.cxx) $< -o $@ \
  -link $(filter-out -L%,$(LDFLAGS.cxx)) \
  $(patsubst -l%,%$(LIBEXT),$(LDLIBS.cxx)) || ($(RM) $@; exit 1)

COMPILER_LIB.cxx = \
 $(subst /,\,$(subst $(empty) $(empty),;,$(patsubst -L%,%, \
  $(shell echo $(filter -L%,$(LDFLAGS.cxx)) | \
  sed -e 's%-L//\([a-zA-Z]\)/%-L\1:/%g' \
  -e 's%-L/cygdrive/\([a-zA-Z]\)/%-L\1:/%g'))))
