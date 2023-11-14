## Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## Initialisation.

SRCROOT := ../../..

MODULES := c cxx check

include $(SRCROOT)/setup.mk

## Local definitions.

PROGRAMS += _exchange
PROGRAMS += _producer
PROGRAMS += _consumer
PROGRAMS += _iserver
PROGRAMS += _iclient
PROGRAMS += _publish
PROGRAMS += _subscrbe
PROGRAMS += _siclient
PROGRAMS += _siserver
PROGRAMS += _userver
PROGRAMS += _uclient

ifneq "$(filter %-win32,$(MAKEIT_TARGET_PLATFORM))" ""
EXCLUDE += _userver.cc _uclient.cc
endif
ifeq "$(filter %-cywin,$(MAKEIT_TARGET_PLATFORM))" ""
EXCLUDE += _userver.cc _uclient.cc
endif

CPPFLAGS += -I$(SRCROOT)/include

LDLIBS += ../dispatch/$(MK)/lib$(LIBEXT)
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
