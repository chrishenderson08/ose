## Copyright 2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

## Target structure for setup.

.PHONY : makeit.setup

makeit.setup ::

## Target structure for "all".

.PHONY : all
.PHONY : all.setup
.PHONY : all.subdirs
.PHONY : all.target
.PHONY : all.always

all : all.setup
all : all.subdirs
all : all.target
all : all.always

all.setup :: makeit.setup

all.subdirs :: all.setup
	@$(foreach dir,$(SUBDIRS), $(MAKE) -C $(dir) all || $(if \
	  $(findstring k,$(firstword $(filter-out --%,$(MAKEFLAGS)))), \
	  true,exit 1);)

all.target :: all.setup

all.always :: all.setup

## Target structure for "clean" targets.

.PHONY : mostlyclean
.PHONY : mostlyclean.setup
.PHONY : mostlyclean.subdirs
.PHONY : mostlyclean.target
.PHONY : mostlyclean.always

mostlyclean : mostlyclean.setup
mostlyclean : mostlyclean.subdirs
mostlyclean : mostlyclean.target
mostlyclean : mostlyclean.always

mostlyclean.setup :: makeit.setup

mostlyclean.subdirs :: mostlyclean.setup
	@$(foreach dir,$(SUBDIRS), $(MAKE) -C $(dir) mostlyclean || $(if \
	  $(findstring k,$(firstword $(filter-out --%,$(MAKEFLAGS)))), \
	  true,exit 1);)

mostlyclean.target :: mostlyclean.setup

mostlyclean.always :: mostlyclean.setup

.PHONY : clean
.PHONY : clean.setup
.PHONY : clean.subdirs
.PHONY : clean.target
.PHONY : clean.always

clean : clean.setup
clean : clean.subdirs
clean : clean.target
clean : clean.always

clean.setup :: mostlyclean.setup

clean.subdirs :: clean.setup
	@$(foreach dir,$(SUBDIRS), $(MAKE) -C $(dir) clean || $(if \
	  $(findstring k,$(firstword $(filter-out --%,$(MAKEFLAGS)))), \
	  true,exit 1);)

clean.target :: clean.setup

clean.always :: clean.setup mostlyclean.always

.PHONY : distclean
.PHONY : distclean.setup
.PHONY : distclean.subdirs
.PHONY : distclean.target
.PHONY : distclean.always

distclean : distclean.setup
distclean : distclean.subdirs
distclean : distclean.target
distclean : distclean.always

distclean.setup :: clean.setup

distclean.subdirs :: distclean.setup
	@$(foreach dir,$(SUBDIRS), $(MAKE) -C $(dir) distclean || $(if \
	  $(findstring k,$(firstword $(filter-out --%,$(MAKEFLAGS)))), \
	  true,exit 1);)

distclean.target :: distclean.setup

distclean.always :: distclean.setup clean.always

.PHONY : realclean
.PHONY : realclean.setup
.PHONY : realclean.subdirs
.PHONY : realclean.target
.PHONY : realclean.always

realclean : realclean.setup
realclean : realclean.subdirs
realclean : realclean.target
realclean : realclean.always

realclean.setup :: distclean.setup

realclean.subdirs :: realclean.setup
	@$(foreach dir,$(SUBDIRS), $(MAKE) -C $(dir) realclean || $(if \
	  $(findstring k,$(firstword $(filter-out --%,$(MAKEFLAGS)))), \
	  true,exit 1);)

realclean.target :: realclean.setup

realclean.always :: realclean.setup distclean.always

## Target structure for "lib".

.PHONY : lib
.PHONY : lib.setup
.PHONY : lib.subdirs
.PHONY : lib.target
.PHONY : lib.always

lib : lib.setup
# lib : lib.subdirs
lib : lib.target
lib : lib.always

lib.setup :: makeit.setup

lib.subdirs :: lib.setup
	@$(foreach dir,$(SUBDIRS), $(MAKE) -C $(dir) lib || $(if \
	  $(findstring k,$(firstword $(filter-out --%,$(MAKEFLAGS)))), \
	  true,exit 1);)

lib.target :: lib.setup

lib.always :: lib.setup

## Target structure for "pic".

.PHONY : pic
.PHONY : pic.setup
.PHONY : pic.subdirs
.PHONY : pic.target
.PHONY : pic.always

pic : pic.setup
# pic : pic.subdirs
pic : pic.target
pic : pic.always

pic.setup :: makeit.setup

pic.subdirs :: pic.setup
	@$(foreach dir,$(SUBDIRS), $(MAKE) -C $(dir) pic || $(if \
	  $(findstring k,$(firstword $(filter-out --%,$(MAKEFLAGS)))), \
	  true,exit 1);)

pic.target :: pic.setup

pic.always :: pic.setup

## Target structure for "programs".

.PHONY : programs
.PHONY : programs.setup
.PHONY : programs.subdirs
.PHONY : programs.target
.PHONY : programs.always

programs : programs.setup
# programs : programs.subdirs
programs : programs.target
programs : programs.always

programs.setup :: makeit.setup

programs.subdirs :: programs.setup
	@$(foreach dir,$(SUBDIRS), $(MAKE) -C $(dir) programs || $(if \
	  $(findstring k,$(firstword $(filter-out --%,$(MAKEFLAGS)))), \
	  true,exit 1);)

programs.target :: programs.setup

programs.always :: programs.setup

## Target structure for "depend".

.PHONY : depend
.PHONY : depend.setup
.PHONY : depend.subdirs
.PHONY : depend.target
.PHONY : depend.always

depend : depend.setup
depend : depend.subdirs
depend : depend.target
depend : depend.always

depend.setup :: makeit.setup

depend.subdirs :: depend.setup
	@$(foreach dir,$(SUBDIRS), $(MAKE) -C $(dir) depend || $(if \
	  $(findstring k,$(firstword $(filter-out --%,$(MAKEFLAGS)))), \
	  true,exit 1);)

depend.target :: depend.setup

depend.always :: depend.setup

## Target structure for "install".

.PHONY : install
.PHONY : install.setup
.PHONY : install.subdirs
.PHONY : install.target
.PHONY : install.always

install : install.setup
install : install.subdirs
install : install.target
install : install.always

install.setup :: makeit.setup

install.subdirs :: install.setup
	@$(foreach dir,$(SUBDIRS), $(MAKE) -C $(dir) install || $(if \
	  $(findstring k,$(firstword $(filter-out --%,$(MAKEFLAGS)))), \
	  true,exit 1);)

install.target :: install.setup

install.always :: install.setup
