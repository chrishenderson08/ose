# This makefile is for Visual C++ and NMAKE only.

CC = CL
AR = LIB

CPPFLAGS = -I../include
CFLAGS = -GX -Zi -Od -MTd

include <objects.mk>

OBJECTS = $(OBJECTS1) $(OBJECTS2)
#OBJECTS = $(OBJECTS1) $(OBJECTS2) $(OBJECTS3)

ose_dbg.lib : $(OBJECTS)
	$(AR) -out:$@ $(OBJECTS)

include <rules.mk>

clean :
	del *.obj

distclean :
	del *.lib *.dll *.exp *.pdb
