## Copyright 2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## Initialisation.

SRCROOT := ../../..

MODULES := c cxx check

include $(SRCROOT)/setup.mk

## Local definitions.

EXCLUDE += fnv%.c
EXCLUDE += have_ulong64.c

CPPFLAGS += -I$(SRCROOT)/include

## Makeit modules.

include $(SRCROOT)/modules.mk

## Local rules.
