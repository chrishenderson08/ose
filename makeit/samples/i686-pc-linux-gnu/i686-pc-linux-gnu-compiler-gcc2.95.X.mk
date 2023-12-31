CC = /usr/bin/gcc
CXX = /usr/bin/g++
LD = /usr/bin/ld

AR = /usr/bin/ar
RANLIB = /usr/bin/ranlib

# Force the inclusion of any directories containing shared
# libraries into executables which are created, so they
# can be found at run time without setting LD_LIBRARY_PATH.

export LD_RUN_PATH = $(subst $() $(),:,$(PICPATH))

COMPILER_VARIANTS = dbg opt std prf
COMPILER_PIC_VARIANTS = opt std

COMPILER_CPPFLAGS =
COMPILER_CFLAGS =
COMPILER_CXXFLAGS =

ifeq "$(filter disable_warnings,$(COMPILER_OPTIONS))" ""
COMPILER_CFLAGS += -Wall
COMPILER_CXXFLAGS += -Wall
endif

COMPILER_EXE_CPPFLAGS =
COMPILER_EXE_CFLAGS =
COMPILER_EXE_CXXFLAGS =

COMPILER_LDFLAGS =
COMPILER_LDLIBS =

COMPILER_PIC_CPPFLAGS =
COMPILER_PIC_CFLAGS = -fPIC
COMPILER_PIC_CXXFLAGS = -fPIC
COMPILER_PIC_LDFLAGS =
COMPILER_PIC_LDLIBS =

COMPILER_CPPFLAGS_dbg =
COMPILER_CFLAGS_dbg = -g
COMPILER_CXXFLAGS_dbg = -g
COMPILER_LDFLAGS_dbg =
COMPILER_LDLIBS_dbg =

COMPILER_CPPFLAGS_opt = -DNDEBUG
COMPILER_CFLAGS_opt = -O
COMPILER_CXXFLAGS_opt = -O
COMPILER_LDFLAGS_opt =
COMPILER_LDLIBS_opt =

COMPILER_CPPFLAGS_prf =
COMPILER_CFLAGS_prf = -pg
COMPILER_CXXFLAGS_prf = -pg
COMPILER_LDFLAGS_prf =
COMPILER_LDLIBS_prf =

COMPILER_CPPFLAGS_std =
COMPILER_CFLAGS_std =
COMPILER_CXXFLAGS_std =
COMPILER_LDFLAGS_std =
COMPILER_LDLIBS_std =

define COMPILER_PIC_LIBRARY
$(CXX) -shared -o @TARGET@ @PIC_LDFLAGS@ \
 @LDFLAGS@ @OBJECTS@ @PIC_LDLIBS@ @LDLIBS@
endef
