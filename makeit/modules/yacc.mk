## Copyright 1992-1993 OTC LIMITED
## Copyright 1995-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

vpath %.y $(SRCDIR) $(SRCDIRS)

## Stop redundant checks on lex input file.

%.y :

y.tab.c :
y.tab.h :

## Try and identify where yacc is.

ifeq "$(origin YACC)" "default"
ifneq "$(wildcard /usr/bin/yacc)" ""
YACC = /usr/bin/yacc
endif
endif

## Exclude y.tab.c so it isn't compiled as C file. Don't remove it though.

override EXCLUDE += y.tab.c

## List makeit subdirectory and current directory in include search path.

ifeq "$(filter -I.,$(CPPFLAGS))" ""
override CPPFLAGS += -I.
endif

ifeq "$(filter -I./$(MK),$(CPPFLAGS))" ""
override CPPFLAGS += -I./$(MK)
endif

## Work out options.

ifneq "$(filter cxx,$(YACC_OPTIONS))" ""
_yacc_cc := YES
MAKEIT_REQUIRED_MODULES += cxx
else
MAKEIT_REQUIRED_MODULES += c
endif

ifneq "$(filter archive_in_library,$(YACC_OPTIONS))" ""
_yacc_archive_in_library := YES
endif

## If generating C++ then make yacc++ default generator.

ifeq "$(_yacc_cc)" "YES"
ifeq "$(origin YACC)" "default"
YACC := yacc++
endif
endif

## Calculate language src.

_yacc_real_SRC := $(filter-out $(EXCLUDE),$(filter %.y,$(SRCFILES)))

_yacc_generated_SRC := \
 $(filter-out $(EXCLUDE),$(addprefix $(MK)/,$(_yacc_generated_SRC)))

## Generate stems for src.

_yacc_real_SRC_STEMS := $(basename $(_yacc_real_SRC))

_yacc_generated_SRC_STEMS := $(notdir $(basename $(_yacc_generated_SRC)))

## Let C/C++ module know about generated src files.

ifeq "$(_yacc_cc)" "YES"

_yacc_OUTPUT_SRC := \
 $(addsuffix .cc,$(_yacc_real_SRC_STEMS) $(_yacc_generated_SRC_STEMS))

ifeq "$(_yacc_archive_in_library)" "YES"
_cc_generated_SRC += $(_yacc_OUTPUT_SRC)
else
_cc_generated_NONLIB_SRC += $(_yacc_OUTPUT_SRC)
endif

else

_yacc_OUTPUT_SRC := \
 $(addsuffix .c,$(_yacc_real_SRC_STEMS) $(_yacc_generated_SRC_STEMS))

ifeq "$(_yacc_archive_in_library)" "YES"
_c_generated_SRC += $(_yacc_OUTPUT_SRC)
else
_c_generated_NONLIB_SRC += $(_yacc_OUTPUT_SRC)
endif

endif

## Rules for generated files.

ifneq "$(_yacc_real_SRC_STEMS)" ""
$(patsubst %,$(MK)/%$(OBJEXT),$(_yacc_real_SRC_STEMS)) \
 : $(MK)/%$(OBJEXT) : $(MK)/%.h
depend.setup :: $(patsubst %,$(MK)/%.h,$(_yacc_real_SRC_STEMS))
endif

ifneq "$(_yacc_generated_SRC_STEMS)" ""
$(patsubst %,$(MK)/%$(OBJEXT),$(_yacc_generated_SRC_STEMS)) \
 : $(MK)/%$(OBJEXT) : $(MK)/%.h
depend.setup :: $(patsubst %,$(MK)/%.h,$(_yacc_generated_SRC_STEMS))
endif

YACCFIX.c = \
 sed -e 's/^\# *line \(.*\)"y.tab.c"\(.*\)$$/\#line \1"$(subst /,\/,$(MK)/$(<F:.y=.c))"\2/'

YACCFIX.cc = \
 sed -e 's/^\# *line \(.*\)"y.tab.c"\(.*\)$$/\#line \1"$(subst /,\/,$(MK)/$(<F:.y=.cc))"\2/'

%.cc : %.y

%.c : %.y

$(MK)/%.cc $(MK)/%.h : %.y
	$(YACC) -d $(YFLAGS) $<
ifneq "$(YACCFILTER)" ""
	cat y.tab.c | $(YACCFIX.cc) | $(YACCFILTER) > $(MK)/$(<F:.y=.cc)
	$(RM) y.tab.c
else
	cat y.tab.c | $(YACCFIX.cc) > $(MK)/$(<F:.y=.cc)
	$(RM) y.tab.c
endif
	mv y.tab.h $(MK)/$(<F:.y=.h)

$(MK)/%.cc $(MK)/%.h : $(MK)/%.y
	$(YACC) -d $(YFLAGS) $<
ifneq "$(YACCFILTER)" ""
	cat y.tab.c | $(YACCFIX.cc) | $(YACCFILTER) > $(<F:.y=.cc)
	$(RM) y.tab.c
else
	cat y.tab.c | $(YACCFIX.cc) > $(<F:.y=.cc)
	$(RM) y.tab.c
endif
	mv y.tab.h $(<F:.y=.h)

$(MK)/%.c $(MK)/%.h : %.y
	$(YACC) -d $(YFLAGS) $<
ifneq "$(YACCFILTER)" ""
	cat y.tab.c | $(YACCFIX.c) | $(YACCFILTER) > $(MK)/$(<F:.y=.c)
	$(RM) y.tab.c
else
	cat y.tab.c | $(YACCFIX.c) > $(MK)/$(<F:.y=.c)
	$(RM) y.tab.c
endif
	mv y.tab.h $(MK)/$(<F:.y=.h)

$(MK)/%.c $(MK)/%.h : $(MK)/%.y
	$(YACC) -d $(YFLAGS) $<
ifneq "$(YACCFILTER)" ""
	cat y.tab.c | $(YACCFIX.c) | $(YACCFILTER) > $(<F:.y=.c)
	$(RM) y.tab.c
else
	cat y.tab.c | $(YACCFIX.c) > $(<F:.y=.c)
	$(RM) y.tab.c
endif
	mv y.tab.h $(<F:.y=.h)

## Add yacc library.

ifeq "$(origin YACCLIB)" "undefined"
YACCLIB := -ly
endif

override LDLIBS += $(YACCLIB)
