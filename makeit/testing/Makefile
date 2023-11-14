# The yet to be installed version of makeit is to be used. Thus need
# to specify where that resides. If using installed version of makeit
# would not need to do this and way top level makefiles included would
# be slightly different as well.

SRCROOT = ..

MAKEIT_HOME = $(SRCROOT)

# We want support for compiling C and C++ code to be included.

MODULES = c cxx

# Trigger building of a shared library. Nothing will happen in respect
# of this option if default configuration being used or the selected
# compiler configuration doesn't support build PIC code within the
# variant specified when build is triggered.

PIC_OPTIONS = shared_library

# Include standard top level setup makefile for makeit.

include $(SRCROOT)/makeit/setup.mk

# List the names of the programs, no need to list the actual program
# source code files as makeit will work that out automatically. Any
# library source code files to go into the library do not need to be
# listed. Thus, if all you are interested in is building some programs,
# but with everything else automatically being put in a library, the only
# thing you need to do is specify the names of the programs in the
# PROGRAMS variable. Note that the names of the files used in this example
# were chosen merely to make it easier to see how they were being used.
# There is no need for program source code files to contain the actual
# string "program", nor is there any need for library source code files
# to contain the string "libsrc".

PROGRAMS += program1
PROGRAMS += program2

# Below, we want however to test the case of non library source code files.
# These are different to library source code files and are source code files
# which are neither program source code files or library source code files.
# Except for when excluding source files from being compiled at all, this
# is the only time that names of source code files which are to be compiled
# need actually be listed. Note that non library source code files will
# only actually be compiled if a rule is included which is dependent on the
# object file produced from it. Similarly for the naming of other files used
# in this example.

NONLIBSRC += nonlibsrc1.cc
NONLIBSRC += nonlibsrc2.c

# List the names of source code files we want to be excluded from any
# compilation at all.

EXCLUDE += exclude1.cc
EXCLUDE += exclude2.c

# Specify the library name. Since installation of build results not being
# done for this example, this only gets used in name of the shared library
# created in the local directory.

LIBNAME = testing

# Include standard top level modules makefile for makeit.

include $(SRCROOT)/makeit/modules.mk

# Trigger building of non library source code files by indicating that
# the programs are dependent upon them. This will also have the side
# effect of causing the object files created from compiling the non library
# source code files, to be linked in with those programs. Note that the
# BINEXT and OBJEXT variables are used to ensure that the makefile is
# portable to Win32 systems.

$(MK)/program1$(BINEXT) : $(MK)/nonlibsrc1$(OBJEXT)
$(MK)/program2$(BINEXT) : $(MK)/nonlibsrc2$(OBJEXT)
