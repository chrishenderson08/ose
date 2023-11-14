CC = /opt/SUNWspro/bin/cc
CXX = /opt/SUNWspro/bin/CC
LD = /usr/ccs/bin/ld

AR = /usr/ccs/bin/ar

# It is not necessary to use "ranlib" on Solaris. It was
# a hangover from SunOS (4.X) and earlier, left around
# simply so makefiles didn't have to be changed.

RANLIB = : /usr/ccs/bin/ranlib

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
COMPILER_CFLAGS +=
COMPILER_CXXFLAGS += +w
else
#COMPILER_CFLAGS += -w
#COMPILER_CXXFLAGS +=
endif

COMPILER_EXE_CPPFLAGS =
COMPILER_EXE_CFLAGS =
COMPILER_EXE_CXXFLAGS =

# By default Sun C++ will only instantiate actual template
# member functions it thinks are used. This doesn't always
# work, especially if you have more than one program code file
# in a directory and each program code file uses overlapping
# sets of template expansions. The compiler can complain that
# there are undefined symbols even though they should have
# been expanded as a result of compiling one of the other
# programs in the directory. You will need to either have at
# most one program code file in a directory, or you can specify
# that the option "-template=wholeclass" be used by uncommenting
# the following line. It only seems to be necessary to use this
# option when compiling the actual program code files and not
# when compiling object files for a library.

COMPILER_EXE_CXXFLAGS += -template=wholeclass

COMPILER_LDFLAGS =
COMPILER_LDLIBS =

COMPILER_PIC_CPPFLAGS =
COMPILER_PIC_CFLAGS = -KPIC
COMPILER_PIC_CXXFLAGS = -KPIC
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

# Must use the C++ compiler to create the library archive
# so that required C++ template expansions are included
# within the library.

define COMPILER_LIB_ARCHIVE
$(CXX) -xar -o @TARGET@ @ALLOBJECTS@
endef

# Must use the C++ compiler to create the shared library
# so that required C++ template expansions are included
# within the library.

define COMPILER_PIC_LIBRARY
$(CXX) -G -o @TARGET@ @PIC_LDFLAGS@ @LDFLAGS@ @OBJECTS@ \
 @PIC_LDLIBS@ @LDLIBS@
endef

# It has been found necessary to specify the C++ runtime
# libraries "-lCstd" and "-lCrun" when linking a loadable
# module. If they aren't provided and the loadable module
# is used in a program executable linked with the standard
# linker or C compiler, there will be undefined symbols
# for all the C++ runtime components.

define COMPILER_PIC_MODULE
$(CXX) -G -o @TARGET@ @PIC_LDFLAGS@ @LDFLAGS@ @OBJECTS@ \
 @PIC_LDLIBS@ @LDLIBS@ -lCstd -lCrun
endef
