## Copyright 1992-1993 OTC LIMITED
## Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

%.h :
%.i :

vpath %.c $(SRCDIR) $(SRCDIRS)

vpath %.h $(SRCDIR) $(SRCDIRS)
vpath %.i $(SRCDIR) $(SRCDIRS)

## Calculation of compiler flags to use for each source file.
## This magic has to cope with the feature whereby specific
## files can be compiled with flags from an alternate variant.
## This is required where heavy optimisation can break the
## compilation of specific files and thus they should not be
## compiled as optimised but within another variant. A big
## fiddle also has to be done on the include directory search
## path if we are compiling in a distinct workspace.

QUALIFIED_VARIANT.c = \
 $(firstword $(patsubst $(VARIANT)=%,%, \
  $(filter $(VARIANT)=%, $(patsubst $(notdir $<):%,%, \
  $(filter $(notdir $<):%,$(QUALIFIED_VARIANT))))) $(VARIANT))

QUALIFIED_CPPFLAGS.c = \
 $(patsubst $(notdir $<):%,%, \
  $(filter -% $(notdir $<):%,$(QUALIFIED_CPPFLAGS)))

ALL_CPPFLAGS.c = $(QUALIFIED_CPPFLAGS.c) \
 $(CPPFLAGS_$(QUALIFIED_VARIANT.c)) $(CPPFLAGS) \
 $(PROJECT_CPPFLAGS_$(QUALIFIED_VARIANT.c)) $(PROJECT_CPPFLAGS) \
 $(COMPILER_CPPFLAGS_$(QUALIFIED_VARIANT.c)) $(COMPILER_CPPFLAGS) \
 $(SYSTEM_CPPFLAGS_$(QUALIFIED_VARIANT.c)) $(SYSTEM_CPPFLAGS)

CPPFLAGS.c =

ifneq "$(SRCDIR)" "."

ifeq "$(filter -I.,$(ALL_CPPFLAGS.c))" ""
CPPFLAGS.c += -I$(SRCDIR) 
else
ifneq "$(firstword $(filter -I%,$(ALL_CPPFLAGS.c)))" "-I."
CPPFLAGS.c += -I. -I$(SRCDIR) 
endif
endif

CPPFLAGS.c += $(foreach opt,$(ALL_CPPFLAGS.c),$(opt) \
  $(patsubst -I%,-I$(SRCDIR)/%,$(filter-out -I/%, \
  $(filter -I%,$(filter-out -I$(MK),$(opt))))))

else

CPPFLAGS.c += $(ALL_CPPFLAGS.c)

endif

LIB_CPPFLAGS.c =  $(LIB_CPPFLAGS) $(PROJECT_LIB_CPPFLAGS) \
 $(COMPILER_LIB_CPPFLAGS) $(COMPILER_LIB_CPPFLAGS_$(VARIANT)) \
 $(SYSTEM_LIB_CPPFLAGS)

PIC_CPPFLAGS.c =  $(PIC_CPPFLAGS) $(PROJECT_PIC_CPPFLAGS) \
 $(COMPILER_PIC_CPPFLAGS) $(COMPILER_PIC_CPPFLAGS_$(VARIANT)) \
 $(SYSTEM_PIC_CPPFLAGS)

OBJ_CPPFLAGS.c =  $(OBJ_CPPFLAGS) $(PROJECT_OBJ_CPPFLAGS) \
 $(COMPILER_OBJ_CPPFLAGS) $(COMPILER_OBJ_CPPFLAGS_$(VARIANT)) \
 $(SYSTEM_OBJ_CPPFLAGS)

EXE_CPPFLAGS.c =  $(EXE_CPPFLAGS) $(PROJECT_EXE_CPPFLAGS) \
 $(COMPILER_EXE_CPPFLAGS) $(COMPILER_EXE_CPPFLAGS_$(VARIANT)) \
 $(SYSTEM_EXE_CPPFLAGS)

QUALIFIED_CFLAGS.c = \
 $(patsubst $(notdir $<):%,%, \
  $(filter -% $(notdir $<):%,$(QUALIFIED_CFLAGS)))

ALL_CFLAGS.c = $(QUALIFIED_CFLAGS.c) \
 $(CFLAGS_$(QUALIFIED_VARIANT.c)) $(CFLAGS) \
 $(PROJECT_CFLAGS_$(QUALIFIED_VARIANT.c)) $(PROJECT_CFLAGS) \
 $(COMPILER_CFLAGS_$(QUALIFIED_VARIANT.c)) $(COMPILER_CFLAGS) \
 $(SYSTEM_CFLAGS_$(QUALIFIED_VARIANT.c)) $(SYSTEM_CFLAGS)

CFLAGS.c = $(ALL_CFLAGS.c)

LIB_CFLAGS.c =  $(LIB_CFLAGS) $(PROJECT_LIB_CFLAGS) \
 $(COMPILER_LIB_CFLAGS) $(COMPILER_LIB_CFLAGS_$(VARIANT)) \
 $(SYSTEM_LIB_CFLAGS)

PIC_CFLAGS.c =  $(PIC_CFLAGS) $(PROJECT_PIC_CFLAGS) \
 $(COMPILER_PIC_CFLAGS) $(COMPILER_PIC_CFLAGS_$(VARIANT)) \
 $(SYSTEM_PIC_CFLAGS)

OBJ_CFLAGS.c =  $(OBJ_CFLAGS) $(PROJECT_OBJ_CFLAGS) \
 $(COMPILER_OBJ_CFLAGS) $(COMPILER_OBJ_CFLAGS_$(VARIANT)) \
 $(SYSTEM_OBJ_CFLAGS)

EXE_CFLAGS.c =  $(EXE_CFLAGS) $(PROJECT_EXE_CFLAGS) \
 $(COMPILER_EXE_CFLAGS) $(SYSTEM_EXE_CFLAGS)

QUALIFIED_LDFLAGS.c = \
 $(patsubst $(notdir $@):%,%, \
  $(filter -% $(notdir $@):%,$(QUALIFIED_LDFLAGS)))

ALL_LDFLAGS.c = $(QUALIFIED_LDFLAGS.c) \
 $(LDFLAGS_$(VARIANT)) $(LDFLAGS) \
 $(PROJECT_LDFLAGS_$(VARIANT)) $(PROJECT_LDFLAGS) \
 $(COMPILER_LDFLAGS_$(VARIANT)) $(COMPILER_LDFLAGS) \
 $(SYSTEM_LDFLAGS_$(VARIANT)) $(SYSTEM_LDFLAGS)

LDFLAGS.c = $(ALL_LDFLAGS.c)

QUALIFIED_LDLIBS.c = \
 $(patsubst $(notdir $@):%,%, \
  $(filter -% $(notdir $@):%,$(QUALIFIED_LDLIBS)))

ALL_LDLIBS.c = $(QUALIFIED_LDLIBS.c) \
 $(LDLIBS_$(VARIANT)) $(LDLIBS) \
 $(PROJECT_LDLIBS_$(VARIANT)) $(PROJECT_LDLIBS) \
 $(COMPILER_LDLIBS_$(VARIANT)) $(COMPILER_LDLIBS) \
 $(SYSTEM_LDLIBS_$(VARIANT)) $(SYSTEM_LDLIBS)

LDLIBS.c = \
 $(filter-out $< $(MK)/lib$(LIBEXT) $(ALL_LDLIBS.c) \
  $(patsubst -l%,\%/lib%$(LIBEXT),$(ALL_LDLIBS.c)), \
  $(filter %$(LIBEXT) %$(OBJEXT),$^)) \
 $(filter $(MK)/lib$(LIBEXT),$^) $(ALL_LDLIBS.c)

ifneq "$(filter no_minus_o_for_objects,$(COMPILER_ATTRIBUTES))" ""
TARGET.c = && mv $(basename $(<F))$(OBJEXT) $@
else
TARGET.c = -o $@
endif

## Build up basic commands for compilation only.

PREPROCESS.c = $(CC) -E $(CFLAGS.c) $(CPPFLAGS.c)

COMPILE.c = $(CC) $(CFLAGS.c) $(CPPFLAGS.c) -c

COMPILE-LIB.c = $(COMPILE.c) $(LIB_CPPFLAGS.c) $(LIB_CFLAGS.c)
COMPILE-PIC.c = $(COMPILE.c) $(PIC_CPPFLAGS.c) $(PIC_CFLAGS.c)
COMPILE-OBJ.c = $(COMPILE.c) $(OBJ_CPPFLAGS.c) $(OBJ_CFLAGS.c)
COMPILE-EXE.c = $(COMPILE.c) $(EXE_CPPFLAGS.c) $(EXE_CFLAGS.c)

COMPILE-LINK-EXE.c = $(LINK-EXE.c) $(EXE_CPPFLAGS.c) $(EXE_CFLAGS.c)
LINK-EXE.c = $(CC) $(CFLAGS.c) $(CPPFLAGS.c)

## Now construct the full commands for use in rules.
## This includes additional flags needing in linking.

RUN-COMPILE-LIB.c = \
 $(COMPILE-LIB.c) $< $(TARGET.c)

RUN-COMPILE-PIC.c = \
 $(COMPILE-PIC.c) $< $(TARGET.c)

RUN-COMPILE-OBJ.c = \
 $(COMPILE-OBJ.c) $< $(TARGET.c)

RUN-COMPILE-EXE.c = \
 $(COMPILE-EXE.c) $< $(TARGET.c)

RUN-COMPILE-LINK-EXE.c = \
 $(COMPILE-LINK-EXE.c) $< $(LDFLAGS.c) $(LDLIBS.c) \
  -o $@ || ($(RM) $@; exit 1)

RUN-LINK-EXE.c = \
 $(LINK-EXE.c) $< $(LDFLAGS.c) $(LDLIBS.c) \
  -o $@ || ($(RM) $@; exit 1)

## Wipe out any standard targets for creation of object files
## from source files and programs from source files for the
## source file extensions this module deals with.

%.o : %.c

% : %.c

%$(OBJEXT) : %.c

%$(BINEXT) : %.c

## Wipe out standard target for creation of object file from src file
## and program from src file.

%.o : %.c

%$(OBJEXT) : %.c
%$(BINEXT) : %.c

## Calculate all language source.

_c_real_SRC := \
 $(filter-out $(EXCLUDE),$(filter %.c,$(SRCFILES)))

_c_generated_MK_SRC := \
 $(addprefix $(MK)/,$(_c_generated_SRC))

_c_make_MK_SRC :=  \
 $(filter-out $(EXCLUDE),$(_c_generated_MK_SRC))

_c_make_SRC :=  \
 $(notdir $(_c_make_MK_SRC))

## Calculate program source.

_c_real_PROGRAM_SRC := \
 $(filter-out $(EXCLUDE),$(filter $(addsuffix .c,$(PROGRAMS)),$(SRCFILES)))

_c_generated_MK_PROGRAM_SRC := \
 $(addprefix $(MK)/,$(_c_generated_PROGRAM_SRC))

_c_make_MK_PROGRAM_SRC := \
 $(filter-out $(EXCLUDE),$(_c_generated_MK_PROGRAM_SRC))

_c_make_PROGRAM_SRC := \
 $(notdir $(_c_make_MK_PROGRAM_SRC))

## Calculate non library source.

_c_real_NONLIB_SRC := \
 $(filter-out $(EXCLUDE),$(filter %.c,$(NONLIBSRC)))

_c_generated_MK_NONLIB_SRC := \
 $(addprefix $(MK)/,$(_c_generated_NONLIB_SRC))

_c_make_MK_NONLIB_SRC := \
 $(filter-out $(EXCLUDE),$(_c_generated_MK_NONLIB_SRC))

_c_make_NONLIB_SRC := \
 $(notdir $(_c_make_MK_NONLIB_SRC))

## Calculate library source.

_c_real_LIB_SRC := \
 $(filter-out $(_c_real_NONLIB_SRC) \
  $(_c_real_PROGRAM_SRC), $(_c_real_SRC))

_c_make_MK_LIB_SRC := \
 $(filter-out $(_c_make_MK_NONLIB_SRC) \
  $(_c_make_MK_PROGRAM_SRC), $(_c_make_MK_SRC))

_c_make_LIB_SRC := \
 $(notdir $(_c_make_MK_LIB_SRC))

## Setup library dependencies for real source.

ifneq "$(_c_real_LIB_SRC)" ""

_c_real_LIB_OBJECTS := \
 $(patsubst %.c,%$(OBJEXT),$(_c_real_LIB_SRC))

_c_real_MK_LIB_OBJECTS := \
 $(addprefix $(MK)/,$(_c_real_LIB_OBJECTS))

$(_c_real_MK_LIB_OBJECTS) : $(MK)/%$(OBJEXT) : %.c
	$(RUN-COMPILE-LIB.c)

$(_c_real_LIB_OBJECTS) : %$(OBJEXT) : makeit.setup

$(_c_real_LIB_OBJECTS) : %$(OBJEXT) : $(MK)/%$(OBJEXT)

_c_real_MK_PIC_OBJECTS := \
 $(addprefix $(MK).pic/,$(_c_real_LIB_OBJECTS))

$(_c_real_MK_PIC_OBJECTS) : $(MK).pic/%$(OBJEXT) : %.c
	$(RUN-COMPILE-PIC.c)

_makeit_MK_LIB_OBJECTS += $(_c_real_MK_LIB_OBJECTS)

endif

## Setup library dependencies for generated source.

ifneq "$(_c_make_MK_LIB_SRC)" ""

_c_make_LIB_OBJECTS := \
 $(patsubst %.c,%$(OBJEXT),$(_c_make_LIB_SRC))

_c_make_MK_LIB_OBJECTS := \
 $(addprefix $(MK)/%,$(_c_make_LIB_OBJECTS))

$(_c_make_MK_LIB_OBJECTS) : $(MK)/%$(OBJEXT) : $(MK)/%.c
	$(RUN-COMPILE-LIB.c)

$(_c_make_LIB_OBJECTS) : %$(OBJEXT) : makeit.setup

$(_c_make_LIB_OBJECTS) : %$(OBJEXT) : $(MK)/%$(OBJEXT)

_c_make_MK_PIC_OBJECTS := \
 $(addprefix $(MK).pic/,$(_c_make_LIB_OBJECTS))

$(_c_make_MK_PIC_OBJECTS) : $(MK).pic/%$(OBJEXT) : $(MK)/%.c
	$(RUN-COMPILE-PIC.c)

_makeit_MK_LIB_OBJECTS += $(_c_make_MK_LIB_OBJECTS)

endif

## Setup dependencies for non library objects coming from real source.

ifneq "$(_c_real_NONLIB_SRC)" ""

_c_real_NONLIB_OBJECTS := \
 $(patsubst %.c,%$(OBJEXT),$(_c_real_NONLIB_SRC))

_c_real_MK_NONLIB_OBJECTS := \
 $(addprefix $(MK)/,$(_c_real_NONLIB_OBJECTS))

$(_c_real_MK_NONLIB_OBJECTS) : $(MK)/%$(OBJEXT) : %.c
	$(RUN-COMPILE-OBJ.c)

$(_c_real_NONLIB_OBJECTS) : %$(OBJEXT) : makeit.setup

$(_c_real_NONLIB_OBJECTS) : %$(OBJEXT) : $(MK)/%$(OBJEXT)

endif

## Setup dependencies for non library objects coming from generated source.

ifneq "$(_c_make_MK_NONLIB_SRC)" ""

_c_make_MK_NONLIB_OBJECTS := \
 $(patsubst %.c,%$(OBJEXT),$(_c_make_MK_NONLIB_SRC))

_c_make_NONLIB_OBJECTS := \
 $(notdir $(_c_make_MK_NONLIB_OBJECTS))

$(_c_make_MK_NONLIB_OBJECTS) : $(MK)/%$(OBJEXT) : $(MK)/%.c
	$(RUN-COMPILE-OBJ.c)

$(_c_make_NONLIB_OBJECTS) : %$(OBJEXT) : makeit.setup

$(_c_make_NONLIB_OBJECTS) : %$(OBJEXT) : $(MK)/%$(OBJEXT)

endif

## Setup dependencies for programs coming from real source.

ifneq "$(_c_real_PROGRAM_SRC)" ""

_c_real_PROGRAM_OBJECTS := \
 $(patsubst %.c,%$(OBJEXT),$(_c_real_PROGRAM_SRC))

_c_real_MK_PROGRAM_OBJECTS := \
 $(addprefix $(MK)/,$(_c_real_PROGRAM_OBJECTS))

_c_real_MK_PROGRAMS := \
 $(addsuffix $(BINEXT),$(addprefix $(MK)/,$(basename $(_c_real_PROGRAM_SRC))))

$(_c_real_MK_PROGRAMS) : $(MK)/%$(BINEXT) : $(MK)/%$(OBJEXT)
	@$(RM) $@
	$(RUN-LINK-EXE.c)

$(_c_real_PROGRAM_OBJECTS) : %$(OBJEXT) : makeit.setup

$(_c_real_PROGRAM_OBJECTS) : %$(OBJEXT) : $(MK)/%$(OBJEXT)

$(_c_real_MK_PROGRAM_OBJECTS) : $(MK)/%$(OBJEXT) : %.c
	$(RUN-COMPILE-EXE.c)

_makeit_MK_BINARIES += $(_c_real_MK_PROGRAMS)

endif

## Setup dependencies for programs coming from generated source.

ifneq "$(_c_make_MK_PROGRAM_SRC)" ""

_c_make_MK_PROGRAM_OBJECTS := \
 $(patsubst %.c,%$(OBJEXT),$(_c_make_MK_PROGRAM_SRC))

_c_make_PROGRAM_OBJECTS := \
 $(notdir $(_c_make_MK_PROGRAM_OBJECTS))

_c_make_MK_PROGRAMS := \
 $(addsuffix $(BINEXT),$(basename $(_c_make_MK_PROGRAM_SRC)))

$(_c_make_MK_PROGRAMS) : $(MK)/%$(BINEXT) : $(MK)/%$(OBJEXT)
	@$(RM) $@
	$(RUN-LINK-EXE.c)

$(_c_make_PROGRAM_OBJECTS) : %$(OBJEXT) : makeit.setup

$(_c_make_PROGRAM_OBJECTS) : %$(OBJEXT) : $(MK)/%$(OBJEXT)

$(_c_make_MK_PROGRAM_OBJECTS) : $(MK)/%$(OBJEXT) : $(MK)/%.c
	$(RUN-COMPILE-EXE.c)

_makeit_MK_BINARIES += $(_c_make_MK_PROGRAMS)

endif

## Dependency files.

DEPFILTER.c = sed -f $(MAKEIT_HOME)/targets/depend.sed | sort -u

ifneq "$(_c_real_LIB_SRC)" ""

_c_real_LIB_D := $(patsubst %.c,$(MK)/%.d,$(_c_real_LIB_SRC))

$(_c_real_LIB_D) : $(MK)/%.d : %.c
	@echo makeit: generating dependencies for $<
	@$(PREPROCESS.c) -DMAKEIT_DEPEND $< | $(DEPFILTER.c) | \
	sed -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%TARGET $$(MK).pic/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%$$(MK)/$(*F).d%' > $@

_depend_SRC += $(_c_real_LIB_D)

endif

ifneq "$(_c_make_MK_LIB_SRC)" ""

_c_make_LIB_D := $(patsubst %.c,%.d,$(_c_make_MK_LIB_SRC))

$(_c_make_LIB_D) : $(MK)/%.d : $(MK)/%.c
	@echo makeit: generating dependencies for $<
	@$(PREPROCESS.c) -DMAKEIT_DEPEND $< | $(DEPFILTER.c) | \
	sed -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%TARGET $$(MK).pic/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%$$(MK)/$(*F).d%' > $@

_depend_SRC += $(_c_make_LIB_D)

endif

ifneq "$(_c_real_NONLIB_SRC)" ""

_c_real_NONLIB_D := $(patsubst %.c,$(MK)/%.d,$(_c_real_NONLIB_SRC))

$(_c_real_NONLIB_D) : $(MK)/%.d : %.c
	@echo makeit: generating dependencies for $<
	@$(PREPROCESS.c) -DMAKEIT_DEPEND $< | $(DEPFILTER.c) | \
	sed -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%$$(MK)/$(*F).d%' > $@

_depend_SRC += $(_c_real_NONLIB_D)

endif

ifneq "$(_c_make_MK_NONLIB_SRC)" ""

_c_make_NONLIB_D := $(patsubst %.c,%.d,$(_c_make_MK_NONLIB_SRC))

$(_c_make_NONLIB_D) : $(MK)/%.d : $(MK)/%.c
	@echo makeit: generating dependencies for $<
	@$(PREPROCESS.c) -DMAKEIT_DEPEND $< | $(DEPFILTER.c) | \
	sed -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%$$(MK)/$(*F).d%' > $@

_depend_SRC += $(_c_make_NONLIB_D)

endif

ifneq "$(_c_real_PROGRAM_SRC)" ""

_c_real_PROGRAM_D := $(patsubst %.c,$(MK)/%.d,$(_c_real_PROGRAM_SRC))

$(_c_real_PROGRAM_D) : $(MK)/%.d : %.c
	@echo makeit: generating dependencies for $<
	@$(PREPROCESS.c) -DMAKEIT_DEPEND $< | $(DEPFILTER.c) | \
	sed -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(BINEXT)%' \
	    -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%$$(MK)/$(*F).d%' > $@

_depend_SRC += $(_c_real_PROGRAM_D)

endif

ifneq "$(_c_make_MK_PROGRAM_SRC)" ""

_c_make_PROGRAM_D := $(patsubst %.c,%.d,$(_c_make_MK_PROGRAM_SRC))

$(_c_make_PROGRAM_D) : $(MK)/%.d : $(MK)/%.c
	@echo makeit: generating dependencies for $<
	@$(PREPROCESS.c) -DMAKEIT_DEPEND $< | $(DEPFILTER.c) | \
	sed -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(BINEXT)%' \
	    -e 's%^TARGET%TARGET $$(MK)/$(*F)$$(OBJEXT)%' \
	    -e 's%^TARGET%$$(MK)/$(*F).d%' > $@

_depend_SRC += $(_c_make_PROGRAM_D)

endif
