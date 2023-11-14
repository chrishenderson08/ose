## Copyright 2003-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

# Enable the OTCLIB_TRACE preprocessor symbol in "dbg" variant.
# This means that the "dbg" variant of the library will have all
# the trace code compiled in and it will be able to be activated
# for a running application. This trace code will slow down any
# application linked against the "dbg" version. If this is a concern
# it should not be defined.

ifeq "$(VARIANT)" "dbg"
PROJECT_CPPFLAGS += -DOTCLIB_TRACE
endif

# Check whether thread support should be compiled into the library.
# If it is, need to define appropriate preprocessor options and
# ensure that the POSIX threads library is linked in.

ifneq "$(filter with_threads,$(OSE_OPTIONS))" ""
PROJECT_CPPFLAGS += -DOSE_WITH_THREADS
PLATFORM_OPTIONS += pthread
endif

# Ensure that any appropriate socket and networking libraries are
# being linked in.

PLATFORM_OPTIONS += socket

# If we are targeting a platform other than the host, need to define
# that as a preprocessor symbol so the correct config file is picked up.

ifneq "$(MAKEIT_PLATFORM)" "$(COMPILER_PLATFORM)"
PROJECT_CPPFLAGS += -DOSE_TARGET_PLATFORM=$(COMPILER_PLATFORM)
endif
