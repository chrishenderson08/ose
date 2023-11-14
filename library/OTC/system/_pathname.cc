/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     system/_pathname.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1994-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/system/pathname.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/OTC.h>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

void test1()
{
  OTC_Tracer tracer("void test1()");

  OTC_Pathname path1 = "file";
  OTC_String string1 = "file";
  OTC_String string2;

  tracer() << "path1 = " << path1 << endl;
  tracer() << "string1 = " << string1 << endl;
  tracer() << "string2 = " << string2 << endl;

  tracer() << "string2 = string1 + path1" << endl;
  string2 = string1 + path1;

  tracer() << "string2 = " << string2 << endl;

  tracer() << "string2 = string1 + '/' + path1" << endl;
  string2 = string1 + '/' + path1;

  tracer() << "string2 = " << string2 << endl;

  // tracer() << "string2 = path1 + '/' + path1" << endl;
  // string2 = path1 + '/' + path1;

  // tracer() << "string2 = " << string2 << endl;

  tracer() << "path1 += \"file\"" << endl;
  path1 += "file";

  tracer() << "path1 = " << path1 << endl;

  tracer() << "path1 += '/'" << endl;
  path1 += "/";

  tracer() << "path1 = " << path1 << endl;

  tracer() << "path1 += path1" << endl;
  path1 += path1;

  tracer() << "path1 = " << path1 << endl;
}

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTC_Pathname pathname1("/tmp/foo.h");
  tracer() << "pathname = " << pathname1 << endl;
  tracer() << "basename = " << pathname1.basename() << endl;
  tracer() << "basename .h = " << pathname1.basename(".h") << endl;
  tracer() << "dirname = " << pathname1.dirname() << endl;
  tracer() << "absolute = " << pathname1.isAbsolute() << endl;
  tracer() << "relative = " << pathname1.isRelative() << endl;

  OTC_Pathname pathname2("/tmp/");
  tracer() << "pathname = " << pathname2 << endl;
  tracer() << "basename = " << pathname2.basename() << endl;
  tracer() << "basename .h = " << pathname2.basename(".h") << endl;
  tracer() << "dirname = " << pathname2.dirname() << endl;
  tracer() << "absolute = " << pathname2.isAbsolute() << endl;
  tracer() << "relative = " << pathname2.isRelative() << endl;

  OTC_Pathname pathname3("/tmp");
  tracer() << "pathname = " << pathname3 << endl;
  tracer() << "basename = " << pathname3.basename() << endl;
  tracer() << "basename .h = " << pathname3.basename(".h") << endl;
  tracer() << "dirname = " << pathname3.dirname() << endl;
  tracer() << "absolute = " << pathname3.isAbsolute() << endl;
  tracer() << "relative = " << pathname3.isRelative() << endl;

  OTC_Pathname pathname4("tmp/foo.h");
  tracer() << "pathname = " << pathname4 << endl;
  tracer() << "basename = " << pathname4.basename() << endl;
  tracer() << "basename .h = " << pathname4.basename(".h") << endl;
  tracer() << "dirname = " << pathname4.dirname() << endl;
  tracer() << "absolute = " << pathname4.isAbsolute() << endl;
  tracer() << "relative = " << pathname4.isRelative() << endl;

  OTC_Pathname pathname5("/");
  tracer() << "pathname = " << pathname5 << endl;
  tracer() << "basename = " << pathname5.basename() << endl;
  tracer() << "basename .h = " << pathname5.basename(".h") << endl;
  tracer() << "dirname = " << pathname5.dirname() << endl;
  tracer() << "absolute = " << pathname5.isAbsolute() << endl;
  tracer() << "relative = " << pathname5.isRelative() << endl;

  OTC_Pathname pathname6("");
  tracer() << "pathname = " << pathname6 << endl;
  tracer() << "basename = " << pathname6.basename() << endl;
  tracer() << "basename .h = " << pathname6.basename(".h") << endl;
  tracer() << "dirname = " << pathname6.dirname() << endl;
  tracer() << "absolute = " << pathname6.isAbsolute() << endl;
  tracer() << "relative = " << pathname6.isRelative() << endl;

  OTC_Pathname pathname7("foo.h");
  tracer() << "pathname = " << pathname7 << endl;
  tracer() << "basename = " << pathname7.basename() << endl;
  tracer() << "basename .h = " << pathname7.basename(".h") << endl;
  tracer() << "dirname = " << pathname7.dirname() << endl;
  tracer() << "absolute = " << pathname7.isAbsolute() << endl;
  tracer() << "relative = " << pathname7.isRelative() << endl;

  OTC_Pathname pathname8("foo.h/");
  tracer() << "pathname = " << pathname8 << endl;
  tracer() << "basename = " << pathname8.basename() << endl;
  tracer() << "basename .h = " << pathname8.basename(".h") << endl;
  tracer() << "dirname = " << pathname8.dirname() << endl;
  tracer() << "absolute = " << pathname8.isAbsolute() << endl;
  tracer() << "relative = " << pathname8.isRelative() << endl;

  OTC_Pathname pathname9("/foo.h/");
  tracer() << "pathname = " << pathname9 << endl;
  tracer() << "basename = " << pathname9.basename() << endl;
  tracer() << "basename .h = " << pathname9.basename(".h") << endl;
  tracer() << "dirname = " << pathname9.dirname() << endl;
  tracer() << "absolute = " << pathname9.isAbsolute() << endl;
  tracer() << "relative = " << pathname9.isRelative() << endl;
}

void test3()
{
  OTC_Tracer tracer("void test3()");

  OTC_Pathname path1("");
  tracer() << path1.absolute() << endl;
  OTC_Pathname path2("/tmp");
  tracer() << path2.absolute() << endl;
  OTC_Pathname path3(".");
  tracer() << path3.absolute() << endl;
}

/* ------------------------------------------------------------------------- */

typedef void (*testFunc)();

testFunc tests[] =
{
  test1,
  test2,
  test3
};

/* ------------------------------------------------------------------------- */

int main(int argc, char* argv[])
{
  u_int const numTests = sizeof(tests)/sizeof(tests[0]);

  if (argc != 2)
  {
    cout << numTests << endl;
    return 1;
  }
  else
  {
    int testNum = atoi(argv[1]);
    if (testNum > 0 && u_int(testNum) <= numTests)
    {
      tests[testNum-1]();
      return 0;
    }
    else
      return 1;
  }
}

/* ------------------------------------------------------------------------- */
