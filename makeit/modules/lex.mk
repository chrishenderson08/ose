## Copyright 1992-1993 OTC LIMITED
## Copyright 1995-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

vpath %.l $(SRCDIR) $(SRCDIRS)

## Stop redundant checks on lex input file.

%.l :
%$(OBJEXT) : %.f
%$(OBJEXT) : %.l
%$(OBJEXT) : %.r

## Try and identify where flex or lex is located.

ifeq "$(origin LEX)" "default"
ifneq "$(wildcard /usr/bin/flex)" ""
LEX = /usr/bin/flex
else
ifneq "$(wildcard /usr/bin/lex)" ""
LEX = /usr/bin/lex
endif
endif
endif

## Exclude lex.yy.c and lex.yy.cc so they aren't compiled.
## Don't remove it though.

override EXCLUDE += lex.yy.c
override EXCLUDE += lex.yy.cc

## List current directory in include search path.

ifeq "$(filter -I.,$(CPPFLAGS))" ""
override CPPFLAGS += -I.
endif

## Work out options.

ifneq "$(filter cxx,$(LEX_OPTIONS))" ""
_lex_cc := YES
MAKEIT_REQUIRED_MODULES += cxx
else
MAKEIT_REQUIRED_MODULES += c
endif

ifneq "$(filter archive_in_library,$(LEX_OPTIONS))" ""
_lex_archive_in_library := YES
endif

## If generating C++ then make lex++ default generator
## if version of lex hasn't been defined explicitly.

ifeq "$(_lex_cc)" "YES"
ifeq "$(origin LEX)" "default"
LEX := lex++
endif
endif

## Look to see if flex is being used.

ifneq "$(filter flex %/flex,$(LEX))" ""
override LFLAGS := $(filter-out -o%,$(LFLAGS))
override LFLAGS += -olex.yy.c
LEXLIB := -lfl
else
LEXLIB := -ll
endif

## Calculate language src.

_lex_real_SRC := $(filter-out $(EXCLUDE),$(filter %.l,$(SRCFILES)))

_lex_generated_SRC := \
 $(filter-out $(EXCLUDE),$(addprefix $(MK)/,$(_lex_generated_SRC)))

## Generate stems for src.

_lex_real_SRC_STEMS := $(basename $(_lex_real_SRC))

_lex_generated_SRC_STEMS := $(notdir $(basename $(_lex_generated_SRC)))

## Let C/C++ module know about generated src files.

ifeq "$(_lex_cc)" "YES"

_lex_OUTPUT_SRC := \
 $(addsuffix .cc,$(_lex_real_SRC_STEMS) $(_lex_generated_SRC_STEMS))

ifeq "$(_lex_archive_in_library)" "YES"
_cc_generated_SRC += $(_lex_OUTPUT_SRC)
else
_cc_generated_NONLIB_SRC += $(_lex_OUTPUT_SRC)
endif

else

_lex_OUTPUT_SRC := \
 $(addsuffix .c,$(_lex_real_SRC_STEMS) $(_lex_generated_SRC_STEMS))

ifeq "$(_lex_archive_in_library)" "YES"
_c_generated_SRC += $(_lex_OUTPUT_SRC)
else
_c_generated_NONLIB_SRC += $(_lex_OUTPUT_SRC)
endif

endif

## Rules for generated files.

LEXFIX.c = \
 sed -e 's/^\# *line \(.*\)"lex.yy.c"\(.*\)$$/\#line \1"$(subst /,\/,$(MK)/$(<F:.l=.c))"\2/'

LEXFIX.cc = \
 sed -e 's/^\# *line \(.*\)"lex.yy.c"\(.*\)$$/\#line \1"$(subst /,\/,$(MK)/$(<F:.l=.cc))"\2/'

ifeq "$(_lex_cc)" "YES"

%.cc : %.l

ifneq "$(_lex_real_SRC_STEMS)" ""
$(patsubst %,$(MK)/%.cc,$(_lex_real_SRC_STEMS)) : $(MK)/%.cc : %.l
	$(LEX) $(LFLAGS) $<
ifneq "$(LEXFILTER)" ""
	cat lex.yy.c | $(LEXFIX.cc) | $(LEXFILTER) > $@
	$(RM) lex.yy.c
else
	cat lex.yy.c | $(LEXFIX.cc) > $@
	$(RM) lex.yy.c
endif
endif

ifneq "$(_lex_generated_SRC_STEMS)" ""
$(patsubst %,$(MK)/%.cc,$(_lex_generated_SRC_STEMS)) : $(MK)/%.cc : $(MK)/%.l
	$(LEX) $(LFLAGS) $<
ifneq "$(LEXFILTER)" ""
	cat lex.yy.c | $(LEXFIX.cc) | $(LEXFILTER) > $@
	$(RM) lex.yy.c
else
	cat lex.yy.c | $(LEXFIX.cc) > $@
	$(RM) lex.yy.c
endif
endif

else

%.c : %.l

ifneq "$(_lex_real_SRC_STEMS)" ""
$(patsubst %,$(MK)/%.c,$(_lex_real_SRC_STEMS)) : $(MK)/%.c : %.l
	$(LEX) $(LFLAGS) $<
ifneq "$(LEXFILTER)" ""
	cat lex.yy.c | $(LEXFIX.c) | $(LEXFILTER) > $@
	$(RM) lex.yy.c
else
	cat lex.yy.c | $(LEXFIX.c) > $@
	$(RM) lex.yy.c
endif
endif

ifneq "$(_lex_generated_SRC_STEMS)" ""
$(patsubst %,$(MK)/%.c,$(_lex_generated_SRC_STEMS)) : $(MK)/%.c : $(MK)/%.l
	$(LEX) $(LFLAGS) $<
ifneq "$(LEXFILTER)" ""
	cat lex.yy.c | $(LEXFIX.c) | $(LEXFILTER) > $@
	$(RM) lex.yy.c
else
	cat lex.yy.c | $(LEXFIX.c) > $@
	$(RM) lex.yy.c
endif
endif

endif

## Add lex library.

override LDLIBS += $(LEXLIB)
