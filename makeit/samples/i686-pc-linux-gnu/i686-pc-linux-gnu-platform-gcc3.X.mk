ifneq "$(filter math,$(PLATFORM_OPTIONS))" ""
COMPILER_LDLIBS += -lm
endif
ifneq "$(filter pthread,$(PLATFORM_OPTIONS))" ""
COMPILER_LDLIBS += -lpthread
endif
