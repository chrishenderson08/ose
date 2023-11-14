ifneq "$(filter pthread,$(PLATFORM_OPTIONS))" ""
COMPILER_CPPFLAGS += -D_REENTRANT
endif

ifneq "$(filter socket,$(PLATFORM_OPTIONS))" ""
COMPILER_LDLIBS += -lsocket -lnsl
endif
ifneq "$(filter math,$(PLATFORM_OPTIONS))" ""
COMPILER_LDLIBS += -lm
endif
ifneq "$(filter pthread,$(PLATFORM_OPTIONS))" ""
COMPILER_LDLIBS += -lpthread
endif

COMPILER_LDLIBS += -ldl
