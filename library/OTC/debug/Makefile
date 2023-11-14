## Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## Initialisation.

SRCROOT := ../../..

MODULES := c cxx check

include $(SRCROOT)/setup.mk

## Local definitions.

PROGRAMS += _assert
PROGRAMS += _ensure
PROGRAMS += _errors
PROGRAMS += _tracer
#PROGRAMS += _tobject
PROGRAMS += _logger
PROGRAMS += _warning
PROGRAMS += _logstrm
PROGRAMS += _tracetag

EXCLUDE += _tobject.cc

CPPFLAGS += -I$(SRCROOT)/include

LDLIBS += ../misc/$(MK)/lib$(LIBEXT)
LDLIBS += ../thread/$(MK)/lib$(LIBEXT)
LDLIBS += ../regex/$(MK)/lib$(LIBEXT)
LDLIBS += ../hash/$(MK)/lib$(LIBEXT)

## Makeit modules.

include $(SRCROOT)/modules.mk

## Local rules.
