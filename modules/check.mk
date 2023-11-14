## Copyright 1992-1993 OTC LIMITED
## Copyright 1994-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## Standard targets.

.PHONY : check
.PHONY : check.setup
.PHONY : check.subdirs
.PHONY : check.target
.PHONY : check.always

check : check.setup
check : check.subdirs
check : check.target
check : check.always

check.setup :: makeit.setup

check.subdirs :: check.setup
ifneq "$(SUBDIRS)" ""
	@$(foreach dir,$(SUBDIRS), $(MAKE) -C $(dir) check || $(if \
	  $(findstring k,$(firstword $(filter-out --%,$(MAKEFLAGS)))), \
	  true,exit 1);)
endif

check.target :: check.setup

check.always :: check.setup

## Calculate output files.

ifneq "$(filter short_suffixes,$(CHECK_OPTIONS))" ""
_check_short_suffixes := YES
endif

ifneq "$(CHECK_INSUFFIX)" ""
_check_in := $(CHECK_INSUFFIX)
else
ifeq "$(_check_short_suffixes)" "YES"
_check_in := i.
else
_check_in := in.
endif
endif

ifneq "$(CHECK_OUTSUFFIX)" ""
_check_out := $(CHECK_OUTSUFFIX)
else
ifeq "$(_check_short_suffixes)" "YES"
_check_out := o.
else
_check_out := out.
endif
endif

_check_SRC := \
 $(filter $(addsuffix .$(_check_out)%,$(PROGRAMS)),$(SRCFILES))

ifneq "$(_check_SRC)" ""

## Run tests.

check.always :: lib programs
	@for i in $(_check_SRC); \
	do \
	  FILE=`basename $$i`; \
	  PROG=`echo $$FILE | sed -e 's/\.$(_check_out)..*$$//'`; \
	  TEST=`echo $$FILE | sed -e 's/^.*\.$(_check_out)//'`; \
	  if test "$(SRCDIRS)" != ""; \
	  then \
	    for d in $(SRCDIRS); \
	    do \
	      if test -f $$d/$$FILE; \
	      then \
		SRCDIR=$$d; \
	      fi; \
	    done; \
	  fi; \
	  if test "$$SRCDIR" = ""; \
	  then \
	    SRCDIR=$(SRCDIR); \
	  fi; \
	  echo "**** Running test number $$TEST of $$PROG ****"; \
	  echo "Using $$i as expected output"; \
	  $(RM) $$PROG.errs; \
	  if test -f $$SRCDIR/$$PROG.$(_check_in)$$TEST; \
	  then \
	    echo "Using $$PROG.$(_check_in)$$TEST as input"; \
	    OTCLIB_TRACELEVEL=1 OTCLIB_LOGEXCEPTION=1 \
	    OTCLIB_LOGFILE=$(MK)/$$i.err OTCLIB_NOLOGSTDERR=1 \
	     $(MK)/$$PROG $$TEST < $$SRCDIR/$$PROG.$(_check_in)$$TEST > \
	     $(MK)/$$i 2>&1; \
	  else \
	    OTCLIB_TRACELEVEL=1 OTCLIB_LOGEXCEPTION=1 \
	    OTCLIB_LOGFILE=$(MK)/$$i.err OTCLIB_NOLOGSTDERR=1 \
	     $(MK)/$$PROG $$TEST > $(MK)/$$i 2>&1; \
	  fi; \
	  STATUS=$$?; \
	  echo; \
	  echo "Exit status: $$STATUS"; \
	  echo; \
	  diff -c $(MK)/$$i $$SRCDIR/$$i; \
	  $(RM) $(MK)/$$i; \
	  echo; \
	  if test -f $(MK)/$$i.err; \
	  then \
	    echo "Messages logged:"; \
	    echo; \
	    cat $(MK)/$$i.err; \
	    $(RM) $(MK)/$$i.err; \
	    echo; \
	  fi; \
	  $(RM) core; \
	done

.PHONY : $(_check_SRC)

## Update output files.

$(_check_SRC) : % :
	@FILE=`basename $@`; \
	PROG=`echo $$FILE | sed -e 's/\.$(_check_out)..*$$//'`; \
	TEST=`echo $$FILE | sed -e 's/^.*\.$(_check_out)//'`; \
	echo "**** Updating file $@ ****"; \
	echo "Running test number $$TEST of $$PROG"; \
	if test -f $(SRCDIR)/$$PROG.$(_check_in)$$TEST; \
	then \
	  echo "Using $$PROG.$(_check_in)$$TEST as input"; \
	  OTCLIB_TRACELEVEL=1 OTCLIB_NOLOGSTDERR=1 \
	   $(MK)/$$PROG $$TEST < $(SRCDIR)/$$PROG.$(_check_in)$$TEST \
	   > $(SRCDIR)/$@ 2>&1; \
	else \
	  OTCLIB_TRACELEVEL=1 OTCLIB_NOLOGSTDERR=1 \
	   $(MK)/$$PROG $$TEST > $(SRCDIR)/$@ 2>&1; \
	fi; \
	echo; \
	$(RM) core

endif
