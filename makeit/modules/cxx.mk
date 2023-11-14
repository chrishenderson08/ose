## Copyright 1992-1993 OTC LIMITED
## Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## This module now handles the .cc, .cpp and .cxx extensions. The
## use of the .C and .c++ extensions is no longer supported, nor is
## the use of the .c extension for C++ code. The .c extension can
## now only be used for C code using the "c" module.

%.h :
%.h++ :
%.hpp :
%.hxx :
%.hh :
%.H :
%.i :

vpath %.c $(SRCDIR) $(SRCDIRS)
vpath %.c++ $(SRCDIR) $(SRCDIRS)
vpath %.cpp $(SRCDIR) $(SRCDIRS)
vpath %.cxx $(SRCDIR) $(SRCDIRS)
vpath %.cc $(SRCDIR) $(SRCDIRS)
vpath %.C $(SRCDIR) $(SRCDIRS)

vpath %.h $(SRCDIR) $(SRCDIRS)
vpath %.h++ $(SRCDIR) $(SRCDIRS)
vpath %.hpp $(SRCDIR) $(SRCDIRS)
vpath %.hxx $(SRCDIR) $(SRCDIRS)
vpath %.hh $(SRCDIR) $(SRCDIRS)
vpath %.H $(SRCDIR) $(SRCDIRS)
vpath %.i $(SRCDIR) $(SRCDIRS)

## Calculation of compiler flags to use for each source file.
## This magic has to cope with the feature whereby specific
## files can be compiled with flags from an alternate variant.
## This is required where heavy optimisation can break the
## compilation of specific files and thus they should not be
## compiled as optimised but within another variant. A big
## fiddle also has to be done on the include directory search
## path if we are compiling in a distinct workspace.

QUALIFIED_VARIANT.cxx = \
 $(firstword $(patsubst $(VARIANT)=%,%, \
  $(filter $(VARIANT)=%, $(patsubst $(notdir $<):%,%, \
  $(filter $(notdir $<):%,$(QUALIFIED_VARIANT))))) $(VARIANT))

QUALIFIED_CPPFLAGS.cxx = \
 $(patsubst $(notdir $<):%,%, \
  $(filter -% $(notdir $<):%,$(QUALIFIED_CPPFLAGS)))

ALL_CPPFLAGS.cxx = $(QUALIFIED_CPPFLAGS.cxx) \
 $(CPPFLAGS_$(QUALIFIED_VARIANT.cxx)) $(CPPFLAGS) \
 $(PROJECT_CPPFLAGS_$(QUALIFIED_VARIANT.cxx)) $(PROJECT_CPPFLAGS) \
 $(COMPILER_CPPFLAGS_$(QUALIFIED_VARIANT.cxx)) $(COMPILER_CPPFLAGS) \
 $(SYSTEM_CPPFLAGS_$(QUALIFIED_VARIANT.cxx)) $(SYSTEM_CPPFLAGS)

CPPFLAGS.cxx =

ifneq "$(SRCDIR)" "."

ifeq "$(filter -I.,$(ALL_CPPFLAGS.cxx))" ""
CPPFLAGS.cxx += -I$(SRCDIR)
else
ifneq "$(firstword $(filter -I%,$(ALL_CPPFLAGS.cxx)))" "-I."
CPPFLAGS.cxx += -I. -I$(SRCDIR)
endif
endif

CPPFLAGS.cxx += $(foreach opt,$(ALL_CPPFLAGS.cxx),$(opt) \
  $(patsubst -I%,-I$(SRCDIR)/%,$(filter-out -I/%, \
  $(filter -I%,$(filter-out -I$(MK),$(opt))))))

else

CPPFLAGS.cxx += $(ALL_CPPFLAGS.cxx)

endif

LIB_CPPFLAGS.cxx =  $(LIB_CPPFLAGS) $(PROJECT_LIB_CPPFLAGS) \
 $(COMPILER_LIB_CPPFLAGS) $(COMPILER_LIB_CPPFLAGS_$(VARIANT)) \
 $(SYSTEM_LIB_CPPFLAGS)

PIC_CPPFLAGS.cxx =  $(PIC_CPPFLAGS) $(PROJECT_PIC_CPPFLAGS) \
 $(COMPILER_PIC_CPPFLAGS) $(COMPILER_PIC_CPPFLAGS_$(VARIANT)) \
 $(SYSTEM_PIC_CPPFLAGS)

OBJ_CPPFLAGS.cxx =  $(OBJ_CPPFLAGS) $(PROJECT_OBJ_CPPFLAGS) \
 $(COMPILER_OBJ_CPPFLAGS) $(COMPILER_OBJ_CPPFLAGS_$(VARIANT)) \
 $(SYSTEM_OBJ_CPPFLAGS)

EXE_CPPFLAGS.cxx =  $(EXE_CPPFLAGS) $(PROJECT_EXE_CPPFLAGS) \
 $(COMPILER_EXE_CPPFLAGS) $(COMPILER_EXE_CPPFLAGS_$(VARIANT)) \
 $(SYSTEM_EXE_CPPFLAGS)

QUALIFIED_CXXFLAGS.cxx = \
 $(patsubst $(notdir $<):%,%, \
  $(filter -% $(notdir $<):%,$(QUALIFIED_CXXFLAGS)))

ALL_CXXFLAGS.cxx = $(QUALIFIED_CXXFLAGS.cxx) \
 $(CXXFLAGS_$(QUALIFIED_VARIANT.cxx)) $(CXXFLAGS) \
 $(PROJECT_CXXFLAGS_$(QUALIFIED_VARIANT.cxx)) $(PROJECT_CXXFLAGS) \
 $(COMPILER_CXXFLAGS_$(QUALIFIED_VARIANT.cxx)) $(COMPILER_CXXFLAGS) \
 $(SYSTEM_CXXFLAGS_$(QUALIFIED_VARIANT.cxx)) $(SYSTEM_CXXFLAGS)

CXXFLAGS.cxx = $(ALL_CXXFLAGS.cxx)

LIB_CXXFLAGS.cxx =  $(LIB_CXXFLAGS) $(PROJECT_LIB_CXXFLAGS) \
 $(COMPILER_LIB_CXXFLAGS) $(COMPILER_LIB_CXXFLAGS_$(VARIANT)) \
 $(SYSTEM_LIB_CXXFLAGS)

PIC_CXXFLAGS.cxx =  $(PIC_CXXFLAGS) $(PROJECT_PIC_CXXFLAGS) \
 $(COMPILER_PIC_CXXFLAGS) $(COMPILER_PIC_CXXFLAGS_$(VARIANT)) \
 $(SYSTEM_PIC_CXXFLAGS)

OBJ_CXXFLAGS.cxx =  $(OBJ_CXXFLAGS) $(PROJECT_OBJ_CXXFLAGS) \
 $(COMPILER_OBJ_CXXFLAGS) $(COMPILER_OBJ_CXXFLAGS_$(VARIANT)) \
 $(SYSTEM_OBJ_CXXFLAGS)

EXE_CXXFLAGS.cxx =  $(EXE_CXXFLAGS) $(PROJECT_EXE_CXXFLAGS) \
 $(COMPILER_EXE_CXXFLAGS) $(COMPILER_EXE_CXXFLAGS_$(VARIANT)) \
 $(SYSTEM_EXE_CXXFLAGS)

QUALIFIED_LDFLAGS.cxx = \
 $(patsubst $(notdir $@):%,%, \
  $(filter -% $(notdir $@):%,$(QUALIFIED_LDFLAGS)))

ALL_LDFLAGS.cxx = $(QUALIFIED_LDFLAGS.cxx) \
 $(LDFLAGS_$(VARIANT)) $(LDFLAGS) \
 $(PROJECT_LDFLAGS_$(VARIANT)) $(PROJECT_LDFLAGS) \
 $(COMPILER_LDFLAGS_$(VARIANT)) $(COMPILER_LDFLAGS) \
 $(SYSTEM_LDFLAGS_$(VARIANT)) $(SYSTEM_LDFLAGS)

LDFLAGS.cxx = $(ALL_LDFLAGS.cxx)

QUALIFIED_LDLIBS.cxx = \
 $(patsubst $(notdir $@):%,%, \
  $(filter -% $(notdir $@):%,$(QUALIFIED_LDLIBS)))

ALL_LDLIBS.cxx = $(QUALIFIED_LDLIBS.cxx) \
 $(LDLIBS_$(VARIANT)) $(LDLIBS) \
 $(PROJECT_LDLIBS_$(VARIANT)) $(PROJECT_LDLIBS) \
 $(COMPILER_LDLIBS_$(VARIANT)) $(COMPILER_LDLIBS) \
 $(SYSTEM_LDLIBS_$(VARIANT)) $(SYSTEM_LDLIBS)

LDLIBS.cxx = \
 $(filter-out $< $(MK)/lib$(LIBEXT) $(ALL_LDLIBS.cxx) \
  $(patsubst -l%,\%/lib%$(LIBEXT),$(ALL_LDLIBS.cxx)), \
  $(filter %$(LIBEXT) %$(OBJEXT),$^)) \
 $(filter $(MK)/lib$(LIBEXT),$^) $(ALL_LDLIBS.cxx)

ifneq "$(filter no_minus_o_for_objects,$(COMPILER_ATTRIBUTES))" ""
TARGET.cxx = && mv $(basename $(<F))$(OBJEXT) $@
else
TARGET.cxx = -o $@
endif

## Build up basic commands for compilation only.

PREPROCESS.cxx = $(CXX) -E $(CXXFLAGS.cxx) $(CPPFLAGS.cxx)

COMPILE.cxx = $(CXX) $(CXXFLAGS.cxx) $(CPPFLAGS.cxx) -c

COMPILE-LIB.cxx = $(COMPILE.cxx) $(LIB_CPPFLAGS.cxx) $(LIB_CXXFLAGS.cxx)
COMPILE-PIC.cxx = $(COMPILE.cxx) $(PIC_CPPFLAGS.cxx) $(PIC_CXXFLAGS.cxx)
COMPILE-OBJ.cxx = $(COMPILE.cxx) $(OBJ_CPPFLAGS.cxx) $(OBJ_CXXFLAGS.cxx)
COMPILE-EXE.cxx = $(COMPILE.cxx) $(EXE_CPPFLAGS.cxx) $(EXE_CXXFLAGS.cxx)

COMPILE-LINK-EXE.cxx = $(LINK-EXE.cxx) $(EXE_CPPFLAGS.cxx) $(EXE_CXXFLAGS.cxx)
LINK-EXE.cxx = $(CXX) $(CXXFLAGS.cxx) $(CPPFLAGS.cxx)

## Now construct the full commands for use in rules.
## This includes additional flags needing in linking.

RUN-COMPILE-LIB.cxx = \
 $(COMPILE-LIB.cxx) $< $(TARGET.cxx)

RUN-COMPILE-PIC.cxx = \
 $(COMPILE-PIC.cxx) $< $(TARGET.cxx)

RUN-COMPILE-OBJ.cxx = \
 $(COMPILE-OBJ.cxx) $< $(TARGET.cxx)

RUN-COMPILE-EXE.cxx = \
 $(COMPILE-EXE.cxx) $< $(TARGET.cxx)

RUN-COMPILER-LINK-EXE.cxx = \
 $(COMPILE-LINK-EXE.cxx) $< $(LDFLAGS.cxx) $(LDLIBS.cxx) \
  -o $@ || ($(RM) $@; exit 1)

RUN-LINK-EXE.cxx = \
 $(LINK-EXE.cxx) $< $(LDFLAGS.cxx) $(LDLIBS.cxx) \
  -o $@ || ($(RM) $@; exit 1)

## Wipe out any standard targets for creation of object files
## from source files and programs from source files for the
## source file extensions this module deals with.

%.o : %.cc
%.o : %.cpp
%.o : %.cxx

% : %.cc
% : %.cpp
% : %.cxx

%$(OBJEXT) : %.cc
%$(OBJEXT) : %.cpp
%$(OBJEXT) : %.cxx

%$(BINEXT) : %.cc
%$(BINEXT) : %.cpp
%$(BINEXT) : %.cxx

## Calculate all language source files.

_cc_real_SRC := \
 $(filter-out $(EXCLUDE),$(filter %.cc,$(SRCFILES)))

_cc_generated_MK_SRC := \
 $(addprefix $(MK)/,$(_cc_generated_SRC))

_cc_make_MK_SRC := \
 $(filter-out $(EXCLUDE),$(_cc_generated_MK_SRC))

_cc_make_SRC := \
 $(notdir $(_cc_make_MK_SRC))

_cpp_real_SRC := \
 $(filter-out $(EXCLUDE),$(filter %.cpp,$(SRCFILES)))

_cpp_generated_MK_SRC := \
 $(addprefix $(MK)/,$(_cpp_generated_SRC))

_cpp_make_MK_SRC := \
 $(filter-out $(EXCLUDE),$(_cpp_generated_MK_SRC))

_cpp_make_SRC := \
 $(notdir $(_cpp_make_MK_SRC))

_cxx_real_SRC := \
 $(filter-out $(EXCLUDE),$(filter %.cxx,$(SRCFILES)))

_cxx_generated_MK_SRC := \
 $(addprefix $(MK)/,$(_cxx_generated_SRC))

_cxx_make_MK_SRC := \
 $(filter-out $(EXCLUDE),$(_cxx_generated_MK_SRC))

_cxx_make_SRC := \
 $(notdir $(_cxx_make_MK_SRC))

## Calculate program source.

_cc_real_PROGRAM_SRC := \
 $(filter-out $(EXCLUDE),$(filter $(addsuffix .cc,$(PROGRAMS)),$(SRCFILES)))

_cc_generated_MK_PROGRAM_SRC := \
 $(addprefix $(MK)/,$(_cc_generated_PROGRAM_SRC))

_cc_make_MK_PROGRAM_SRC := \
 $(filter-out $(EXCLUDE),$(_cc_generated_MK_PROGRAM_SRC))

_cc_make_PROGRAM_SRC := \
 $(notdir $(_cc_make_MK_PROGRAM_SRC))

_cpp_real_PROGRAM_SRC := \
 $(filter-out $(EXCLUDE),$(filter $(addsuffix .cpp,$(PROGRAMS)),$(SRCFILES)))

_cpp_generated_MK_PROGRAM_SRC := \
 $(addprefix $(MK)/,$(_cpp_generated_PROGRAM_SRC))

_cpp_make_MK_PROGRAM_SRC := \
 $(filter-out $(EXCLUDE),$(_cpp_generated_MK_PROGRAM_SRC))

_cpp_make_PROGRAM_SRC := \
 $(notdir $(_cpp_make_MK_PROGRAM_SRC))

_cxx_real_PROGRAM_SRC := \
 $(filter-out $(EXCLUDE),$(filter $(addsuffix .cxx,$(PROGRAMS)),$(SRCFILES)))

_cxx_generated_MK_PROGRAM_SRC := \
 $(addprefix $(MK)/,$(_cxx_generated_PROGRAM_SRC))

_cxx_make_MK_PROGRAM_SRC := \
 $(filter-out $(EXCLUDE),$(_cxx_generated_MK_PROGRAM_SRC))

_cxx_make_PROGRAM_SRC := \
 $(notdir $(_cxx_make_MK_PROGRAM_SRC))

## Calculate non library source.

_cc_real_NONLIB_SRC := \
 $(filter-out $(EXCLUDE),$(filter %.cc,$(NONLIBSRC)))

_cc_generated_MK_NONLIB_SRC := \
 $(addprefix $(MK)/,$(_cc_generated_NONLIB_SRC))

_cc_make_MK_NONLIB_SRC := \
 $(filter-out $(EXCLUDE),$(_cc_generated_MK_NONLIB_SRC))

_cc_make_NONLIB_SRC := \
 $(notdir $(_cc_make_NONLIB_SRC))

_cpp_real_NONLIB_SRC := \
 $(filter-out $(EXCLUDE),$(filter %.cpp,$(NONLIBSRC)))

_cpp_generated_MK_NONLIB_SRC := \
 $(addprefix $(MK)/,$(_cpp_generated_NONLIB_SRC))

_cpp_make_MK_NONLIB_SRC := \
 $(filter-out $(EXCLUDE),$(_cpp_generated_MK_NONLIB_SRC))

_cpp_make_NONLIB_SRC := \
 $(notdir $(_cpp_make_NONLIB_SRC))

_cxx_real_NONLIB_SRC := \
 $(filter-out $(EXCLUDE),$(filter %.cxx,$(NONLIBSRC)))

_cxx_generated_MK_NONLIB_SRC := \
 $(addprefix $(MK)/,$(_cxx_generated_NONLIB_SRC))

_cxx_make_MK_NONLIB_SRC := \
 $(filter-out $(EXCLUDE),$(_cxx_generated_MK_NONLIB_SRC))

_cxx_make_NONLIB_SRC := \
 $(notdir $(_cxx_make_NONLIB_SRC))

## Calculate library source.

_cc_real_LIB_SRC := \
 $(filter-out $(_cc_real_NONLIB_SRC) \
  $(_cc_real_PROGRAM_SRC),$(_cc_real_SRC))

_cc_make_MK_LIB_SRC := \
 $(filter-out $(_cc_make_MK_NONLIB_SRC) \
  $(_cc_make_MK_PROGRAM_SRC), $(_cc_make_MK_SRC))

_cc_make_LIB_SRC := \
 $(notdir $(_cc_make_MK_LIB_SRC))

_cpp_real_LIB_SRC := \
 $(filter-out $(_cpp_real_NONLIB_SRC) \
  $(_cpp_real_PROGRAM_SRC),$(_cpp_real_SRC))

_cpp_make_MK_LIB_SRC := \
 $(filter-out $(_cpp_make_MK_NONLIB_SRC) \
  $(_cpp_make_MK_PROGRAM_SRC), $(_cpp_make_MK_SRC))

_cpp_make_LIB_SRC := \
 $(notdir $(_cpp_make_MK_LIB_SRC))

_cxx_real_LIB_SRC := \
 $(filter-out $(_cxx_real_NONLIB_SRC) \
  $(_cxx_real_PROGRAM_SRC),$(_cxx_real_SRC))

_cxx_make_MK_LIB_SRC := \
 $(filter-out $(_cxx_make_MK_NONLIB_SRC) \
  $(_cxx_make_MK_PROGRAM_SRC), $(_cxx_make_MK_SRC))

_cxx_make_LIB_SRC := \
 $(notdir $(_cxx_make_MK_LIB_SRC))

## Setup library dependencies for real source.

ifneq "$(_cc_real_LIB_SRC)" ""

_cc_real_LIB_OBJECTS := \
 $(patsubst %.cc,%$(OBJEXT),$(_cc_real_LIB_SRC))

_cc_real_MK_LIB_OBJECTS := \
 $(addprefix $(MK)/,$(_cc_real_LIB_OBJECTS))

$(_cc_real_MK_LIB_OBJECTS) : $(MK)/%$(OBJEXT) : %.cc
	$(RUN-COMPILE-LIB.cxx)

_cc_real_MK_PIC_OBJECTS := \
 $(addprefix $(MK).pic/,$(_cc_real_LIB_OBJECTS))

$(_cc_real_MK_PIC_OBJECTS) : $(MK).pic/%$(OBJEXT) : %.cc
	$(RUN-COMPILE-PIC.cxx)

_makeit_MK_LIB_OBJECTS += $(_cc_real_MK_LIB_OBJECTS)

endif

ifneq "$(_cpp_real_LIB_SRC)" ""

_cpp_real_LIB_OBJECTS := \
 $(patsubst %.cpp,%$(OBJEXT),$(_cpp_real_LIB_SRC))

_cpp_real_MK_LIB_OBJECTS := \
 $(addprefix $(MK)/,$(_cpp_real_LIB_OBJECTS))

$(_cpp_real_MK_LIB_OBJECTS) : $(MK)/%$(OBJEXT) : %.cpp
	$(RUN-COMPILE-LIB.cxx)

_cpp_real_MK_PIC_OBJECTS := \
 $(addprefix $(MK).pic/,$(_cpp_real_LIB_OBJECTS))

$(_cpp_real_MK_PIC_OBJECTS) : $(MK).pic/%$(OBJEXT) : %.cpp
	$(RUN-COMPILE-PIC.cxx)

_makeit_MK_LIB_OBJECTS += $(_cpp_real_MK_LIB_OBJECTS)

endif

ifneq "$(_cxx_real_LIB_SRC)" ""

_cxx_real_LIB_OBJECTS := \
 $(patsubst %.cxx,%$(OBJEXT),$(_cxx_real_LIB_SRC))

_cxx_real_MK_LIB_OBJECTS := \
 $(addprefix $(MK)/,$(_cxx_real_LIB_OBJECTS))

$(_cxx_real_MK_LIB_OBJECTS) : $(MK)/%$(OBJEXT) : %.cxx
	$(RUN-COMPILE-LIB.cxx)

_cxx_real_MK_PIC_OBJECTS := \
 $(addprefix $(MK).pic/,$(_cxx_real_LIB_OBJECTS))

$(_cxx_real_MK_PIC_OBJECTS) : $(MK).pic/%$(OBJEXT) : %.cxx
	$(RUN-COMPILE-PIC.cxx)

_makeit_MK_LIB_OBJECTS += $(_cxx_real_MK_LIB_OBJECTS)

endif

## Setup library dependencies for generated source.

ifneq "$(_cc_make_MK_LIB_SRC)" ""

_cc_make_LIB_OBJECTS := \
 $(patsubst %.cc,%$(OBJEXT),$(_cc_make_LIB_SRC))

_cc_make_MK_LIB_OBJECTS := \
 $(addprefix $(MK)/,$(_cc_make_LIB_OBJECTS))

$(_cc_make_MK_LIB_OBJECTS) : $(MK)/%$(OBJEXT) : $(MK)/%.cc
	$(RUN-COMPILE-LIB.cxx)

_cc_make_MK_PIC_OBJECTS := \
 $(addprefix $(MK).pic/,$(_cc_make_LIB_OBJECTS))

$(_cc_make_MK_PIC_OBJECTS) : $(MK).pic/%$(OBJEXT) : $(MK)/%.cc
	$(RUN-COMPILE-PIC.cxx)

_makeit_MK_LIB_OBJECTS += $(_cc_make_MK_LIB_OBJECTS)

endif

ifneq "$(_cpp_make_MK_LIB_SRC)" ""

_cpp_make_LIB_OBJECTS := \
 $(patsubst %.cpp,%$(OBJEXT),$(_cpp_make_LIB_SRC))

_cpp_make_MK_LIB_OBJECTS := \
 $(addprefix $(MK)/,$(_cpp_make_LIB_OBJECTS))

$(_cpp_make_MK_LIB_OBJECTS) : $(MK)/%$(OBJEXT) : $(MK)/%.cpp
	$(RUN-COMPILE-LIB.cxx)

_cpp_make_MK_PIC_OBJECTS := \
 $(addprefix $(MK).pic/,$(_cpp_make_LIB_OBJECTS))

$(_cpp_make_MK_PIC_OBJECTS) : $(MK).pic/%$(OBJEXT) : $(MK)/%.cpp
	$(RUN-COMPILE-PIC.cxx)

_makeit_MK_LIB_OBJECTS += $(_cpp_make_MK_LIB_OBJECTS)

endif

ifneq "$(_cxx_make_MK_LIB_SRC)" ""

_cxx_make_LIB_OBJECTS := \
 $(patsubst %.cxx,%$(OBJEXT),$(_cxx_make_LIB_SRC))

_cxx_make_MK_LIB_OBJECTS := \
 $(addprefix $(MK)/,$(_cxx_make_LIB_OBJECTS))

$(_cxx_make_MK_LIB_OBJECTS) : $(MK)/%$(OBJEXT) : $(MK)/%.cxx
	$(RUN-COMPILE-LIB.cxx)

_cxx_make_MK_PIC_OBJECTS := \
 $(addprefix $(MK).pic/,$(_cxx_make_LIB_OBJECTS))

$(_cxx_make_MK_PIC_OBJECTS) : $(MK).pic/%$(OBJEXT) : $(MK)/%.cxx
	$(RUN-COMPILE-PIC.cxx)

_makeit_MK_LIB_OBJECTS += $(_cxx_make_MK_LIB_OBJECTS)

endif

## Setup dependencies for non library objects coming from real source.

ifneq "$(_cc_real_NONLIB_SRC)" ""

_cc_real_NONLIB_OBJECTS := \
 $(patsubst %.cc,%$(OBJEXT),$(_cc_real_NONLIB_SRC))

_cc_real_MK_NONLIB_OBJECTS := \
 $(addprefix $(MK)/,$(_cc_real_NONLIB_OBJECTS))

$(_cc_real_MK_NONLIB_OBJECTS) : $(MK)/%$(OBJEXT) : %.cc
	$(RUN-COMPILE-OBJ.cxx)

$(_cc_real_NONLIB_OBJECTS) : %$(OBJEXT) : makeit.setup

$(_cc_real_NONLIB_OBJECTS) : %$(OBJEXT) : $(MK)/%$(OBJEXT)

endif

ifneq "$(_cpp_real_NONLIB_SRC)" ""

_cpp_real_NONLIB_OBJECTS := \
 $(patsubst %.cpp,%$(OBJEXT),$(_cpp_real_NONLIB_SRC))

_cpp_real_MK_NONLIB_OBJECTS := \
 $(addprefix $(MK)/,$(_cpp_real_NONLIB_OBJECTS))

$(_cpp_real_MK_NONLIB_OBJECTS) : $(MK)/%$(OBJEXT) : %.cpp
	$(RUN-COMPILE-OBJ.cxx)

$(_cpp_real_NONLIB_OBJECTS) : %$(OBJEXT) : makeit.setup

$(_cpp_real_NONLIB_OBJECTS) : %$(OBJEXT) : $(MK)/%$(OBJEXT)

endif

ifneq "$(_cxx_real_NONLIB_SRC)" ""

_cxx_real_NONLIB_OBJECTS := \
 $(patsubst %.cxx,%$(OBJEXT),$(_cxx_real_NONLIB_SRC))

_cxx_real_MK_NONLIB_OBJECTS := \
 $(addprefix $(MK)/,$(_cxx_real_NONLIB_OBJECTS))

$(_cxx_real_MK_NONLIB_OBJECTS) : $(MK)/%$(OBJEXT) : %.cxx
	$(RUN-COMPILE-OBJ.cxx)

$(_cxx_real_NONLIB_OBJECTS) : %$(OBJEXT) : makeit.setup

$(_cxx_real_NONLIB_OBJECTS) : %$(OBJEXT) : $(MK)/%$(OBJEXT)

endif

## Setup dependencies for non library objects coming from generated source.

ifneq "$(_cc_make_MK_NONLIB_SRC)" ""

_cc_make_MK_NONLIB_OBJECTS := \
 $(patsubst %.cc,%$(OBJEXT),$(_cc_make_MK_NONLIB_SRC))

_cc_make_NONLIB_OBJECTS := \
 $(notdir $(_cc_make_MK_NONLIB_OBJECTS))

$(_cc_make_MK_NONLIB_OBJECTS) : $(MK)/%$(OBJEXT) : $(MK)/%.cc
	$(RUN-COMPILE-OBJ.cxx)

$(_cc_make_NONLIB_OBJECTS) : %$(OBJEXT) : makeit.setup

$(_cc_make_NONLIB_OBJECTS) : %$(OBJEXT) : $(MK)/%$(OBJEXT)

endif

ifneq "$(_cpp_make_MK_NONLIB_SRC)" ""

_cpp_make_MK_NONLIB_OBJECTS := \
 $(patsubst %.cpp,%$(OBJEXT),$(_cpp_make_MK_NONLIB_SRC))

_cpp_make_NONLIB_OBJECTS := \
 $(notdir $(_cpp_make_MK_NONLIB_OBJECTS))

$(_cpp_make_MK_NONLIB_OBJECTS) : $(MK)/%$(OBJEXT) : $(MK)/%.cpp
	$(RUN-COMPILE-OBJ.cxx)

$(_cpp_make_NONLIB_OBJECTS) : %$(OBJEXT) : makeit.setup

$(_cpp_make_NONLIB_OBJECTS) : %$(OBJEXT) : $(MK)/%$(OBJEXT)

endif

ifneq "$(_cxx_make_MK_NONLIB_SRC)" ""

_cxx_make_MK_NONLIB_OBJECTS := \
 $(patsubst %.cxx,%$(OBJEXT),$(_cxx_make_MK_NONLIB_SRC))

_cxx_make_NONLIB_OBJECTS := \
 $(notdir $(_cxx_make_MK_NONLIB_OBJECTS))

$(_cxx_make_MK_NONLIB_OBJECTS) : $(MK)/%$(OBJEXT) : $(MK)/%.cxx
	$(RUN-COMPILE-OBJ.cxx)

$(_cxx_make_NONLIB_OBJECTS) : %$(OBJEXT) : makeit.setup

$(_cxx_make_NONLIB_OBJECTS) : %$(OBJEXT) : $(MK)/%$(OBJEXT)

endif

## Setup dependencies for programs coming from real source.

ifneq "$(_cc_real_PROGRAM_SRC)" ""

_cc_real_PROGRAM_OBJECTS := \
 $(patsubst %.cc,%$(OBJEXT),$(_cc_real_PROGRAM_SRC))

_cc_real_MK_PROGRAM_OBJECTS := \
 $(addprefix $(MK)/,$(_cc_real_PROGRAM_OBJECTS))

_cc_real_MK_PROGRAMS := \
 $(addsuffix $(BINEXT),$(addprefix $(MK)/,$(basename $(_cc_real_PROGRAM_SRC))))

$(_cc_real_PROGRAM_OBJECTS) : %$(OBJEXT) : makeit.setup

$(_cc_real_PROGRAM_OBJECTS) : %$(OBJEXT) : $(MK)/%$(OBJEXT)

ifneq "$(filter no_separate_link_phase,$(COMPILER_ATTRIBUTES))" ""

$(_cc_real_MK_PROGRAMS) : $(MK)/%$(BINEXT) : %.cc
	@$(RM) $@
	$(RUN-COMPILER-LINK-EXE.cxx)
	@$(RM) $(<F:.cc=$(OBJEXT))

else

$(_cc_real_MK_PROGRAMS) : $(MK)/%$(BINEXT) : $(MK)/%$(OBJEXT)
	@$(RM) $@
	$(RUN-LINK-EXE.cxx)

endif

$(_cc_real_MK_PROGRAM_OBJECTS) : $(MK)/%$(OBJEXT) : %.cc
	$(RUN-COMPILE-EXE.cxx)

_makeit_MK_BINARIES += $(_cc_real_MK_PROGRAMS)

endif

ifneq "$(_cpp_real_PROGRAM_SRC)" ""

_cpp_real_PROGRAM_OBJECTS := \
 $(patsubst %.cpp,%$(OBJEXT),$(_cpp_real_PROGRAM_SRC))

_cpp_real_MK_PROGRAM_OBJECTS := \
 $(addprefix $(MK)/,$(_cpp_real_PROGRAM_OBJECTS))

_cpp_real_MK_PROGRAMS := \
 $(addsuffix $(BINEXT),$(addprefix $(MK)/,$(basename $(_cpp_real_PROGRAM_SRC))))

$(_cpp_real_PROGRAM_OBJECTS) : %$(OBJEXT) : makeit.setup

$(_cpp_real_PROGRAM_OBJECTS) : %$(OBJEXT) : $(MK)/%$(OBJEXT)

ifneq "$(filter no_separate_link_phase,$(COMPILER_ATTRIBUTES))" ""

$(_cpp_real_MK_PROGRAMS) : $(MK)/%$(BINEXT) : %.cpp
	@$(RM) $@
	$(RUN-COMPILER-LINK-EXE.cxx)
	@$(RM) $(<F:.cpp=$(OBJEXT))

else

$(_cpp_real_MK_PROGRAMS) : $(MK)/%$(BINEXT) : $(MK)/%$(OBJEXT)
	@$(RM) $@
	$(RUN-LINK-EXE.cxx)

endif

$(_cpp_real_MK_PROGRAM_OBJECTS) : $(MK)/%$(OBJEXT) : %.cpp
	$(RUN-COMPILE-EXE.cxx)

_makeit_MK_BINARIES += $(_cpp_real_MK_PROGRAMS)

endif

ifneq "$(_cxx_real_PROGRAM_SRC)" ""

_cxx_real_PROGRAM_OBJECTS := \
 $(patsubst %.cxx,%$(OBJEXT),$(_cxx_real_PROGRAM_SRC))

_cxx_real_MK_PROGRAM_OBJECTS := \
 $(addprefix $(MK)/,$(_cxx_real_PROGRAM_OBJECTS))

_cxx_real_MK_PROGRAMS := \
 $(addsuffix $(BINEXT),$(addprefix $(MK)/,$(basename $(_cxx_real_PROGRAM_SRC))))

$(_cxx_real_PROGRAM_OBJECTS) : %$(OBJEXT) : makeit.setup

$(_cxx_real_PROGRAM_OBJECTS) : %$(OBJEXT) : $(MK)/%$(OBJEXT)

ifneq "$(filter no_separate_link_phase,$(COMPILER_ATTRIBUTES))" ""

$(_cxx_real_MK_PROGRAMS) : $(MK)/%$(BINEXT) : %.cxx
	@$(RM) $@
	$(RUN-COMPILER-LINK-EXE.cxx)
	@$(RM) $(<F:.cxx=$(OBJEXT))

else

$(_cxx_real_MK_PROGRAMS) : $(MK)/%$(BINEXT) : $(MK)/%$(OBJEXT)
	@$(RM) $@
	$(RUN-LINK-EXE.cxx)

endif

$(_cxx_real_MK_PROGRAM_OBJECTS) : $(MK)/%$(OBJEXT) : %.cxx
	$(RUN-COMPILE-EXE.cxx)

_makeit_MK_BINARIES += $(_cxx_real_MK_PROGRAMS)

endif

## Setup dependencies for programs coming from generated source.

ifneq "$(_cc_make_MK_PROGRAM_SRC)" ""

_cc_make_MK_PROGRAM_OBJECTS := \
 $(patsubst %.cc,%$(OBJEXT),$(_cc_make_MK_PROGRAM_SRC))

_cc_make_PROGRAM_OBJECTS := \
 $(notdir $(_cc_make_MK_PROGRAM_OBJECTS))

_cc_make_MK_PROGRAMS := \
 $(addsuffix $(BINEXT),$(basename $(_cc_make_MK_PROGRAM_SRC)))

$(_cc_make_PROGRAM_OBJECTS) : %$(OBJEXT) : makeit.setup

$(_cc_make_PROGRAM_OBJECTS) : %$(OBJEXT) : $(MK)/%$(OBJEXT)

ifneq "$(filter no_separate_link_phase,$(COMPILER_ATTRIBUTES))" ""

$(_cc_make_MK_PROGRAMS) : $(MK)/%$(BINEXT) : $(MK)/%.cc
	@$(RM) $@
	$(RUN-COMPILER-LINK-EXE.cxx)
	@$(RM) $(<F:.cc=$(OBJEXT))

else

$(_cc_make_MK_PROGRAMS) : $(MK)/%$(BINEXT) : $(MK)/%$(OBJEXT)
	@$(RM) $@
	$(RUN-LINK-EXE.cxx)

endif

$(_cc_make_MK_PROGRAM_OBJECTS) : $(MK)/%$(OBJEXT) : $(MK)/%.cc
	$(RUN-COMPILE-EXE.cxx)

_makeit_MK_BINARIES += $(_cc_make_MK_PROGRAMS)

endif

ifneq "$(_cpp_make_MK_PROGRAM_SRC)" ""

_cpp_make_MK_PROGRAM_OBJECTS := \
 $(patsubst %.cpp,%$(OBJEXT),$(_cpp_make_MK_PROGRAM_SRC))

_cpp_make_PROGRAM_OBJECTS := \
 $(notdir $(_cpp_make_MK_PROGRAM_OBJECTS))

_cpp_make_MK_PROGRAMS := \
 $(addsuffix $(BINEXT),$(basename $(_cpp_make_MK_PROGRAM_SRC)))

$(_cpp_make_PROGRAM_OBJECTS) : %$(OBJEXT) : makeit.setup

$(_cpp_make_PROGRAM_OBJECTS) : %$(OBJEXT) : $(MK)/%$(OBJEXT)

ifneq "$(filter no_separate_link_phase,$(COMPILER_ATTRIBUTES))" ""

$(_cpp_make_MK_PROGRAMS) : $(MK)/%$(BINEXT) : $(MK)/%.cpp
	@$(RM) $@
	$(RUN-COMPILER-LINK-EXE.cxx)
	@$(RM) $(<F:.cpp=$(OBJEXT))

else

$(_cpp_make_MK_PROGRAMS) : $(MK)/%$(BINEXT) : $(MK)/%$(OBJEXT)
	@$(RM) $@
	$(RUN-LINK-EXE.cxx)

endif

$(_cpp_make_MK_PROGRAM_OBJECTS) : $(MK)/%$(OBJEXT) : $(MK)/%.cpp
	$(RUN-COMPILE-EXE.cxx)

_makeit_MK_BINARIES += $(_cpp_make_MK_PROGRAMS)

endif

ifneq "$(_cxx_make_MK_PROGRAM_SRC)" ""

_cxx_make_MK_PROGRAM_OBJECTS := \
 $(patsubst %.cxx,%$(OBJEXT),$(_cxx_make_MK_PROGRAM_SRC))

_cxx_make_PROGRAM_OBJECTS := \
 $(notdir $(_cxx_make_MK_PROGRAM_OBJECTS))

_cxx_make_MK_PROGRAMS := \
 $(addsuffix $(BINEXT),$(basename $(_cxx_make_MK_PROGRAM_SRC)))

$(_cxx_make_PROGRAM_OBJECTS) : %$(OBJEXT) : makeit.setup

$(_cxx_make_PROGRAM_OBJECTS) : %$(OBJEXT) : $(MK)/%$(OBJEXT)

ifneq "$(filter no_separate_link_phase,$(COMPILER_ATTRIBUTES))" ""

$(_cxx_make_MK_PROGRAMS) : $(MK)/%$(BINEXT) : $(MK)/%.cxx
	@$(RM) $@
	$(RUN-COMPILER-LINK-EXE.cxx)
	@$(RM) $(<F:.cxx=$(OBJEXT))

else

$(_cxx_make_MK_PROGRAMS) : $(MK)/%$(BINEXT) : $(MK)/%$(OBJEXT)
	@$(RM) $@
	$(RUN-LINK-EXE.cxx)

endif

$(_cxx_make_MK_PROGRAM_OBJECTS) : $(MK)/%$(OBJEXT) : $(MK)/%.cxx
	$(RUN-COMPILE-EXE.cxx)

_makeit_MK_BINARIES += $(_cxx_make_MK_PROGRAMS)

endif

## Dependency files.

DEPFILTER.cxx = sed -f $(MAKEIT_HOME)/targets/depend.sed | sort -u

ifneq "$(_cc_real_LIB_SRC)" ""

_cc_real_LIB_D := $(patsubst %.cc,$(MK)/%.d,$(_cc_real_LIB_SRC))

$(_cc_real_LIB_D) : $(MK)/%.d : %.cc
	@echo makeit: generating dependencies for $<
	@$(PREPROCESS.cxx) -DMAKEIT_DEPEND $< | $(DEPFILTER.cxx) | \
	sed -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%TARGET $$(MK).pic/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%$$(MK)/$(*F).d%' > $@

_depend_SRC += $(_cc_real_LIB_D)

endif

ifneq "$(_cpp_real_LIB_SRC)" ""

_cpp_real_LIB_D := $(patsubst %.cpp,$(MK)/%.d,$(_cpp_real_LIB_SRC))

$(_cpp_real_LIB_D) : $(MK)/%.d : %.cpp
	@echo makeit: generating dependencies for $<
	@$(PREPROCESS.cxx) -DMAKEIT_DEPEND $< | $(DEPFILTER.cxx) | \
	sed -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%TARGET $$(MK).pic/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%$$(MK)/$(*F).d%' > $@

_depend_SRC += $(_cpp_real_LIB_D)

endif

ifneq "$(_cxx_real_LIB_SRC)" ""

_cxx_real_LIB_D := $(patsubst %.cxx,$(MK)/%.d,$(_cxx_real_LIB_SRC))

$(_cxx_real_LIB_D) : $(MK)/%.d : %.cxx
	@echo makeit: generating dependencies for $<
	@$(PREPROCESS.cxx) -DMAKEIT_DEPEND $< | $(DEPFILTER.cxx) | \
	sed -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%TARGET $$(MK).pic/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%$$(MK)/$(*F).d%' > $@

_depend_SRC += $(_cxx_real_LIB_D)

endif

ifneq "$(_cc_make_MK_LIB_SRC)" ""

_cc_make_LIB_D := $(patsubst %.cc,%.d,$(_cc_make_MK_LIB_SRC))

$(_cc_make_LIB_D) : $(MK)/%.d : $(MK)/%.cc
	@echo makeit: generating dependencies for $<
	@$(PREPROCESS.cxx) -DMAKEIT_DEPEND $< | $(DEPFILTER.cxx) | \
	sed -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%TARGET $$(MK).pic/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%$$(MK)/$(*F).d%' > $@

_depend_SRC += $(_cc_make_LIB_D)

endif

ifneq "$(_cpp_make_MK_LIB_SRC)" ""

_cpp_make_LIB_D := $(patsubst %.cpp,%.d,$(_cpp_make_MK_LIB_SRC))

$(_cpp_make_LIB_D) : $(MK)/%.d : $(MK)/%.cpp
	@echo makeit: generating dependencies for $<
	@$(PREPROCESS.cxx) -DMAKEIT_DEPEND $< | $(DEPFILTER.cxx) | \
	sed -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%TARGET $$(MK).pic/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%$$(MK)/$(*F).d%' > $@

_depend_SRC += $(_cpp_make_LIB_D)

endif

ifneq "$(_cxx_make_MK_LIB_SRC)" ""

_cxx_make_LIB_D := $(patsubst %.cxx,%.d,$(_cxx_make_MK_LIB_SRC))

$(_cxx_make_LIB_D) : $(MK)/%.d : $(MK)/%.cxx
	@echo makeit: generating dependencies for $<
	@$(PREPROCESS.cxx) -DMAKEIT_DEPEND $< | $(DEPFILTER.cxx) | \
	sed -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%TARGET $$(MK).pic/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%$$(MK)/$(*F).d%' > $@

_depend_SRC += $(_cxx_make_LIB_D)

endif

ifneq "$(_cc_real_NONLIB_SRC)" ""

_cc_real_NONLIB_D := $(patsubst %.cc,$(MK)/%.d,$(_cc_real_NONLIB_SRC))

$(_cc_real_NONLIB_D) : $(MK)/%.d : %.cc
	@echo makeit: generating dependencies for $<
	@$(PREPROCESS.cxx) -DMAKEIT_DEPEND $< | $(DEPFILTER.cxx) | \
	sed -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%$$(MK)/$(*F).d%' > $@

_depend_SRC += $(_cc_real_NONLIB_D)

endif

ifneq "$(_cpp_real_NONLIB_SRC)" ""

_cpp_real_NONLIB_D := $(patsubst %.cpp,$(MK)/%.d,$(_cpp_real_NONLIB_SRC))

$(_cpp_real_NONLIB_D) : $(MK)/%.d : %.cpp
	@echo makeit: generating dependencies for $<
	@$(PREPROCESS.cxx) -DMAKEIT_DEPEND $< | $(DEPFILTER.cxx) | \
	sed -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%$$(MK)/$(*F).d%' > $@

_depend_SRC += $(_cpp_real_NONLIB_D)

endif

ifneq "$(_cxx_real_NONLIB_SRC)" ""

_cxx_real_NONLIB_D := $(patsubst %.cxx,$(MK)/%.d,$(_cxx_real_NONLIB_SRC))

$(_cxx_real_NONLIB_D) : $(MK)/%.d : %.cxx
	@echo makeit: generating dependencies for $<
	@$(PREPROCESS.cxx) -DMAKEIT_DEPEND $< | $(DEPFILTER.cxx) | \
	sed -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%$$(MK)/$(*F).d%' > $@

_depend_SRC += $(_cxx_real_NONLIB_D)

endif

ifneq "$(_cc_make_MK_NONLIB_SRC)" ""

_cc_make_NONLIB_D := $(patsubst %.cc,%.d,$(_cc_make_MK_NONLIB_SRC))

$(_cc_make_NONLIB_D) : $(MK)/%.d : $(MK)/%.cc
	@echo makeit: generating dependencies for $<
	@$(PREPROCESS.cxx) -DMAKEIT_DEPEND $< | $(DEPFILTER.cxx) | \
	sed -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%$$(MK)/$(*F).d%' > $@

_depend_SRC += $(_cc_make_NONLIB_D)

endif

ifneq "$(_cpp_make_MK_NONLIB_SRC)" ""

_cpp_make_NONLIB_D := $(patsubst %.cpp,%.d,$(_cpp_make_MK_NONLIB_SRC))

$(_cpp_make_NONLIB_D) : $(MK)/%.d : $(MK)/%.cpp
	@echo makeit: generating dependencies for $<
	@$(PREPROCESS.cxx) -DMAKEIT_DEPEND $< | $(DEPFILTER.cxx) | \
	sed -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%$$(MK)/$(*F).d%' > $@

_depend_SRC += $(_cpp_make_NONLIB_D)

endif

ifneq "$(_cxx_make_MK_NONLIB_SRC)" ""

_cxx_make_NONLIB_D := $(patsubst %.cxx,%.d,$(_cxx_make_MK_NONLIB_SRC))

$(_cxx_make_NONLIB_D) : $(MK)/%.d : $(MK)/%.cxx
	@echo makeit: generating dependencies for $<
	@$(PREPROCESS.cxx) -DMAKEIT_DEPEND $< | $(DEPFILTER.cxx) | \
	sed -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%$$(MK)/$(*F).d%' > $@

_depend_SRC += $(_cxx_make_NONLIB_D)

endif

ifneq "$(_cc_real_PROGRAM_SRC)" ""

_cc_real_PROGRAM_D := $(patsubst %.cc,$(MK)/%.d,$(_cc_real_PROGRAM_SRC))

$(_cc_real_PROGRAM_D) : $(MK)/%.d : %.cc
	@echo makeit: generating dependencies for $<
	@$(PREPROCESS.cxx) -DMAKEIT_DEPEND $< | \
	 $(DEPFILTER.cxx) | sed -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(BINEXT)%' \
	    -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%$$(MK)/$(*F).d%' > $@

_depend_SRC += $(_cc_real_PROGRAM_D)

endif

ifneq "$(_cpp_real_PROGRAM_SRC)" ""

_cpp_real_PROGRAM_D := $(patsubst %.cpp,$(MK)/%.d,$(_cpp_real_PROGRAM_SRC))

$(_cpp_real_PROGRAM_D) : $(MK)/%.d : %.cpp
	@echo makeit: generating dependencies for $<
	@$(PREPROCESS.cxx) -DMAKEIT_DEPEND $< | \
	 $(DEPFILTER.cxx) | sed -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(BINEXT)%' \
	    -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%$$(MK)/$(*F).d%' > $@

_depend_SRC += $(_cpp_real_PROGRAM_D)

endif

ifneq "$(_cxx_real_PROGRAM_SRC)" ""

_cxx_real_PROGRAM_D := $(patsubst %.cxx,$(MK)/%.d,$(_cxx_real_PROGRAM_SRC))

$(_cxx_real_PROGRAM_D) : $(MK)/%.d : %.cxx
	@echo makeit: generating dependencies for $<
	@$(PREPROCESS.cxx) -DMAKEIT_DEPEND $< | \
	 $(DEPFILTER.cxx) | sed -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(BINEXT)%' \
	    -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%$$(MK)/$(*F).d%' > $@

_depend_SRC += $(_cxx_real_PROGRAM_D)

endif

ifneq "$(_cc_make_MK_PROGRAM_SRC)" ""

_cc_make_PROGRAM_D := $(patsubst %.cc,%.d,$(_cc_make_MK_PROGRAM_SRC))

$(_cc_make_PROGRAM_D) : $(MK)/%.d : $(MK)/%.cc
	@echo makeit: generating dependencies for $<
	@$(PREPROCESS.cxx) -DMAKEIT_DEPEND $< | \
	 $(DEPFILTER.cxx) | sed -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(BINEXT)%' \
	    -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%$$(MK)/$(*F).d%' > $@

_depend_SRC += $(_cc_make_PROGRAM_D)

endif

ifneq "$(_cpp_make_MK_PROGRAM_SRC)" ""

_cpp_make_PROGRAM_D := $(patsubst %.cpp,%.d,$(_cpp_make_MK_PROGRAM_SRC))

$(_cpp_make_PROGRAM_D) : $(MK)/%.d : $(MK)/%.cpp
	@echo makeit: generating dependencies for $<
	@$(PREPROCESS.cxx) -DMAKEIT_DEPEND $< | \
	 $(DEPFILTER.cxx) | sed -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(BINEXT)%' \
	    -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%$$(MK)/$(*F).d%' > $@

_depend_SRC += $(_cpp_make_PROGRAM_D)

endif

ifneq "$(_cxx_make_MK_PROGRAM_SRC)" ""

_cxx_make_PROGRAM_D := $(patsubst %.cxx,%.d,$(_cxx_make_MK_PROGRAM_SRC))

$(_cxx_make_PROGRAM_D) : $(MK)/%.d : $(MK)/%.cxx
	@echo makeit: generating dependencies for $<
	@$(PREPROCESS.cxx) -DMAKEIT_DEPEND $< | \
	 $(DEPFILTER.cxx) | sed -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(BINEXT)%' \
	    -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%$$(MK)/$(*F).d%' > $@

_depend_SRC += $(_cxx_make_PROGRAM_D)

endif
