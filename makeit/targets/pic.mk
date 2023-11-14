## Copyright 1992-1993 OTC LIMITED
## Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

(%) : %

## Defaults for library operations.

define DEFAULT_PIC_CLOSURE
endef

define DEFAULT_PIC_LIBRARY
exit 1; *** makeit needs to be configured ***
endef

ifeq "$(origin COMPILER_PIC_CLOSURE)" "undefined"
override COMPILER_PIC_CLOSURE = $(DEFAULT_PIC_CLOSURE)
endif

ifeq "$(origin COMPILER_PIC_LIBRARY)" "undefined"
override COMPILER_PIC_LIBRARY = $(DEFAULT_PIC_LIBRARY)
endif

## If no special action for building a loadable
## module then default to using that for building
## the shared library. This should be the case on
## all platforms except for Mac OS X and those
## which embed version numbering into the name
## of a shared library.

ifeq "$(origin COMPILER_PIC_MODULE)" "undefined"
ifeq "$(origin COMPILER_PIC_LIBRARY)" "undefined"
override COMPILER_PIC_MODULE = $(DEFAULT_PIC_LIBRARY)
else
override COMPILER_PIC_MODULE = $(COMPILER_PIC_LIBRARY)
endif
endif

## Pick up LIBDIR as location to install any
## products if PICDIR isn't explicitly defined.

ifeq "$(PICDIR)" ""
ifneq "$(LIBDIR)" ""
override PICDIR := $(LIBDIR)
endif
endif

## Deduce the list of input object files. If
## there aren't any, we don't need to do
## anything.

_makeit_PIC_OBJECTS := $(notdir $(_makeit_MK_LIB_OBJECTS))

ifeq "$(filter objects_pic_by_default,$(COMPILER_ATTRIBUTES))" ""
_makeit_MK_PIC_OBJECTS := $(addprefix $(MK).pic/,$(_makeit_PIC_OBJECTS))
else
_makeit_MK_PIC_OBJECTS := $(_makeit_MK_LIB_OBJECTS)
endif

ifneq "$(_makeit_MK_PIC_OBJECTS)" ""

## Last hurdle is whether a shared library
## or loadable module can be built for the
## current variant.

ifneq "$(filter $(VARIANT),$(COMPILER_PIC_VARIANTS))" ""

all.always :: pic

## If no shared library or loadable module
## name is provided, we need to provide a
## dummy instead.

LIBNAME.pic = $(firstword $(LIBNAME) NONAME)

## Put together the compiler flags we need
## to use.

ALL_LDFLAGS.pic = $(LDFLAGS_$(VARIANT)) $(LDFLAGS) \
 $(PROJECT_LDFLAGS_$(VARIANT)) $(PROJECT_LDFLAGS) \
 $(COMPILER_LDFLAGS_$(VARIANT)) $(COMPILER_LDFLAGS) \
 $(SYSTEM_LDFLAGS_$(VARIANT)) $(SYSTEM_LDFLAGS)

ifeq "$(origin CLOSURE_LDFLAGS)" "undefined"
override CLOSURE_LDFLAGS.pic = $(ALL_LDFLAGS.pic)
else
override CLOSURE_LDFLAGS.pic = $(CLOSURE_LDFLAGS)
endif

ALL_LDLIBS.pic = $(LDLIBS_$(VARIANT)) $(LDLIBS) \
 $(PROJECT_LDLIBS_$(VARIANT)) $(PROJECT_LDLIBS) \
 $(COMPILER_LDLIBS_$(VARIANT)) $(COMPILER_LDLIBS) \
 $(SYSTEM_LDLIBS_$(VARIANT)) $(SYSTEM_LDLIBS)

ifeq "$(origin CLOSURE_LDLIBS)" "undefined"
override CLOSURE_LDLIBS.pic = $(ALL_LDLIBS.pic)
else
override CLOSURE_LDLIBS.pic = $(CLOSURE_LDLIBS)
endif

PIC_LDFLAGS.pic = $(PIC_LDFLAGS) $(PROJECT_PIC_LDFLAGS) \
 $(COMPILER_PIC_LDFLAGS) $(SYSTEM_PIC_LDFLAGS)

PIC_LDLIBS.pic = $(PIC_LDLIBS) $(PROJECT_PIC_LDLIBS) \
 $(COMPILER_PIC_LDLIBS) $(SYSTEM_PIC_LDLIBS)

LDFLAGS.pic = $(filter -L%,$(ALL_LDFLAGS.pic))
LDLIBS.pic = $(filter -l%,$(ALL_LDLIBS.pic))

## Expanded library operations common to both
## shared libraries and loadable modules.

PIC_CLOSURE.pic = \
 $(subst @LDFLAGS@,$(filter -L%,$(CLOSURE_LDFLAGS.pic)), \
 $(subst @LDLIBS@,$(filter %$(LIBEXT) -l%,$(CLOSURE_LDLIBS.pic)), \
 $(subst @OBJECTS@,$?, \
 $(subst @ALLOBJECTS@,$^, \
 $(COMPILER_PIC_CLOSURE)))))

RUN-PIC_CLOSURE.pic = \
 $(PIC_CLOSURE.pic)

## First handle loadable modules. A loadable
## module does not have a version number or
## any type of prefix to the name.

ifneq "$(filter loadable_module,$(PIC_OPTIONS))" ""

PIC_LIBRARY.pic = \
 $(subst @TARGET@,$@, \
 $(subst @DESTDIR@,$(PICDIR), \
 $(subst @FILE@,$(notdir $@), \
 $(subst @NAME@,$(LIBNAME.pic), \
 $(subst @PIC_LDFLAGS@,$(PIC_LDFLAGS.pic), \
 $(subst @LDFLAGS@,$(LDFLAGS.pic), \
 $(subst @PIC_LDLIBS@,$(PIC_LDLIBS.pic), \
 $(subst @LDLIBS@,$(LDLIBS.pic), \
 $(subst @OBJECTS@,$(_makeit_MK_PIC_OBJECTS), \
 $(COMPILER_PIC_MODULE))))))))))

RUN-PIC_LIBRARY.pic = \
 $(PIC_LIBRARY.pic)

pic.always :: $(MK)/$(LIBNAME.pic)$(SHMEXT)

$(MK)/$(LIBNAME.pic)$(SHMEXT) :: $(_makeit_MK_PIC_OBJECTS)
	-$(RUN-PIC_CLOSURE.pic)
	$(RUN-PIC_LIBRARY.pic) || ($(RM) $@; exit 1)

## The loadable module will only be installed
## if a location had been defined as well as
## a name for the loadable module.

ifneq "$(PICDIR)" ""

ifneq "$(LIBNAME)" ""

_install_directories += $(PICDIR)

install-pic.always :: $(PICDIR)/$(LIBNAME)$(SHMEXT)

$(PICDIR)/$(LIBNAME)$(SHMEXT) : $(MK)/$(LIBNAME.pic)$(SHMEXT)
	$(INSTALL.PIC) $< $@

endif

endif

endif

## Now for shared libraries. Shared libraries
## can have version information embedded into
## the name of the library. One some platforms
## the ultimate location of the library also
## needs to be defined when the library is
## being constructed.

ifneq "$(filter shared_library,$(PIC_OPTIONS))" ""

ifeq "$(LIBVERSION)" ""
LIBVERSION.pic = 1.0
else
LIBVERSION.pic = $(LIBVERSION)
endif

PIC_LIBRARY.pic = \
 $(subst @TARGET@,$@, \
 $(subst @DESTDIR@,$(PICDIR), \
 $(subst @FILE@,$(notdir $@), \
 $(subst @NAME@,$(LIBNAME.pic), \
 $(subst @VERSION@,$(LIBVERSION.pic), \
 $(subst @PIC_LDFLAGS@,$(PIC_LDFLAGS.pic), \
 $(subst @LDFLAGS@,$(LDFLAGS.pic), \
 $(subst @PIC_LDLIBS@,$(PIC_LDLIBS.pic), \
 $(subst @LDLIBS@,$(LDLIBS.pic), \
 $(subst @OBJECTS@,$(_makeit_MK_PIC_OBJECTS), \
 $(COMPILER_PIC_LIBRARY)))))))))))

RUN-PIC_LIBRARY.pic = \
 $(PIC_LIBRARY.pic)

ifeq "$(SHLEXT)" ".so"

pic.always :: $(MK)/$(SHLPRE)$(LIBNAME.pic)$(SHLEXT).$(LIBVERSION.pic)

$(MK)/$(SHLPRE)$(LIBNAME.pic)$(SHLEXT).$(LIBVERSION.pic) :: \
 $(_makeit_MK_PIC_OBJECTS)
	-$(RUN-PIC_CLOSURE.pic)
	$(RUN-PIC_LIBRARY.pic) || ($(RM) $@; exit 1)

else

ifeq "$(SHLEXT)" ".dylib"

pic.always :: $(MK)/$(SHLPRE)$(LIBNAME.pic).$(LIBVERSION.pic)$(SHLEXT)

$(MK)/$(SHLPRE)$(LIBNAME.pic).$(LIBVERSION.pic)$(SHLEXT) :: \
 $(_makeit_MK_PIC_OBJECTS)
	-$(RUN-PIC_CLOSURE.pic)
	$(RUN-PIC_LIBRARY.pic) || ($(RM) $@; exit 1)

else

pic.always :: $(MK)/$(SHLPRE)$(LIBNAME.pic)$(SHLEXT)

$(MK)/$(SHLPRE)$(LIBNAME.pic)$(SHLEXT) :: $(_makeit_MK_PIC_OBJECTS)
	-$(RUN-PIC_CLOSURE.pic)
	$(RUN-PIC_LIBRARY.pic) || ($(RM) $@; exit 1)

endif

endif

## The shared library will only be installed
## if a location had been defined as well as
## a name for it. Installation can be a bit
## tricky depending on the platform. Rather
## than embed knowledge of specific platforms,
## attempt to deduce what to do based on the
## shared library extension.

ifneq "$(PICDIR)" ""

ifneq "$(LIBNAME)" ""

_install_directories += $(PICDIR)

ifeq "$(SHLEXT)" ".so"

install-pic.always :: \
 $(PICDIR)/$(SHLPRE)$(LIBNAME.pic)$(SHLEXT).$(LIBVERSION.pic)

$(PICDIR)/$(SHLPRE)$(LIBNAME.pic)$(SHLEXT).$(LIBVERSION.pic) : \
  $(MK)/$(SHLPRE)$(LIBNAME.pic)$(SHLEXT).$(LIBVERSION.pic)
	$(INSTALL.PIC) $< $@
	$(RM) $(PICDIR)/$(SHLPRE)$(LIBNAME)$(SHLEXT)
	ln -s $(notdir $@) $(PICDIR)/$(SHLPRE)$(LIBNAME)$(SHLEXT)

else

ifeq "$(SHLEXT)" ".dylib"

install-pic.always :: \
 $(PICDIR)/$(SHLPRE)$(LIBNAME.pic).$(LIBVERSION.pic)$(SHLEXT)

$(PICDIR)/$(SHLPRE)$(LIBNAME.pic).$(LIBVERSION.pic)$(SHLEXT) : \
  $(MK)/$(SHLPRE)$(LIBNAME.pic).$(LIBVERSION.pic)$(SHLEXT)
	$(INSTALL.PIC) $< $@
	$(RM) $(PICDIR)/$(SHLPRE)$(LIBNAME)$(SHLEXT)
	ln -s $(notdir $@) $(PICDIR)/$(SHLPRE)$(LIBNAME)$(SHLEXT)

else

install-pic.always :: $(PICDIR)/$(SHLPRE)$(LIBNAME.pic)$(SHLEXT)

$(PICDIR)/$(SHLPRE)$(LIBNAME.pic)$(SHLEXT) : \
 $(MK)/$(SHLPRE)$(LIBNAME.pic)$(SHLEXT)
	$(INSTALL.PIC) $< $@

endif

endif

endif

endif

## When objects aren't pic by default there
## will be a separate build subdirectory for
## them. This needs to be created and also
## subsequently removed.

ifeq "$(filter objects_pic_by_default,$(COMPILER_ATTRIBUTES))" ""

mostlyclean.target ::
	$(RM) $(_makeit_MK_PIC_OBJECTS)

clean.always ::
	rm -rf $(MK).pic

ifeq "$(filter mostlyclean clean realclean distclean,$(MAKECMDGOALS))" ""

makeit.setup :: $(MK).pic

$(MK).pic :
	@mkdir $@

endif

endif

endif

endif

endif
