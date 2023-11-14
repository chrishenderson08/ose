ifneq "$(_makeit_MK_PIC_OBJECTS)" ""
ifneq "$(filter $(VARIANT),$(COMPILER_PIC_VARIANTS))" ""
ifneq "$(filter shared_library,$(PIC_OPTIONS))" ""

lib.always :: $(MK)/lib$(LIBEXT)

$(MK)/lib$(LIBEXT) : $(MK)/$(SHLPRE)$(LIBNAME.pic)$(SHLEXT)
	mv $(MK)/$(LIBPRE)$(LIBNAME.pic)$(LIBEXT) $@

ifneq "$(LIBDIR)" ""

install-pic.always :: $(LIBDIR)/$(LIBPRE)$(LIBNAME.pic)$(LIBEXT)

$(LIBDIR)/$(LIBPRE)$(LIBNAME.pic)$(LIBEXT) : \
 $(MK)/$(LIBPRE)$(LIBNAME.pic)$(LIBEXT)
	$(INSTALL.LIB) $< $@

endif

endif
endif
endif
