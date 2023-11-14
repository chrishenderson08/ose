## Copyright 2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## Initialisation.

SRCROOT := ../../..

MODULES := c cxx check

include $(SRCROOT)/setup.mk

## Local definitions.

PROGRAMS += main

EXCLUDE += engine.c

#EXCLUDE += main.c
#EXCLUDE += debug.c
#EXCLUDE += split.c

NONLIBSRC += debug.c
NONLIBSRC += split.c

CPPFLAGS += -I$(SRCROOT)/include

## Makeit modules.

include $(SRCROOT)/modules.mk

## Local rules.

$(MK)/main$(BINEXT) : $(MK)/debug$(OBJEXT) $(MK)/split$(OBJEXT)
