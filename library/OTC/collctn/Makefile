## Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## Initialisation.

SRCROOT := ../../..

MODULES := c cxx check

include $(SRCROOT)/setup.mk

## Local definitions.

PROGRAMS += _list
PROGRAMS += _linklist
PROGRAMS += _stack
PROGRAMS += _queue
PROGRAMS += _hset
PROGRAMS += _hmap
PROGRAMS += _huniqmap
PROGRAMS += _hbag
PROGRAMS += _deque
PROGRAMS += _bitset
PROGRAMS += _ordlist
PROGRAMS += _priqueue
PROGRAMS += _bndstack
PROGRAMS += _bndqueue
PROGRAMS += _range
PROGRAMS += _array
PROGRAMS += _tbag
PROGRAMS += _tmanymap
PROGRAMS += _ilist
PROGRAMS += _single
PROGRAMS += _pair
PROGRAMS += _triple
PROGRAMS += _icursor
PROGRAMS += _visitor
PROGRAMS += _worker
PROGRAMS += _prworker
PROGRAMS += _itmap
PROGRAMS += _prvisitr
PROGRAMS += _mlist
PROGRAMS += _ihmap
PROGRAMS += _hindex
PROGRAMS += _tindex
PROGRAMS += _tuniqmap
PROGRAMS += _tset
PROGRAMS += _tmap
PROGRAMS += _tdualmap

NONLIBSRC := $(filter xxx%.cc,$(SRCFILES))

CPPFLAGS += -I$(SRCROOT)/include

LDLIBS += $(MK)/xxxrandom$(OBJEXT)

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
