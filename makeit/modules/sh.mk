## Copyright 1992 OTC LIMITED
## Copyright 1995-2000 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

vpath %.sh $(SRCDIR) $(SRCDIRS)

## Wipe out standard target for creation of object file from src file
## and program from src file.

% : %.sh

## Calculate language src.

_sh_real_SRC := \
 $(filter-out $(EXCLUDE),$(filter $(addsuffix .sh,$(PROGRAMS)),$(SRCFILES)))

_sh_generated_SRC := \
 $(filter-out $(EXCLUDE),$(addprefix $(MK)/,$(_sh_generated_SRC)))

## Setup dependencies.

ifeq "$(SHFILTER)" ""
ifneq "$(SHPATSUBST)" ""
SHFILTER := sed $(patsubst %,-e %,$(SHPATSUBST))
endif
endif

ifneq "$(_sh_real_SRC)" ""

_sh_real_SCRIPTS := $(basename $(_sh_real_SRC))

_sh_real_MK_SCRIPTS := \
 $(patsubst %,$(MK)/%$(CMDEXT),$(_sh_real_SCRIPTS))

$(_sh_real_MK_SCRIPTS) : $(MK)/%$(CMDEXT) : %.sh
	$(RM) $@
ifneq "$(SHFILTER)" ""
	cat $< | $(SHFILTER) > $@
else
	cp $< $@
endif
	chmod 0775 $@

$(_sh_real_SCRIPTS) : % : makeit.setup

$(_sh_real_SCRIPTS) : % : $(MK)/%$(CMDEXT)

_makeit_MK_SCRIPTS += $(_sh_real_MK_SCRIPTS)

endif

ifneq "$(_sh_generated_SRC)" ""

_sh_generated_SCRIPTS := $(notdir $(basename $(_sh_generated_SRC)))

_sh_generated_MK_SCRIPTS := \
 $(patsubst %,$(MK)/%$(CMDEXT),$(_sh_generated_SCRIPTS))

$(_sh_generated_MK_SCRIPTS) : $(MK)/%$(CMDEXT) : $(MK)/%.sh
	$(RM) $@
ifneq "$(SHFILTER)" ""
	cat $< | $(SHFILTER) > $@
else
	cp $< $@
endif
	chmod 0775 $@

$(_sh_generated_SCRIPTS) : % : makeit.setup

$(_sh_generated_SCRIPTS) : % : $(MK)/%$(CMDEXT)

_makeit_MK_SCRIPTS += $(_sh_generated_MK_SCRIPTS)

endif
