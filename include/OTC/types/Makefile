## Copyright 2003-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## Initialisation.

SRCROOT := ../../..

MAKEIT_OPTIONS += suppress_workarea

include $(SRCROOT)/setup.mk

## Local definitions.

INCDIR := $(includedir)/$(PACKAGE_NAME)-$(PACKAGE_VERSION)/OTC/types
INCLUDES := $(filter %.h %.hh %.i %.c,$(SRCFILES))

## Makeit modules.

include $(SRCROOT)/modules.mk
