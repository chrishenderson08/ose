## Copyright 1992-1993 OTC LIMITED
## Copyright 1995-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

% : %$(OBJEXT)

## Add binaries and scripts to general class of executables.

_makeit_MK_SCRIPTS := $(strip $(_makeit_MK_SCRIPTS))
_makeit_MK_BINARIES := $(strip $(_makeit_MK_BINARIES))

_makeit_MK_EXECUTABLES := \
 $(strip $(_makeit_MK_SCRIPTS) $(_makeit_MK_BINARIES))

## Setup targets so that programs will be built when the targets
## "all" and "programs" are used.

all.always :: programs

programs.always :: $(_makeit_MK_EXECUTABLES)

## Setup targets so that user doesn't have to give name of program
## within makeit subdirectory in order to build it.

_makeit_EXECUTABLES := \
 $(patsubst %$(BINEXT),%,$(notdir $(_makeit_MK_EXECUTABLES)))

ifneq "$(_makeit_EXECUTABLES)" ""

$(_makeit_EXECUTABLES) : % : makeit.setup

$(_makeit_EXECUTABLES) : % : $(MK)/%$(BINEXT)

endif

## Setup dependencies for binaries on library in this directory
## if a library exists and if binaries require library. If the
## library is being suppressed and program would otherwise have
## been linked against library it will instead be linked against
## all the object files which would make up the library.

ifneq "$(_makeit_MK_LIB_OBJECTS)" ""

_makeit_MK_LIB_BINARIES := \
 $(filter-out $(patsubst %,$(MK)/%$(BINEXT),$(NONLIBPROGRAMS)), \
 $(_makeit_MK_BINARIES))

ifneq "$(_makeit_MK_LIB_BINARIES)" ""

ifeq "$(filter suppress_archive,$(LIB_OPTIONS))" ""

$(_makeit_MK_LIB_BINARIES) : % : $(MK)/lib$(LIBEXT)

else

$(_makeit_MK_LIB_BINARIES) : % : $(_makeit_MK_LIB_OBJECTS)

endif

endif

endif

## Setup dependencies for binaries on libraries and objects coming
## from other locations and non library objects coming from this
## directory.

ifeq "$(filter suppress_dependencies,$(PROGRAM_OPTIONS))" ""

ifneq "$(_makeit_MK_BINARIES)" ""

$(_makeit_MK_BINARIES) : % : $(filter %$(OBJEXT),$(LDLIBS))

$(_makeit_MK_BINARIES) : % : $(filter %$(LIBEXT),$(LDLIBS))

endif

endif

mostlyclean.target :: mostlyclean.setup
ifneq "$(_makeit_MK_BINARIES)" ""
	$(RM) $(addsuffix $(OBJEXT),$(_makeit_MK_BINARIES))
endif

mostlyclean.always :: mostlyclean.setup
ifneq "$(filter core a.out,$(SRCFILES))" ""
	$(RM) core a.out
endif
ifeq "$(VARIANT)" "prf"
ifneq "$(filter mon.out gmon.out,$(SRCFILES))" ""
	$(RM) mon.out gmon.out
endif
endif
