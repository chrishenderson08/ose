## Copyright 1995-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

# Include modules.

_makeit_module_list := \
 $(shell cat $(wildcard $(MAKEIT_MODULE_DEPENDENCIES)) | tsort)

MAKEIT_MODULES += $(_makeit_module_list)

_makeit_modules := \
 $(foreach module,$(filter $(MODULES),$(MAKEIT_MODULES)),\
 $(firstword $(wildcard $(addsuffix /$(module).mk,$(MAKEIT_MODULE_PATH))) \
 missing/module/$(module))) $(addprefix invalid/module/,\
 $(filter-out $(MAKEIT_MODULES),$(MODULES)))

include $(_makeit_modules)

# Include rules associated with builtin targets.

include $(MAKEIT_HOME)/targets/lib.mk

include $(MAKEIT_HOME)/targets/pic.mk

include $(MAKEIT_HOME)/targets/programs.mk

include $(MAKEIT_HOME)/targets/depend.mk

include $(MAKEIT_HOME)/targets/install.mk

# Include extensions to targets for platform and compiler.

ifneq "$(MAKEIT_CONFIGURATION))" ""

override MAKEIT_PLATFORM_SETTINGS_FILE := \
 $(MAKEIT_PLATFORM)-platform-$(MAKEIT_CONFIGURATION).mk

-include $(MAKEIT_HOME)/config/$(MAKEIT_PLATFORM_SETTINGS_FILE)

override MAKEIT_TARGETS_FILE := \
 $(MAKEIT_PLATFORM)-targets-$(MAKEIT_CONFIGURATION).mk

-include $(MAKEIT_HOME)/config/$(MAKEIT_TARGETS_FILE)

endif

# Print diagnostics about modules which were required but not included.

_makeit_missing_modules := \
 $(sort $(filter-out $(MODULES),$(MAKEIT_REQUIRED_MODULES)))

ifneq "$(_makeit_missing_modules)" ""

makeit.setup ::
	@$(foreach module,$(_makeit_missing_modules), \
	 echo "makeit: module was not listed, require '$(module)'";)
	exit 1

endif
