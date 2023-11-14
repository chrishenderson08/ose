## Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## Make "all" the default target.

all :

## Include target structures for builtin targets. These
## need to be included before anything else otherwise
## there can be ordering problems, especially with the
## "clean" targets.

include $(MAKEIT_HOME)/targets/setup.mk

## Determine the platform on which the build is occuring.

CONFIG.GUESS := $(MAKEIT_HOME)/scripts/config.guess

PLATFORM_HOST := $(strip $(shell $(SHELL) $(CONFIG.GUESS)))

MAKEIT_PLATFORM_VERSION := \
 $(shell echo $(PLATFORM_HOST) | sed -e 's/^.*[a-zA-Z]\([.0-9]*\)$$/\1/')

MAKEIT_PLATFORM := \
 $(shell echo $(PLATFORM_HOST) | sed -e 's/^\(.*[a-zA-Z]\)[.0-9]*$$/\1/')

## Search for and include description files for any
## packages of extension modules for makeit.

MAKEIT_PACKAGES += $(wildcard $(MAKEIT_HOME)/modules/*.pkg)

ifneq "$(MAKEIT_PACKAGES)" ""
include $(MAKEIT_PACKAGES)
endif

## Add directory for inbuilt makeit modules as well as
## the name of the corresponding module dependency file.

MAKEIT_MODULE_PATH += $(MAKEIT_HOME)/modules
MAKEIT_MODULE_DEPENDENCIES += $(MAKEIT_HOME)/modules/makeit.d

## Define default filename suffixes and
## library name components. These need to be
## defined in this file as they might be
## needed in the users makefile or any of
## the included module files.

BINEXT :=

ifneq "$(wildcard /usr/bin/ls.exe)" ""
BINEXT := .exe
endif

CMDEXT :=
LIBPRE := lib
LIBEXT := .a
OBJEXT := .o
SHLPRE := lib

ifneq "$(wildcard /lib/libc.so* /usr/lib/libc.so*)" ""
SHLEXT := .so
else
ifneq "$(wildcard /lib/libc.sl /usr/lib/libc.sl)" ""
SHLEXT := .sl
else
ifneq "$(wildcard /lib/libc_s.a /usr/lib/libc_s.a)" ""
SHLEXT := _s.a
else
ifneq "$(wildcard /usr/lib/libc.dylib)" ""
SHLEXT := .dylib
else
SHLEXT := .so
endif
endif
endif
endif

SHMEXT := .so

## Include host config file for this target.

MAKEIT_PLATFORM_DEFAULTS_FILE := \
 $(MAKEIT_PLATFORM)-defaults.mk

-include $(MAKEIT_HOME)/config/$(MAKEIT_PLATFORM_DEFAULTS_FILE)

override MAKEIT_CONFIGURATION := $(strip $(MAKEIT_CONFIGURATION))

ifeq "$(MAKEIT_CONFIGURATION)" ""

override MAKEIT_CONFIGURATION := $(firstword \
 $(patsubst $(MAKEIT_HOME)/config/$(MAKEIT_PLATFORM)-compiler-%.mk,%, \
 $(wildcard $(MAKEIT_HOME)/config/$(MAKEIT_PLATFORM)-compiler-*.mk)))

endif

ifneq "$(MAKEIT_CONFIGURATION)" ""

MAKEIT_COMPILER_SETTINGS_FILE := \
 $(MAKEIT_PLATFORM)-compiler-$(MAKEIT_CONFIGURATION).mk

include $(MAKEIT_HOME)/config/$(MAKEIT_COMPILER_SETTINGS_FILE)

else

ifeq "$(origin CC)" "default"
CC = gcc
endif

ifeq "$(origin CXX)" "default"
CXX = g++
endif

endif

## Set target platform to be the same as the host
## platform if none has been explicitly specified.
## This is not used in makeit itself, but might be
## used by user supplied modules.

override COMPILER_PLATFORM := $(strip $(COMPILER_PLATFORM))

ifeq "$(COMPILER_PLATFORM)" ""
override COMPILER_PLATFORM := $(MAKEIT_PLATFORM)
endif

## Fall back on default for VARIANT if not defined or not in
## set of variants supported by compiler. The compiler must
## always support the "dbg" variant.

ifneq "$(MAKEIT_CONFIGURATION)" ""
override VARIANT := $(strip $(VARIANT))
ifeq "$(filter $(VARIANT),$(COMPILER_VARIANTS))" ""
override VARIANT := dbg
endif
endif

## Name of makeit subdirectory.

ifeq "$(MK)" ""
ifneq "$(MAKEIT_CONFIGURATION)" ""
ifneq "$(filter xc-$(COMPILER_PLATFORM)-%,$(MAKEIT_CONFIGURATION))" ""
override MK := $(MAKEIT_CONFIGURATION)-$(VARIANT)
else
override MK := $(MAKEIT_PLATFORM)-$(MAKEIT_CONFIGURATION)-$(VARIANT)
endif
else
override MK := $(MAKEIT_PLATFORM)
endif
endif

## Calculate SRCDIR, source files etc.

override SRCDIR := $(strip $(SRCDIR))

ifeq "$(SRCDIR)" ""
override SRCDIR := .
endif

## Prevent SRCDIR/SRCDIRS being passed on command line to sub makes.
## While we are doing this, do the same thing for SUBDIRS.

override MAKEOVERRIDES := $(filter-out \
 SUBDIRS=% SRCDIR=% SRCDIRS=%,$(MAKEOVERRIDES))

## Handle specification of additional source directories.

ifneq "$(SRCDIR)" "."
override SRCDIRS := $(foreach dir,$(SRCDIRS),$(dir) \
 $(patsubst %,$(SRCDIR)/%,$(filter-out /%,$(dir))))
endif

override SRCDIRS := $(wildcard $(strip $(SRCDIRS)))

override MAKEOVERRIDES := $(filter-out SRCDIRS=%,$(MAKEOVERRIDES))

## Work out set of source files. If compilation in separate directory, also
## merge in source files in that directory.

override SRCFILES := $(notdir $(wildcard $(SRCDIR)/*))

ifneq "$(SRCDIR)" "."
override SRCFILES += $(wildcard *)
endif

ifneq "$(SRCDIR)" "."
override SRCFILES += $(wildcard *)
endif

ifneq "$(SRCDIRS)" ""
override SRCFILES += $(notdir $(wildcard $(addsuffix /*,$(SRCDIRS))))
endif

override SRCFILES := $(sort $(SRCFILES))

## Check for filenames containing characters which can give us problems.
## Eliminate them from the list of files and give a diagnostic warning.

_makeit_illegal_files :=
_makeit_illegal_files += $(filter-out $(subst %,,$(SRCFILES)),$(SRCFILES))
_makeit_illegal_files += $(filter-out $(subst \#,,$(SRCFILES)),$(SRCFILES))
_makeit_illegal_files += $(filter-out $(subst :,,$(SRCFILES)),$(SRCFILES))
_makeit_illegal_files := $(sort $(_makeit_illegal_files))

ifneq "$(_makeit_illegal_files)" ""

override SRCFILES := $(filter $(subst %,,$(SRCFILES)),$(SRCFILES))
override SRCFILES := $(filter $(subst \#,,$(SRCFILES)),$(SRCFILES))
override SRCFILES := $(filter $(subst :,,$(SRCFILES)),$(SRCFILES))

ifneq "$(filter filename_warnings,$(MAKEIT_OPTIONS))" ""
makeit.setup ::
	@$(foreach file,$(_makeit_illegal_files), \
	 echo "makeit: illegal characters, ignoring '$(file)'";)
endif

endif
