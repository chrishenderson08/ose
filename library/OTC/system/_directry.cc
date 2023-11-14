/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     system/_directry.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1995-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/system/directry.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/OTC.h>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

void test1()
{
  OTC_Tracer tracer("void test1()");

  OTC_Directory dir1(".");
  OTC_Iterator<OTC_String> files1(dir1.files());
  OTC_Iterator<OTC_String> files2(files1.clone());

  tracer() << "-- files1 --" << endl;
  for (files1.reset(); files1.isValid(); files1.next())
    tracer() << "\"" << files1.item() << "\"" << endl;

  tracer() << "-- files2 --" << endl;
  for (files2.reset(); files2.isValid(); files2.next())
    tracer() << "\"" << files2.item() << "\"" << endl;

  tracer() << "-- files1 and files2 --" << endl;
  for (
   files1.reset(), files2.reset();
   files1.isValid() && files2.isValid();
   files1.next(), files2.next()
  )
  {
    tracer() << "\"" << files1.item() << "\"" << endl;
    tracer() << "\"" << files2.item() << "\"" << endl;
  }
}

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTC_Directory dir1(".");
  tracer() << "searchable . = " << dir1.isSearchable() << endl;
  OTC_Directory dir2("/vmunix");
  tracer() << "searchable /vmunix = " << dir2.isSearchable() << endl;
  OTC_Directory dir3("/vmunix/..");
  tracer() << "searchable /vmunix/.. = " << dir3.isSearchable() << endl;
}

void test3()
{
  OTC_Tracer tracer("void test3()");

  OTC_Directory dir1(".");
  tracer() << "contains . = " << dir1.contains(".") << endl;
  tracer() << "contains .. = " << dir1.contains("..") << endl;
  tracer() << "contains / = " << dir1.contains("/") << endl;
  tracer() << "contains foo = " << dir1.contains("foo") << endl;
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
