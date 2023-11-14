## Copyright 2003-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## Initialisation.

SRCROOT := ..

MODULES := c cxx

PIC_OPTIONS += shared_library

SRCDIRS += OTC/hash
SRCDIRS += OTC/regex
SRCDIRS += OTC/thread
SRCDIRS += OTC/misc
SRCDIRS += OTC/debug
SRCDIRS += OTC/memory
SRCDIRS += OTC/refcnt
SRCDIRS += OTC/collctn
SRCDIRS += OTC/text
SRCDIRS += OTC/types
SRCDIRS += OTC/system
SRCDIRS += OTC/dispatch
SRCDIRS += OTC/message
SRCDIRS += OTC/internet

ifeq "$(filter vc6.% vc7.0,$(MAKEIT_CONFIGURATION))" ""
SRCDIRS += ONS/agent
endif

include $(SRCROOT)/setup.mk

## Local definitions.

EXCLUDE += $(filter x%.cc _%.cc,$(SRCFILES))

EXCLUDE += fnv%.c
EXCLUDE += have_ulong64.c

EXCLUDE += debug.c
EXCLUDE += engine.c
EXCLUDE += main.c
EXCLUDE += split.c

CPPFLAGS += -I$(SRCROOT)/include

# Note that shared library version numbers do not
# track package version numbers but are incremented
# independently. Prior to version 8.0 of OSE, the
# version number was always encoded explicitly into
# the name of the library. Proper operating system
# shared library versioning support is now used.
# Rather than start at 1.0, versioning has started
# at 2.0 with version 8.0 of OSE.

LIBVERSION = 2.0

ifneq "$(filter with_threads,$(OSE_OPTIONS))" ""
ifneq "$(VARIANT)" ""
LIBNAME = $(PACKAGE_NAME)_mt_$(VARIANT)
else
LIBNAME = $(PACKAGE_NAME)_mt
endif
else
ifneq "$(VARIANT)" ""
LIBNAME = $(PACKAGE_NAME)_$(VARIANT)
else
LIBNAME = $(PACKAGE_NAME)
endif
endif

ifneq "$(MAKEIT_CONFIGURATION)" ""
LIBDIR = $(libdir)/$(PACKAGE_NAME)-$(PACKAGE_VERSION)/$(MAKEIT_CONFIGURATION)
else
LIBDIR = $(libdir)/$(PACKAGE_NAME)-$(PACKAGE_VERSION)
endif

## Makeit modules.

include $(SRCROOT)/modules.mk

## Local rules.

distclean.always ::
	@$(foreach dir,$(SRCDIRS), $(MAKE) -C $(dir) distclean || $(if \
	  $(findstring k,$(firstword $(filter-out --%,$(MAKEFLAGS)))), \
	  true,exit 1);)
