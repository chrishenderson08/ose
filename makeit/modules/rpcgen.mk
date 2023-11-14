## Copyright 1992 OTC LIMITED
## Copyright 1995-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

vpath %.x $(SRCDIR) $(SRCDIRS)

## Definitions.

ifeq "$(origin RPCGEN)" "undefined"
ifneq "$(wildcard /usr/bin/rpcgen)" ""
RPCGEN = /usr/bin/rpcgen
else
RPCGEN := rpcgen
endif
endif

ifeq "$(origin RPCGENXDRFLAGS)" "undefined"
RPCGENXDRFLAGS := -c
endif

ifeq "$(origin RPCGENSVCFLAGS)" "undefined"
RPCGENSVCFLAGS := -s tcp -s udp
endif

ifeq "$(origin RPCGENCLNTFLAGS)" "undefined"
RPCGENCLNTFLAGS := -l
endif

ifeq "$(origin RPCGENTBLFLAGS)" "undefined"
RPCGENTBLFLAGS := -t
endif

ifeq "$(origin RPCGENHDRFLAGS)" "undefined"
RPCGENHDRFLAGS := -h
endif

## Stop redundant checks on rpcgen input files.

%.x :

## List makeit subdirectory and current in include search path.

ifeq "$(filter -I.,$(CPPFLAGS))" ""
override CPPFLAGS += -I.
endif

ifeq "$(filter -I./$(MK),$(CPPFLAGS))" ""
override CPPFLAGS += -I./$(MK)
endif

## Work out options.

ifneq "$(filter cxx,$(RPCGEN_OPTIONS))" ""
_x_OUTPUT_EXT := cc
MAKEIT_REQUIRED_MODULES += cxx
else
_x_OUTPUT_EXT := c
MAKEIT_REQUIRED_MODULES += c
endif

## Calculate language src.

_x_real_SRC := $(filter-out $(EXCLUDE),$(filter %.x,$(SRCFILES)))

_x_generated_SRC := \
 $(filter-out $(EXCLUDE),$(addprefix $(MK)/,$(_x_generated_SRC)))

## Generate stems for src.

_x_real_SRC_STEMS := $(basename $(_x_real_SRC))

_x_generated_SRC_STEMS := $(notdir $(basename $(_x_generated_SRC)))

## Let C or C++ modules know about generated src files.

_x_OUTPUT_SRC := \
 $(addsuffix _clnt.$(_x_OUTPUT_EXT),$(_x_real_SRC_STEMS)) \
 $(addsuffix _xdr.$(_x_OUTPUT_EXT),$(_x_real_SRC_STEMS)) \
 $(addsuffix _clnt.$(_x_OUTPUT_EXT),$(_x_generated_SRC_STEMS)) \
 $(addsuffix _xdr.$(_x_OUTPUT_EXT),$(_x_generated_SRC_STEMS))

_x_OUTPUT_PROGRAMS := \
 $(filter $(PROGRAMS),$(addsuffix _svc,$(_x_real_SRC_STEMS))) \
 $(filter $(PROGRAMS),$(addsuffix _svc,$(_x_generated_SRC_STEMS)))

_x_OUTPUT_PROGRAM_SRC := \
 $(addsuffix .$(_x_OUTPUT_EXT),$(_x_OUTPUT_PROGRAMS))

_x_OUTPUT_NONLIB_SRC := \
 $(filter-out $(_x_OUTPUT_PROGRAM_SRC), \
 $(addsuffix _svc.$(_x_OUTPUT_EXT),$(_x_real_SRC_STEMS)) \
 $(addsuffix _svc.$(_x_OUTPUT_EXT),$(_x_generated_SRC_STEMS)))

ifeq "$(filter cxx,$(RPCGEN_OPTIONS))" ""

_c_generated_SRC += $(_x_OUTPUT_SRC)
_c_generated_PROGRAM_SRC += $(_x_OUTPUT_PROGRAM_SRC)
_c_generated_NONLIB_SRC += $(_x_OUTPUT_NONLIB_SRC)

else

_cc_generated_SRC += $(_x_OUTPUT_SRC)
_cc_generated_PROGRAM_SRC += $(_x_OUTPUT_PROGRAM_SRC)
_cc_generated_NONLIB_SRC += $(_x_OUTPUT_NONLIB_SRC)

endif

## Rules for generated files.

ifneq "$(_x_real_SRC_STEMS)" ""

$(patsubst %,$(MK)/%_svc,$(_x_real_SRC_STEMS)) \
 : $(MK)/%_svc : $(MK)/%.h

$(patsubst %,$(MK)/%_svc$(OBJEXT),$(_x_real_SRC_STEMS)) \
 : $(MK)/%_svc$(OBJEXT) : $(MK)/%.h

$(patsubst %,$(MK)/%_clnt$(OBJEXT),$(_x_real_SRC_STEMS)) \
 : $(MK)/%_clnt$(OBJEXT) : $(MK)/%.h

$(patsubst %,$(MK)/%_xdr$(OBJEXT),$(_x_real_SRC_STEMS)) \
 : $(MK)/%_xdr$(OBJEXT) : $(MK)/%.h

endif

ifneq "$(_x_generated_SRC_STEMS)" ""

$(patsubst %,$(MK)/%_svc,$(_x_generated_SRC_STEMS)) \
 : $(MK)/%_svc : $(MK)/%.h

$(patsubst %,$(MK)/%_svc$(OBJEXT),$(_x_generated_SRC_STEMS)) \
 : $(MK)/%_svc$(OBJEXT) : $(MK)/%.h

$(patsubst %,$(MK)/%_clnt$(OBJEXT),$(_x_generated_SRC_STEMS)) \
 : $(MK)/%_clnt$(OBJEXT) : $(MK)/%.h

$(patsubst %,$(MK)/%_xdr$(OBJEXT),$(_x_generated_SRC_STEMS)) \
 : $(MK)/%_xdr$(OBJEXT) : $(MK)/%.h

endif

$(MK)/%.h : %.x
	$(RPCGEN) $(RPCGENHDRFLAGS) $(RPCGENFLAGS) -o $@ $<

$(MK)/%.h : $(MK)/%.x
	$(RPCGEN) $(RPCGENHDRFLAGS) $(RPCGENFLAGS) -o $@ $<

$(MK)/%_clnt.c : %.x
	$(RPCGEN) $(RPCGENCLNTFLAGS) $(RPCGENFLAGS) -o $@ $<

$(MK)/%_clnt.cc : %.x
	$(RPCGEN) $(RPCGENCLNTFLAGS) $(RPCGENFLAGS) -o $@ $<

$(MK)/%_clnt.c : $(MK)/%.x
	$(RPCGEN) $(RPCGENCLNTFLAGS) $(RPCGENFLAGS) -o $@ $<

$(MK)/%_clnt.cc : $(MK)/%.x
	$(RPCGEN) $(RPCGENCLNTFLAGS) $(RPCGENFLAGS) -o $@ $<

$(MK)/%_svc.c : %.x
	$(RPCGEN) $(RPCGENSVCFLAGS) $(RPCGENFLAGS) -o $@ $<

$(MK)/%_svc.cc : %.x
	$(RPCGEN) $(RPCGENSVCFLAGS) $(RPCGENFLAGS) -o $@ $<

$(MK)/%_svc.c : $(MK)/%.x
	$(RPCGEN) $(RPCGENSVCFLAGS) $(RPCGENFLAGS) -o $@ $<

$(MK)/%_svc.cc : $(MK)/%.x
	$(RPCGEN) $(RPCGENSVCFLAGS) $(RPCGENFLAGS) -o $@ $<

$(MK)/%_xdr.c : %.x
	$(RPCGEN) $(RPCGENXDRFLAGS) $(RPCGENFLAGS) -o $@ $<

$(MK)/%_xdr.cc : %.x
	$(RPCGEN) $(RPCGENXDRFLAGS) $(RPCGENFLAGS) -o $@ $<

$(MK)/%_xdr.c : $(MK)/%.x
	$(RPCGEN) $(RPCGENXDRFLAGS) $(RPCGENFLAGS) -o $@ $<

$(MK)/%_xdr.cc : $(MK)/%.x
	$(RPCGEN) $(RPCGENXDRFLAGS) $(RPCGENFLAGS) -o $@ $<
