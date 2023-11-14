## Copyright 2003-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## Initialisation.

SRCROOT := ../..

MAKEIT_OPTIONS += suppress_workarea

include $(SRCROOT)/setup.mk

## Local definitions.

SUBDIRS += stdlib
SUBDIRS += misc
SUBDIRS += thread
SUBDIRS += debug
SUBDIRS += memory
SUBDIRS += refcnt
SUBDIRS += collctn
SUBDIRS += text
SUBDIRS += types
SUBDIRS += system
SUBDIRS += dispatch
SUBDIRS += message
SUBDIRS += internet

INCDIR := $(includedir)/$(PACKAGE_NAME)-$(PACKAGE_VERSION)/OTC

INCLUDES := $(filter %.h %.hh,$(SRCFILES))

## Makeit modules.

include $(SRCROOT)/modules.mk

## Local targets/rules.
