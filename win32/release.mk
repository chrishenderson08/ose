# This makefile is for Visual C++ and NMAKE only.

CC = CL
AR = LIB

CPPFLAGS = -I../include -DOSE_DLL_EXPORT
CFLAGS = -GX -Zi -Od -MD
LDLIBS = ws2_32.lib

include <objects.mk>

OBJECTS = $(OBJECTS1) $(OBJECTS2)
#OBJECTS = $(OBJECTS1) $(OBJECTS2) $(OBJECTS3)

ose_opt.dll : $(OBJECTS)
	$(CC) -LD -o $@ $(OBJECTS) $(LDLIBS)

include <rules.mk>

clean :
	del *.obj

distclean :
	del *.lib *.dll *.exp *.pdb
