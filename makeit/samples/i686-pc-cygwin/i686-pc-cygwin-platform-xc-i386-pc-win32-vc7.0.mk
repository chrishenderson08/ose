ifneq "$(filter pthread,$(PLATFORM_OPTIONS))" ""
COMPILER_CPPFLAGS += -Lc:/posix/include
COMPILER_LDFLAGS += -Lc:/posix/lib
COMPILER_LDLIBS += -lpthreadVC
endif
