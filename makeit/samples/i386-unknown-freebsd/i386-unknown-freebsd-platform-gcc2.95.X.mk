ifneq "$(filter math,$(PLATFORM_OPTIONS))" ""
COMPILER_LDLIBS += -lm
endif
ifneq "$(filter pthread,$(PLATFORM_OPTIONS))" ""
COMPILER_LDFLAGS += -pthread
endif

ifneq "$(filter pthread,$(PLATFORM_OPTIONS))" ""
COMPILER_PIC_LDFLAGS += -pthread
endif
