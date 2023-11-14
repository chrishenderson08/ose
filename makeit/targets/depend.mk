## Copyright 1992-1993 OTC LIMITED
## Copyright 1995-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## Default name for dependency file.

override DEPEND_FILENAME := $(strip $(DEPEND_FILENAME))

ifeq "$(DEPEND_FILENAME)" ""
override DEPEND_FILENAME := $(firstword \
 $(notdir $(wildcard $(SRCDIR)/GNUMakefile)) \
 $(notdir $(wildcard $(SRCDIR)/makefile)) \
 $(notdir $(wildcard $(SRCDIR)/Makefile)) \
 Makefile).d
endif

## Default dependency filter. Eliminates any files not included from
## relative include directory.

override DEPEND_FILTER := $(strip $(DEPEND_FILTER))

ifeq "$(DEPEND_FILTER)" ""
override DEPEND_FILTER := sed -e "/ \/.*/d" -e "/ [a-zA-Z]:\/.*/d"
endif

## Don't bother with what is left unless we have to.

ifneq "$(_depend_SRC)" ""

ifneq "$(filter automatic_rebuild,$(DEPEND_OPTIONS))" ""
$(DEPEND_FILENAME) :: depend.setup
$(DEPEND_FILENAME) :: depend.target
$(DEPEND_FILENAME) :: depend.always
depend.always :: $(_depend_SRC)
else
depend.always :: $(_depend_SRC)
endif
	@$(RM) $(DEPEND_FILENAME)
	@echo makeit: updating $(DEPEND_FILENAME)
	@for i in $(_depend_SRC); do cat $$i | $(DEPEND_FILTER) | \
	  sed -e 's%$(MK)/%$$(MK)/%' >> $(DEPEND_FILENAME); done

## Clean targets.

mostlyclean.target ::
	$(RM) $(_depend_SRC)

realclean.always ::
	$(RM) $(DEPEND_FILENAME)

endif

## Include dependencies if they exist. Always check current directory
## before SRCDIR.

ifneq "$(wildcard $(DEPEND_FILENAME))" ""
include $(DEPEND_FILENAME)
else
ifneq "$(SRCDIR)" "."
ifneq "$(filter $(DEPEND_FILENAME),$(SRCFILES))" ""
include $(SRCDIR)/$(DEPEND_FILENAME)
endif
endif
endif
