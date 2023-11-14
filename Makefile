## Copyright 2003-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
##
## Note: This Makefile requires GNU make 3.78 or later.
## You must have run "configure" before you can run make.

## Initialisation.

SRCROOT := .

MAKEIT_OPTIONS += suppress_workarea

include $(SRCROOT)/setup.mk

## Local definitions.

ifeq "$(VARIANTS)" ""
VARIANTS = $(COMPILER_VARIANTS)
endif

INSTALLDIRS += $(libdir)/$(PACKAGE_NAME)-$(PACKAGE_VERSION)/modules

AUXDIR1 = $(libdir)/$(PACKAGE_NAME)-$(PACKAGE_VERSION)
AUXILIARIES1 += $(wildcard modules/*.mk modules/*.d)

## Makeit modules.

include $(SRCROOT)/modules.mk

## Local rules.

all.always ::
ifneq "$(VARIANT)" ""
	@$(foreach var,$(VARIANTS), $(MAKE) -C library \
	 VARIANT=$(var) all || $(if $(findstring k,$(firstword \
	 $(filter-out --%,$(MAKEFLAGS)))),true,exit 1);)
else
	$(MAKE) -C library all
endif

install.always :: install.setup
	$(MAKE) -C include install
ifneq "$(VARIANT)" ""
	@$(foreach var,$(VARIANTS), $(MAKE) -C library \
	 VARIANT=$(var) install || $(if $(findstring k,$(firstword \
	 $(filter-out --%,$(MAKEFLAGS)))),true,exit 1);)
else
	$(MAKE) -C library install
endif

ifneq "$(INSTALLED_MAKEIT_HOME)" ""

PACKAGE_HOME = $(INSTALLED_MAKEIT_HOME)/modules

install.always :: install-modules

install-modules : $(PACKAGE_HOME)/ose.pkg

$(PACKAGE_HOME)/ose.pkg : modules/ose.pkg
	$(INSTALL.AUX) $< $@

endif

mostlyclean.target ::
ifneq "$(VARIANT)" ""
	@$(foreach var,$(VARIANTS), $(MAKE) -C library \
	 VARIANT=$(var) mostlyclean || $(if $(findstring k,$(firstword \
	 $(filter-out --%,$(MAKEFLAGS)))),true,exit 1);)
else
	$(MAKE) -C library mostlyclean
endif

clean.target ::
	-$(MAKE) -C makeit clean
	$(MAKE) -C include clean
ifneq "$(VARIANT)" ""
	@$(foreach var,$(VARIANTS), $(MAKE) -C library \
	 VARIANT=$(var) clean || $(if $(findstring k,$(firstword \
	 $(filter-out --%,$(MAKEFLAGS)))),true,exit 1);)
else
	$(MAKE) -C library clean
endif

clean.always ::
	rm -rf autom4te.cache

distclean.target ::
	-$(MAKE) -C makeit distclean
	$(MAKE) -C include distclean
ifneq "$(VARIANT)" ""
	@$(foreach var,$(VARIANTS), $(MAKE) -C library \
	 VARIANT=$(var) distclean || $(if $(findstring k,$(firstword \
	 $(filter-out --%,$(MAKEFLAGS)))),true,exit 1);)
else
	$(MAKE) -C library distclean
endif

distclean.always ::
	$(RM) modules/ose.pkg modules/ose.mk
	$(RM) config.log config.status
	$(RM) config.mk

realclean.target ::
	-$(MAKE) -C makeit realclean
	$(MAKE) -C include realclean
ifneq "$(VARIANT)" ""
	@$(foreach var,$(VARIANTS), $(MAKE) -C library \
	 VARIANT=$(var) realclean || $(if $(findstring k,$(firstword \
	 $(filter-out --%,$(MAKEFLAGS)))),true,exit 1);)
else
	$(MAKE) -C library realclean
endif

realclean.always ::
	$(RM) configure
