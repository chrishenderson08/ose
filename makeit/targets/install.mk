## Copyright 1992 OTC LIMITED
## Copyright 1995-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

vpath %.c $(SRCDIR) $(SRCDIRS)
vpath %.c++ $(SRCDIR) $(SRCDIRS)
vpath %.cxx $(SRCDIR) $(SRCDIRS)
vpath %.cpp $(SRCDIR) $(SRCDIRS)
vpath %.cc $(SRCDIR) $(SRCDIRS)
vpath %.C $(SRCDIR) $(SRCDIRS)

vpath %.h $(SRCDIR) $(SRCDIRS)
vpath %.h++ $(SRCDIR) $(SRCDIRS)
vpath %.hxx $(SRCDIR) $(SRCDIRS)
vpath %.hpp $(SRCDIR) $(SRCDIRS)
vpath %.hh $(SRCDIR) $(SRCDIRS)
vpath %.H $(SRCDIR) $(SRCDIRS)

vpath %.mk $(SRCDIR) $(SRCDIRS)
vpath %.sed $(SRCDIR) $(SRCDIRS)

vpath %.1 $(SRCDIR) $(SRCDIRS)
vpath %.2 $(SRCDIR) $(SRCDIRS)
vpath %.3 $(SRCDIR) $(SRCDIRS)
vpath %.4 $(SRCDIR) $(SRCDIRS)
vpath %.5 $(SRCDIR) $(SRCDIRS)
vpath %.6 $(SRCDIR) $(SRCDIRS)
vpath %.7 $(SRCDIR) $(SRCDIRS)
vpath %.8 $(SRCDIR) $(SRCDIRS)

vpath mandesc $(SRCDIR) $(SRCDIRS)

## Separate "install" target into sub targets.

install.always :: install-lib.setup install-lib.always
install.always :: install-pic.setup install-pic.always
install.always :: install-bin.setup install-bin.always
install.always :: install-inc.setup install-inc.always
install.always :: install-man.setup install-man.always
install.always :: install-aux.setup install-aux.always

.PHONY : install-lib
.PHONY : install-pic
.PHONY : install-bin
.PHONY : install-inc
.PHONY : install-man
.PHONY : install-aux

install-lib : install-lib.setup
install-lib : install-lib.subdirs
install-lib : install-lib.target
install-lib : install-lib.always

install-pic : install-pic.setup
install-pic : install-pic.subdirs
install-pic : install-pic.target
install-pic : install-pic.always

install-bin : install-bin.setup
install-bin : install-bin.subdirs
install-bin : install-bin.target
install-bin : install-bin.always

install-inc : install-inc.setup
install-inc : install-inc.subdirs
install-inc : install-inc.target
install-inc : install-inc.always

install-man : install-man.setup
install-man : install-man.subdirs
install-man : install-man.target
install-man : install-man.always

install-aux : install-aux.setup
install-aux : install-aux.subdirs
install-aux : install-aux.target
install-aux : install-aux.always

.PHONY : install-lib.setup
.PHONY : install-pic.setup
.PHONY : install-bin.setup
.PHONY : install-inc.setup
.PHONY : install-man.setup
.PHONY : install-aux.setup

install-lib.setup :: install.setup
install-pic.setup :: install.setup
install-bin.setup :: install.setup
install-inc.setup :: install.setup
install-man.setup :: install.setup
install-aux.setup :: install.setup

.PHONY : install-lib.subdirs
.PHONY : install-pic.subdirs
.PHONY : install-bin.subdirs
.PHONY : install-inc.subdirs
.PHONY : install-man.subdirs
.PHONY : install-aux.subdirs

install-lib.subdirs :: install-lib.setup
	@$(foreach dir,$(SUBDIRS), $(MAKE) -C $(dir) install-lib || $(if \
	  $(findstring k,$(firstword $(filter-out --%,$(MAKEFLAGS)))), \
	  true,exit 1);)

install-pic.subdirs :: install-pic.setup
	@$(foreach dir,$(SUBDIRS), $(MAKE) -C $(dir) install-pic || $(if \
	  $(findstring k,$(firstword $(filter-out --%,$(MAKEFLAGS)))), \
	  true,exit 1);)

install-bin.subdirs :: install-bin.setup
	@$(foreach dir,$(SUBDIRS), $(MAKE) -C $(dir) install-bin || $(if \
	  $(findstring k,$(firstword $(filter-out --%,$(MAKEFLAGS)))), \
	  true,exit 1);)

install-inc.subdirs :: install-inc.setup
	@$(foreach dir,$(SUBDIRS), $(MAKE) -C $(dir) install-inc || $(if \
	  $(findstring k,$(firstword $(filter-out --%,$(MAKEFLAGS)))), \
	  true,exit 1);)

install-man.subdirs :: install-man.setup
	@$(foreach dir,$(SUBDIRS), $(MAKE) -C $(dir) install-man || $(if \
	  $(findstring k,$(firstword $(filter-out --%,$(MAKEFLAGS)))), \
	  true,exit 1);)

install-aux.subdirs :: install-aux.setup
	@$(foreach dir,$(SUBDIRS), $(MAKE) -C $(dir) install-aux || $(if \
	  $(findstring k,$(firstword $(filter-out --%,$(MAKEFLAGS)))), \
	  true,exit 1);)

.PHONY : install-lib.target
.PHONY : install-pic.target
.PHONY : install-bin.target
.PHONY : install-inc.target
.PHONY : install-man.target
.PHONY : install-aux.target

install-lib.target ::
install-pic.target ::
install-bin.target ::
install-inc.target ::
install-man.target ::
install-aux.target ::

.PHONY : install-lib.always
.PHONY : install-pic.always
.PHONY : install-bin.always
.PHONY : install-inc.always
.PHONY : install-man.always
.PHONY : install-aux.always

install-lib.always ::
install-pic.always ::
install-bin.always ::
install-inc.always ::
install-man.always ::
install-aux.always ::

## Setup some definitions.

override INSTALL := $(SHELL) $(MAKEIT_HOME)/scripts/install-sh -c
override MKINSTALLDIRS := $(SHELL) $(MAKEIT_HOME)/scripts/mkinstalldirs

ifeq "$(origin INSTALL.LIBFLAGS)" "undefined"
INSTALL.LIBFLAGS := -m 0664
endif
ifeq "$(origin INSTALL.PICFLAGS)" "undefined"
INSTALL.PICFLAGS := -m 0775
endif
ifeq "$(origin INSTALL.BINFLAGS)" "undefined"
INSTALL.BINFLAGS := -m 0775
endif
ifeq "$(origin INSTALL.INCFLAGS)" "undefined"
INSTALL.INCFLAGS := -m 0444
endif
ifeq "$(origin INSTALL.MANFLAGS)" "undefined"
INSTALL.MANFLAGS := -m 0444
endif
ifeq "$(origin INSTALL.AUXFLAGS)" "undefined"
INSTALL.AUXFLAGS := -m 0444
endif

INSTALL.LIB := $(INSTALL) $(INSTALL.LIBFLAGS)
INSTALL.PIC := $(INSTALL) $(INSTALL.PICFLAGS)
INSTALL.BIN := $(INSTALL) $(INSTALL.BINFLAGS)
INSTALL.INC := $(INSTALL) $(INSTALL.INCFLAGS)
INSTALL.MAN := $(INSTALL) $(INSTALL.MANFLAGS)
INSTALL.AUX := $(INSTALL) $(INSTALL.AUXFLAGS)

ifeq "$(STRIP)" ""
override STRIP := strip
endif

## Installation for libraries.

# Installation rules for libraries and loadable
# modules are in lib.mk and pic.mk respectively
# because of the closer binding required to the
# rules which generate them.

## Installation for executables.

ifeq "$(BINDIR1)" ""
override BINDIR1 := $(BINDIR)
ifneq "$(origin EXECUTABLES)" "undefined"
override EXECUTABLES1 := $(EXECUTABLES)
endif
endif

ifneq "$(BINDIR1)" ""

_install_directories += $(BINDIR1)

_install_BINARY_EXECUTABLES1 := $(notdir $(_makeit_MK_BINARIES))
_install_SCRIPT_EXECUTABLES1 := $(notdir $(_makeit_MK_SCRIPTS))

ifneq "$(origin EXECUTABLES1)" "undefined"

_install_BINARY_EXECUTABLES1 := \
 $(filter $(EXECUTABLES1),$(_install_BINARY_EXECUTABLES1))

_install_SCRIPT_EXECUTABLES1 := \
 $(filter-out $(_install_BINARY_EXECUTABLES1),$(EXECUTABLES1))

endif

ifneq "$(_install_BINARY_EXECUTABLES1)" ""

_install_BINDIR_BINARY_EXECUTABLES1 := \
 $(patsubst %,$(BINDIR1)/$(BINPREFIX1)%$(BINSUFFIX1), \
 $(_install_BINARY_EXECUTABLES1))

install-bin.always :: $(_install_BINDIR_BINARY_EXECUTABLES1)

$(_install_BINDIR_BINARY_EXECUTABLES1) : \
 $(BINDIR1)/$(BINPREFIX1)%$(BINSUFFIX1) : $(MK)/%
	$(INSTALL.BIN) $< $@
ifneq "$(filter strip_binaries,$(INSTALL_OPTIONS))" ""
	$(STRIP) $@
endif

endif

ifneq "$(_install_SCRIPT_EXECUTABLES1)" ""

_install_BINDIR_SCRIPT_EXECUTABLES1 := \
 $(patsubst %,$(BINDIR1)/$(BINPREFIX1)%$(BINSUFFIX1), \
 $(_install_SCRIPT_EXECUTABLES1))

install-bin.always :: $(_install_BINDIR_SCRIPT_EXECUTABLES1)

$(_install_BINDIR_SCRIPT_EXECUTABLES1) : \
 $(BINDIR1)/$(BINPREFIX1)%$(BINSUFFIX1) : $(MK)/%
	$(INSTALL.BIN) $< $@

endif

endif

ifneq "$(BINDIR2)" ""

_install_directories += $(BINDIR2)

ifneq "$(EXECUTABLES2)" ""

_install_BINARY_EXECUTABLES2 := \
 $(filter $(EXECUTABLES2),$(notdir $(_makeit_MK_BINARIES)))

_install_SCRIPT_EXECUTABLES2 := \
 $(filter-out $(_install_BINARY_EXECUTABLES2),$(EXECUTABLES2))

endif

ifneq "$(_install_BINARY_EXECUTABLES2)" ""

_install_BINDIR_BINARY_EXECUTABLES2 := \
 $(patsubst %,$(BINDIR2)/$(BINPREFIX2)%$(BINSUFFIX2), \
 $(_install_BINARY_EXECUTABLES2))

install-bin.always :: $(_install_BINDIR_BINARY_EXECUTABLES2)

$(_install_BINDIR_BINARY_EXECUTABLES2) : \
 $(BINDIR2)/$(BINPREFIX2)%$(BINSUFFIX2) : $(MK)/%
	$(INSTALL.BIN) $< $@
ifneq "$(filter strip_binaries,$(INSTALL_OPTIONS))" ""
	$(STRIP) $@
endif

endif

ifneq "$(_install_SCRIPT_EXECUTABLES2)" ""

_install_BINDIR_SCRIPT_EXECUTABLES2 := \
 $(patsubst %,$(BINDIR2)/$(BINPREFIX2)%$(BINSUFFIX2), \
 $(_install_SCRIPT_EXECUTABLES2))

install-bin.always :: $(_install_BINDIR_SCRIPT_EXECUTABLES2)

$(_install_BINDIR_SCRIPT_EXECUTABLES2) : \
 $(BINDIR2)/$(BINPREFIX2)%$(BINSUFFIX2) : $(MK)/%
	$(INSTALL.BIN) $< $@

endif

endif

ifneq "$(BINDIR3)" ""

_install_directories += $(BINDIR3)

ifneq "$(EXECUTABLES3)" ""

_install_BINARY_EXECUTABLES3 := \
 $(filter $(EXECUTABLES3),$(notdir $(_makeit_MK_BINARIES)))

_install_SCRIPT_EXECUTABLES3 := \
 $(filter-out $(_install_BINARY_EXECUTABLES3),$(EXECUTABLES3))

endif

ifneq "$(_install_BINARY_EXECUTABLES3)" ""

_install_BINDIR_BINARY_EXECUTABLES3 := \
 $(patsubst %,$(BINDIR3)/$(BINPREFIX3)%$(BINSUFFIX3), \
 $(_install_BINARY_EXECUTABLES3))

install-bin.always :: $(_install_BINDIR_BINARY_EXECUTABLES3)

$(_install_BINDIR_BINARY_EXECUTABLES3) : \
 $(BINDIR3)/$(BINPREFIX3)%$(BINSUFFIX3) : $(MK)/%
	$(INSTALL.BIN) $< $@
ifneq "$(filter strip_binaries,$(INSTALL_OPTIONS))" ""
	$(STRIP) $@
endif

endif

ifneq "$(_install_SCRIPT_EXECUTABLES3)" ""

_install_BINDIR_SCRIPT_EXECUTABLES3 := \
 $(patsubst %,$(BINDIR3)/$(BINPREFIX3)%$(BINSUFFIX3), \
 $(_install_SCRIPT_EXECUTABLES3))

install-bin.always :: $(_install_BINDIR_SCRIPT_EXECUTABLES3)

$(_install_BINDIR_SCRIPT_EXECUTABLES3) : \
 $(BINDIR3)/$(BINPREFIX3)%$(BINSUFFIX3) : $(MK)/%
	$(INSTALL.BIN) $< $@

endif

endif

ifneq "$(BINDIR4)" ""

_install_directories += $(BINDIR4)

ifneq "$(EXECUTABLES4)" ""

_install_BINARY_EXECUTABLES4 := \
 $(filter $(EXECUTABLES4),$(notdir $(_makeit_MK_BINARIES)))

_install_SCRIPT_EXECUTABLES4 := \
 $(filter-out $(_install_BINARY_EXECUTABLES4),$(EXECUTABLES4))

endif

ifneq "$(_install_BINARY_EXECUTABLES4)" ""

_install_BINDIR_BINARY_EXECUTABLES4 := \
 $(patsubst %,$(BINDIR4)/$(BINPREFIX4)%$(BINSUFFIX4), \
 $(_install_BINARY_EXECUTABLES4))

install-bin.always :: $(_install_BINDIR_BINARY_EXECUTABLES4)

$(_install_BINDIR_BINARY_EXECUTABLES4) : \
 $(BINDIR4)/$(BINPREFIX4)%$(BINSUFFIX4) : $(MK)/%
	$(INSTALL.BIN) $< $@
ifneq "$(filter strip_binaries,$(INSTALL_OPTIONS))" ""
	$(STRIP) $@
endif

endif

ifneq "$(_install_SCRIPT_EXECUTABLES4)" ""

_install_BINDIR_SCRIPT_EXECUTABLES4 := \
 $(patsubst %,$(BINDIR4)/$(BINPREFIX4)%$(BINSUFFIX4), \
 $(_install_SCRIPT_EXECUTABLES4))

install-bin.always :: $(_install_BINDIR_SCRIPT_EXECUTABLES4)

$(_install_BINDIR_SCRIPT_EXECUTABLES4) : \
 $(BINDIR4)/$(BINPREFIX4)%$(BINSUFFIX4) : $(MK)/%
	$(INSTALL.BIN) $< $@

endif

endif

## Installation for header files.

ifneq "$(INCDIR)" ""

_install_directories += $(INCDIR)

ifeq "$(origin INCLUDES)" "undefined"
_install_INCLUDES := $(filter-out $(EXCLUDE), \
 $(filter %.h %.hh %.hpp %.hxx %.H %.h++,$(SRCFILES)))
else
_install_INCLUDES := $(INCLUDES)
endif

ifneq "$(_install_INCLUDES)" ""

_install_INCDIR_INCLUDES := $(addprefix $(INCDIR)/,$(_install_INCLUDES))

install-inc.always :: $(_install_INCDIR_INCLUDES)

$(_install_INCDIR_INCLUDES) : $(INCDIR)/% : %
	$(INSTALL.INC) $< $@

endif

endif

## Installation for manual pages.

ifneq "$(MANDIR)" ""

_install_directories += $(MANDIR)

ifeq "$(origin MANDESC)" "undefined"
_install_MANDESC := $(filter mandesc,$(SRCFILES))
else
_install_MANDESC := $(MANDESC)
endif

ifneq "$(_install_MANDESC)" ""

install-man.always :: $(addprefix $(MANDIR)/,mandesc)

$(addprefix $(MANDIR)/,mandesc) : $(_install_MANDESC)
	$(INSTALL.MAN) $< $@

endif

ifneq "$(SECTION1)" ""

_install_directories += $(MANDIR)/man1

_install_MANDIR_SECTION1 := $(addprefix $(MANDIR)/man1/,$(SECTION1))

install-man.always :: $(_install_MANDIR_SECTION1)

$(_install_MANDIR_SECTION1) : $(MANDIR)/man1/% : %
	$(INSTALL.MAN) $< $@

endif

ifneq "$(SECTION2)" ""

_install_directories += $(MANDIR)/man2

_install_MANDIR_SECTION2 := $(addprefix $(MANDIR)/man2/,$(SECTION2))

install-man.always :: $(_install_MANDIR_SECTION2)

$(_install_MANDIR_SECTION2) : $(MANDIR)/man2/% : %
	$(INSTALL.MAN) $< $@

endif

ifneq "$(SECTION3)" ""

_install_directories += $(MANDIR)/man3

_install_MANDIR_SECTION3 := $(addprefix $(MANDIR)/man3/,$(SECTION3))

install-man.always :: $(_install_MANDIR_SECTION3)

$(_install_MANDIR_SECTION3) : $(MANDIR)/man3/% : %
	$(INSTALL.MAN) $< $@

endif

ifneq "$(SECTION4)" ""

_install_directories += $(MANDIR)/man4

_install_MANDIR_SECTION4 := $(addprefix $(MANDIR)/man4/,$(SECTION4))

install-man.always :: $(_install_MANDIR_SECTION4)

$(_install_MANDIR_SECTION4) : $(MANDIR)/man4/% : %
	$(INSTALL.MAN) $< $@

endif

ifneq "$(SECTION5)" ""

_install_directories += $(MANDIR)/man5

_install_MANDIR_SECTION5 := $(addprefix $(MANDIR)/man5/,$(SECTION5))

install-man.always :: $(_install_MANDIR_SECTION5)

$(_install_MANDIR_SECTION5) : $(MANDIR)/man5/% : %
	$(INSTALL.MAN) $< $@

endif

ifneq "$(SECTION6)" ""

_install_directories += $(MANDIR)/man6

_install_MANDIR_SECTION6 := $(addprefix $(MANDIR)/man6/,$(SECTION6))

install-man.always :: $(_install_MANDIR_SECTION6)

$(_install_MANDIR_SECTION6) : $(MANDIR)/man6/% : %
	$(INSTALL.MAN) $< $@

endif

ifneq "$(SECTION7)" ""

_install_directories += $(MANDIR)/man7

_install_MANDIR_SECTION7 := $(addprefix $(MANDIR)/man7/,$(SECTION7))

install-man.always :: $(_install_MANDIR_SECTION7)

$(_install_MANDIR_SECTION7) : $(MANDIR)/man7/% : %
	$(INSTALL.MAN) $< $@

endif

ifneq "$(SECTION8)" ""

_install_directories += $(MANDIR)/man8

_install_MANDIR_SECTION8 := $(addprefix $(MANDIR)/man8/,$(SECTION8))

install-man.always :: $(_install_MANDIR_SECTION8)

$(_install_MANDIR_SECTION8) : $(MANDIR)/man8/% : %
	$(INSTALL.MAN) $< $@

endif

endif

## Installation for auxiliary files.

ifeq "$(origin INSTALL.AUXFLAGS1)" "undefined"
INSTALL.AUXFLAGS1 := $(INSTALL.AUXFLAGS)
endif
ifeq "$(origin INSTALL.AUXFLAGS2)" "undefined"
INSTALL.AUXFLAGS2 := $(INSTALL.AUXFLAGS)
endif
ifeq "$(origin INSTALL.AUXFLAGS3)" "undefined"
INSTALL.AUXFLAGS3 := $(INSTALL.AUXFLAGS)
endif
ifeq "$(origin INSTALL.AUXFLAGS4)" "undefined"
INSTALL.AUXFLAGS4 := $(INSTALL.AUXFLAGS)
endif
ifeq "$(origin INSTALL.AUXFLAGS5)" "undefined"
INSTALL.AUXFLAGS5 := $(INSTALL.AUXFLAGS)
endif
ifeq "$(origin INSTALL.AUXFLAGS6)" "undefined"
INSTALL.AUXFLAGS6 := $(INSTALL.AUXFLAGS)
endif

INSTALL.AUX1 := $(INSTALL) $(INSTALL.AUXFLAGS1)
INSTALL.AUX2 := $(INSTALL) $(INSTALL.AUXFLAGS2)
INSTALL.AUX3 := $(INSTALL) $(INSTALL.AUXFLAGS3)
INSTALL.AUX4 := $(INSTALL) $(INSTALL.AUXFLAGS4)
INSTALL.AUX5 := $(INSTALL) $(INSTALL.AUXFLAGS5)
INSTALL.AUX6 := $(INSTALL) $(INSTALL.AUXFLAGS6)

ifeq "$(AUXDIR1)" ""
override AUXDIR1 := $(AUXDIR)
override AUXILIARIES1 := $(AUXILIARIES)
endif

ifneq "$(AUXDIR1)" ""

_install_directories += $(AUXDIR1)

ifneq "$(AUXILIARIES1)" ""

_install_AUXDIR_AUXILIARIES1 := \
 $(patsubst %,$(AUXDIR1)/$(AUXPREFIX1)%$(AUXSUFFIX1),$(AUXILIARIES1))

install-aux.always :: $(_install_AUXDIR_AUXILIARIES1)

$(_install_AUXDIR_AUXILIARIES1) : $(AUXDIR1)/$(AUXPREFIX1)%$(AUXSUFFIX1) : %
	$(INSTALL.AUX1) $< $@

endif

endif

ifneq "$(AUXDIR2)" ""

_install_directories += $(AUXDIR2)

ifneq "$(AUXILIARIES2)" ""

_install_AUXDIR_AUXILIARIES2 := \
 $(patsubst %,$(AUXDIR2)/$(AUXPREFIX2)%$(AUXSUFFIX2),$(AUXILIARIES2))

install-aux.always :: $(_install_AUXDIR_AUXILIARIES2)

$(_install_AUXDIR_AUXILIARIES2) : $(AUXDIR2)/$(AUXPREFIX2)%$(AUXSUFFIX2) : %
	$(INSTALL.AUX2) $< $@

endif

endif

ifneq "$(AUXDIR3)" ""

_install_directories += $(AUXDIR3)

ifneq "$(AUXILIARIES3)" ""

_install_AUXDIR_AUXILIARIES3 := \
 $(patsubst %,$(AUXDIR3)/$(AUXPREFIX3)%$(AUXSUFFIX3),$(AUXILIARIES3))

install-aux.always :: $(_install_AUXDIR_AUXILIARIES3)

$(_install_AUXDIR_AUXILIARIES3) : $(AUXDIR3)/$(AUXPREFIX3)%$(AUXSUFFIX3) : %
	$(INSTALL.AUX3) $< $@

endif

endif

ifneq "$(AUXDIR4)" ""

_install_directories += $(AUXDIR4)

ifneq "$(AUXILIARIES4)" ""

_install_AUXDIR_AUXILIARIES4 := \
 $(patsubst %,$(AUXDIR4)/$(AUXPREFIX4)%$(AUXSUFFIX4),$(AUXILIARIES4))

install-aux.always :: $(_install_AUXDIR_AUXILIARIES4)

$(_install_AUXDIR_AUXILIARIES4) : $(AUXDIR4)/$(AUXPREFIX4)%$(AUXSUFFIX4) : %
	$(INSTALL.AUX4) $< $@

endif

endif

ifneq "$(AUXDIR5)" ""

_install_directories += $(AUXDIR5)

ifneq "$(AUXILIARIES5)" ""

_install_AUXDIR_AUXILIARIES5 := \
 $(patsubst %,$(AUXDIR5)/$(AUXPREFIX5)%$(AUXSUFFIX5),$(AUXILIARIES5))

install-aux.always :: $(_install_AUXDIR_AUXILIARIES5)

$(_install_AUXDIR_AUXILIARIES5) : $(AUXDIR5)/$(AUXPREFIX5)%$(AUXSUFFIX5) : %
	$(INSTALL.AUX5) $< $@

endif

endif

ifneq "$(AUXDIR6)" ""

_install_directories += $(AUXDIR6)

ifneq "$(AUXILIARIES6)" ""

_install_AUXDIR_AUXILIARIES6 := \
 $(patsubst %,$(AUXDIR6)/$(AUXPREFIX6)%$(AUXSUFFIX6),$(AUXILIARIES6))

install-aux.always :: $(_install_AUXDIR_AUXILIARIES6)

$(_install_AUXDIR_AUXILIARIES6) : $(AUXDIR6)/$(AUXPREFIX6)%$(AUXSUFFIX6) : %
	$(INSTALL.AUX6) $< $@

endif

endif

## Setup rule for creation of installation directories.

_install_directories := $(sort $(_install_directories) $(INSTALLDIRS))

install.setup :: $(_install_directories)

ifneq "$(_install_directories)" ""
$(_install_directories) : % :
	$(MKINSTALLDIRS) $@
endif
