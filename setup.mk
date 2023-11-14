## Copyright 2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

# Define empty rules to ensure that "all" is the default target
# and to ensure correct ordering of actions when doing setup.
# There are already appropriate .PHONY target definitions later.

all :

makeit.setup ::

# The yet to be installed version of makeit is to be used. Thus need
# to specify where that resides. This will only work if the SRCROOT
# variable is set correctly in all makefiles to be a relative path
# to the top of the source tree as appropriate for the directory in
# which the makefile resides. Note that we also need to detect if the
# "makeit" script was used to run build. If it is, we remember its
# location and cache that so we know where the module bundle file
# should be installed.

ifneq "$(MAKEIT_HOME)" ""
INSTALLED_MAKEIT_HOME := $(MAKEIT_HOME)
endif

MAKEIT_HOME = $(SRCROOT)/makeit

# Define where the directory this file resides in is supposed to be.

CONFIG_HOME = $(SRCROOT)/config

# The makeit variable defining the identifier for the host platform
# hasn't yet been defined so we need to cheat and work it out here
# so that we can use it to pull in platform specific options.

CONFIG.GUESS := $(MAKEIT_HOME)/scripts/config.guess

PLATFORM_HOST := $(strip $(shell $(SHELL) $(CONFIG.GUESS)))

MAKEIT_PLATFORM_VERSION := \
 $(shell echo $(PLATFORM_HOST) | sed -e 's/^.*[a-zA-Z]\([.0-9]*\)$$/\1/')

MAKEIT_PLATFORM := \
 $(shell echo $(PLATFORM_HOST) | sed -e 's/^\(.*[a-zA-Z]\)[.0-9]*$$/\1/')

# Include the generated config which defines where everything
# will be later installed, plus the package name and version
# assigned to the software.

include $(SRCROOT)/config.mk

# Include the project specific options.

include $(SRCROOT)/options.mk

# Define local modules included with this project.

MAKEIT_MODULE_PATH += $(SRCROOT)/modules
MAKEIT_MODULE_DEPENDENCIES += $(SRCROOT)/modules/ose.d

# Include the standard setup file for makeit.

include $(MAKEIT_HOME)/makeit/setup.mk

# Include the projects specific settings.

include $(SRCROOT)/settings.mk
