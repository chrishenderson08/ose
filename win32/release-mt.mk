# This makefile is for Visual C++ and NMAKE only.

CC = CL
AR = LIB

CPPFLAGS = -I../include -DOSE_DLL_EXPORT -Ic:/posix/include -DOSE_WITH_THREADS
CFLAGS = -GX -Zi -Od -MD
LDLIBS = ws2_32.lib pthreadVC.lib

include <objects.mk>

OBJECTS = $(OBJECTS1) $(OBJECTS2)
#OBJECTS = $(OBJECTS1) $(OBJECTS2) $(OBJECTS3)

ose_mt_opt.dll : $(OBJECTS)
	$(CC) -LD -o $@ $(OBJECTS) $(LDLIBS)

include <rules.mk>

clean :
	del *.obj

distclean :
	del *.lib *.dll *.exp *.pdb
