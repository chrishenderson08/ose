## Copyright 2003-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## Initialisation.

SRCROOT := ../..

MAKEIT_OPTIONS += suppress_workarea

include $(SRCROOT)/setup.mk

## Local definitions.

INCDIR := $(includedir)/$(PACKAGE_NAME)-$(PACKAGE_VERSION)/OSE

INCLUDES += OSE.h
INCLUDES += compile.h
INCLUDES += export.h
INCLUDES += system.h
INCLUDES += version.h

ifneq "$(MAKEIT_PLATFORM)" "$(COMPILER_PLATFORM)"
INCLUDES += $(COMPILER_PLATFORM).h
else
INCLUDES += config.h
endif

## Makeit modules.

include $(SRCROOT)/modules.mk

## Local rules.

distclean.always ::
	$(RM) config.h
