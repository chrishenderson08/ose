## Copyright 2003-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## Initialisation.

SRCROOT := ../../..

MODULES := c cxx check

include $(SRCROOT)/setup.mk

## Local definitions.

PROGRAMS += _agent
PROGRAMS += _monitor
PROGRAMS += _service
PROGRAMS += _callback

CPPFLAGS += -I$(SRCROOT)/include

LDLIBS += ../../OTC/internet/$(MK)/lib$(LIBEXT)
LDLIBS += ../../OTC/message/$(MK)/lib$(LIBEXT)
LDLIBS += ../../OTC/dispatch/$(MK)/lib$(LIBEXT)
LDLIBS += ../../OTC/system/$(MK)/lib$(LIBEXT)
LDLIBS += ../../OTC/types/$(MK)/lib$(LIBEXT)
LDLIBS += ../../OTC/text/$(MK)/lib$(LIBEXT)
LDLIBS += ../../OTC/collctn/$(MK)/lib$(LIBEXT)
LDLIBS += ../../OTC/refcnt/$(MK)/lib$(LIBEXT)
LDLIBS += ../../OTC/memory/$(MK)/lib$(LIBEXT)
LDLIBS += ../../OTC/debug/$(MK)/lib$(LIBEXT)
LDLIBS += ../../OTC/misc/$(MK)/lib$(LIBEXT)
LDLIBS += ../../OTC/thread/$(MK)/lib$(LIBEXT)
LDLIBS += ../../OTC/hash/$(MK)/lib$(LIBEXT)

## Makeit modules.

include $(SRCROOT)/modules.mk

## Local rules.
