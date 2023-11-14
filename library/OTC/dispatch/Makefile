## Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## Initialisation.

SRCROOT := ../../..

MODULES := c cxx check

include $(SRCROOT)/setup.mk

## Local definitions.

PROGRAMS += _dispatch
PROGRAMS += _signal
PROGRAMS += _envelope
PROGRAMS += _action
PROGRAMS += _shutdown
PROGRAMS += _subject
PROGRAMS += _svbroker
PROGRAMS += _svpyload

CPPFLAGS += -I$(SRCROOT)/include

LDLIBS += ../system/$(MK)/lib$(LIBEXT)
LDLIBS += ../types/$(MK)/lib$(LIBEXT)
LDLIBS += ../text/$(MK)/lib$(LIBEXT)
LDLIBS += ../collctn/$(MK)/lib$(LIBEXT)
LDLIBS += ../refcnt/$(MK)/lib$(LIBEXT)
LDLIBS += ../memory/$(MK)/lib$(LIBEXT)
LDLIBS += ../debug/$(MK)/lib$(LIBEXT)
LDLIBS += ../misc/$(MK)/lib$(LIBEXT)
LDLIBS += ../thread/$(MK)/lib$(LIBEXT)
LDLIBS += ../regex/$(MK)/lib$(LIBEXT)
LDLIBS += ../hash/$(MK)/lib$(LIBEXT)

## Makeit modules.

include $(SRCROOT)/modules.mk

## Local rules.
