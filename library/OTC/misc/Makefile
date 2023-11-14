## Copyright 1997-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## Initialisation.

SRCROOT := ../../..

MODULES := c cxx check

include $(SRCROOT)/setup.mk

## Local definitions.

PROGRAMS += _funcstrm
PROGRAMS += _vfuncbuf

EXCLUDE += fnv%.c
EXCLUDE += have_ulong64.c

CPPFLAGS += -I$(SRCROOT)/include

LDLIBS += ../regex/$(MK)/lib$(LIBEXT)
LDLIBS += ../hash/$(MK)/lib$(LIBEXT)

## Makeit modules.

include $(SRCROOT)/modules.mk

## Local rules.
