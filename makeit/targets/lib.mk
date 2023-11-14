## Copyright 1992-1993 OTC LIMITED
## Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

(%) : %

## Defaults for library operations.

ifeq "$(RANLIB)" "" 
override RANLIB := $(firstword $(wildcard /bin/ranlib /usr/bin/ranlib))
ifeq "$(RANLIB)" ""
override RANLIB := : ranlib
endif
endif

define DEFAULT_LIB_CLOSURE
endef

define DEFAULT_LIB_ARCHIVE
$(AR) rv @TARGET@ @OBJECTS@
endef

define DEFAULT_LIB_RANLIB
$(RANLIB) @TARGET@
endef

ifeq "$(origin COMPILER_LIB_CLOSURE)" "undefined"
override COMPILER_LIB_CLOSURE = $(DEFAULT_LIB_CLOSURE)
endif

ifeq "$(origin COMPILER_LIB_ARCHIVE)" "undefined"
override COMPILER_LIB_ARCHIVE = $(DEFAULT_LIB_ARCHIVE)
endif

ifeq "$(origin COMPILER_LIB_RANLIB)" "undefined"
override COMPILER_LIB_RANLIB = $(DEFAULT_LIB_RANLIB)
endif

## Only need to create a library if there are
## object files which need to be put in it.
## We don't need to create one however if the
## generation of the library archive is being
## suppressed. In either case, set of
## convenience targets are supplied so that
## building of objects is simplified.

_makeit_MK_LIB_OBJECTS := $(strip $(_makeit_MK_LIB_OBJECTS))

_makeit_LIB_OBJECTS = $(notdir $(_makeit_MK_LIB_OBJECTS))

ifneq "$(_makeit_MK_LIB_OBJECTS)" ""

$(patsubst %$(OBJEXT),%.o,$(_makeit_LIB_OBJECTS)) : %.o : makeit.setup

$(patsubst %$(OBJEXT),%.o,$(_makeit_LIB_OBJECTS)) : %.o : $(MK)/%$(OBJEXT)

ifeq "$(filter suppress_archive,$(LIB_OPTIONS))" ""

all.always :: lib

ALL_LDFLAGS.lib = $(LDFLAGS_$(VARIANT)) $(LDFLAGS) \
 $(PROJECT_LDFLAGS_$(VARIANT)) $(PROJECT_LDFLAGS) \
 $(COMPILER_LDFLAGS_$(VARIANT)) $(COMPILER_LDFLAGS) \
 $(SYSTEM_LDFLAGS_$(VARIANT)) $(SYSTEM_LDFLAGS)

ifeq "$(origin CLOSURE_LDFLAGS)" "undefined"
override CLOSURE_LDFLAGS.lib = $(ALL_LDFLAGS.lib)
else
override CLOSURE_LDFLAGS.lib = $(CLOSURE_LDFLAGS)
endif

ALL_LDLIBS.lib = $(LDLIBS_$(VARIANT)) $(LDLIBS) \
 $(PROJECT_LDLIBS_$(VARIANT)) $(PROJECT_LDLIBS) \
 $(COMPILER_LDLIBS_$(VARIANT)) $(COMPILER_LDLIBS) \
 $(SYSTEM_LDLIBS_$(VARIANT)) $(SYSTEM_LDLIBS)

ifeq "$(origin CLOSURE_LDLIBS)" "undefined"
override CLOSURE_LDLIBS.lib = $(ALL_LDLIBS.lib)
else
override CLOSURE_LDLIBS.lib = $(CLOSURE_LDLIBS)
endif

LIB_CLOSURE.lib = \
 $(subst @LDFLAGS@,$(filter -L%,$(CLOSURE_LDFLAGS.lib)), \
 $(subst @LDLIBS@,$(filter %$(LIBEXT) -l%,$(CLOSURE_LDLIBS.lib)), \
 $(subst @OBJECTS@,$?, \
 $(subst @ALLOBJECTS@,$^, \
 $(COMPILER_LIB_CLOSURE)))))

RUN-LIB_CLOSURE.lib = \
 $(LIB_CLOSURE.lib)

LIB_RANLIB.lib = \
 $(subst @TARGET@,$@, \
 $(COMPILER_LIB_RANLIB))

RUN-LIB_RANLIB.lib = \
 $(LIB_RANLIB.lib)

LIB_ARCHIVE.lib = \
 $(subst @TARGET@,$@, \
 $(subst @OBJECTS@,$?, \
 $(subst @ALLOBJECTS@,$^, \
 $(COMPILER_LIB_ARCHIVE))))

RUN-LIB_ARCHIVE.lib = \
 $(LIB_ARCHIVE.lib); \
 $(LIB_RANLIB.lib)

lib.always :: $(MK)/lib$(LIBEXT)

$(MK)/lib$(LIBEXT) :: $(_makeit_MK_LIB_OBJECTS)
	-$(RUN-LIB_CLOSURE.lib)
	$(RUN-LIB_ARCHIVE.lib)

## Library installation.

ifneq "$(LIBDIR)" ""

_install_directories += $(LIBDIR)

ifneq "$(LIBNAME)" ""

install-lib.always :: $(LIBDIR)/$(LIBPRE)$(LIBNAME)$(LIBEXT)

$(LIBDIR)/$(LIBPRE)$(LIBNAME)$(LIBEXT) : $(MK)/lib$(LIBEXT)
	$(INSTALL.LIB) $< $@
	$(RUN-LIB_RANLIB.lib)

endif

endif

endif

mostlyclean.target :: mostlyclean.setup
	$(RM) $(_makeit_MK_LIB_OBJECTS)

endif

## Build output directory.

mostlyclean.always :: mostlyclean.setup
ifneq "$(filter %$(OBJEXT),$(SRCFILES))" ""
	$(RM) *$(OBJEXT)
endif

ifeq "$(filter suppress_workarea,$(MAKEIT_OPTIONS))" ""

clean.always ::
	rm -rf $(MK)

ifeq "$(filter mostlyclean clean realclean distclean,$(MAKECMDGOALS))" ""

makeit.setup :: $(MK)

$(MK) :
	@mkdir $@

endif

endif
