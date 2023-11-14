## Copyright 2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## Initialisation.

SRCROOT := ../..

MAKEIT_OPTIONS += suppress_workarea

include $(SRCROOT)/setup.mk

## Local definitions.

SUBDIRS += agent

INCDIR := $(includedir)/$(PACKAGE_NAME)-$(PACKAGE_VERSION)/ONS

INCLUDES := $(filter %.h %.hh,$(SRCFILES))

## Makeit modules.

include $(SRCROOT)/modules.mk

## Local targets/rules.
