## Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## Initialisation.

SRCROOT := ../../..

MODULES := c cxx check

include $(SRCROOT)/setup.mk

## Local definitions.

PROGRAMS += _string
PROGRAMS += _record
PROGRAMS += _globex
PROGRAMS += _regex
PROGRAMS += _regexp
PROGRAMS += _symbol
PROGRAMS += _tracepat
PROGRAMS += _options
PROGRAMS += _vrecord
PROGRAMS += _slogger
PROGRAMS += _prprties
PROGRAMS += _osstream
PROGRAMS += _domstrng
PROGRAMS += _domnode
PROGRAMS += _pattern

CPPFLAGS += -I$(SRCROOT)/include

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
