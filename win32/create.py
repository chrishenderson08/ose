#! /usr/bin/env python

import os
import glob

def output(fd1,fd2,files,group):
  print >> fd1, "%s = \\" % group
  for f in files:
    if os.path.splitext(f)[-1] == ".cc":
      print >> fd1, " %s.obj \\" % os.path.basename(f[:-3])
    elif os.path.splitext(f)[-1] == ".c":
      print >> fd1, " %s.obj \\" % os.path.basename(f[:-2])
  print >> fd1
  print >> fd1
  for f in files:
    if os.path.splitext(f)[-1] == ".cc":
      print >> fd2, "%s.obj : %s" % (os.path.basename(f[:-3]),f)
      print >> fd2, "\t$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp %s" % f
    elif os.path.splitext(f)[-1] == ".c":
      print >> fd2, "%s.obj : %s" % (os.path.basename(f[:-2]),f)
      print >> fd2, "\t$(CC) -c $(CPPFLAGS) $(CFLAGS) %s" % f
    print >> fd2
  print >> fd2

fd1 = open("objects.mk","w")
fd2 = open("rules.mk","w")
print >> fd1, "# GENERATED FILE, DO NOT EDIT."
print >> fd1
print >> fd2, "# GENERATED FILE, DO NOT EDIT."
print >> fd2

code1 = glob.glob("../library/OTC/hash/hash_*.c") 
code1 += glob.glob("../library/OTC/regex/reg*.c") 
output(fd1,fd2,code1,"OBJECTS1")

code2 = glob.glob("../library/OTC/*/otc*.cc")
output(fd1,fd2,code2,"OBJECTS2")

code3 = glob.glob("../library/ONS/*/ons*.cc")
output(fd1,fd2,code3,"OBJECTS3")

fd1.close()
fd2.close()
