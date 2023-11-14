/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     text/_options.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1992 OTC LIMITED
//     Copyright 1995-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/text/options.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/OTC.h>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

char* ARGV[] =
{
  "program", "arg1", "arg2", 0
};

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTC_Options args;
  args.initialise(3,ARGV);
  tracer() << "numOptions = " << args.numOptions() << endl;
  for (u_int i=1; i<=args.numOptions(); i++)
    tracer() << "option " << i << " = " << args.option(i) << endl;
  OTC_Iterator<OTC_String> options(args.options());
  tracer() << "all options =";
  for (options.reset(); options.isValid(); options.next())
    tracer() << " " << options.item();
  tracer() << endl;
}

void test3()
{
  OTC_Tracer tracer("void test3()");

  OTC_Options args;
  args.initialise("program arg1 arg2");
  tracer() << "numOptions = " << args.numOptions() << endl;
  for (u_int i=1; i<=args.numOptions(); i++)
    tracer() << "option " << i << " = " << args.option(i) << endl;
  OTC_Iterator<OTC_String> options(args.options());
  tracer() << "all options =";
  for (options.reset(); options.isValid(); options.next())
    tracer() << " " << options.item();
  tracer() << endl;
}

void test4()
{
  OTC_Tracer tracer("void test4()");

  OTC_Options args;
  args.initialise("program \"arg1-p1 arg1-p2\" arg2");
  tracer() << "numOptions = " << args.numOptions() << endl;
  for (u_int i=1; i<=args.numOptions(); i++)
    tracer() << "option " << i << " = " << args.option(i) << endl;
  OTC_Iterator<OTC_String> options(args.options());
  tracer() << "all options =";
  for (options.reset(); options.isValid(); options.next())
    tracer() << " " << options.item();
  tracer() << endl;
}

void test5()
{
  OTC_Tracer tracer("void test5()");

  OTC_Options args;
  args.initialise(" ");
  tracer() << "numOptions = " << args.numOptions() << endl;
  for (u_int i=1; i<=args.numOptions(); i++)
    tracer() << "option " << i << " = " << args.option(i) << endl;
  OTC_Iterator<OTC_String> options(args.options());
  tracer() << "all options =";
  for (options.reset(); options.isValid(); options.next())
    tracer() << " " << options.item();
  tracer() << endl;
}

void test6()
{
  OTC_Tracer tracer("void test6()");

  OTC_Options args;
  args.initialise("  program arg1 arg2  ");
  tracer() << "numOptions = " << args.numOptions() << endl;
  for (u_int i=1; i<=args.numOptions(); i++)
    tracer() << "option " << i << " = " << args.option(i) << endl;
  OTC_Iterator<OTC_String> options(args.options());
  tracer() << "all options =";
  for (options.reset(); options.isValid(); options.next())
    tracer() << " " << options.item();
  tracer() << endl;
}

void test7()
{
  OTC_Tracer tracer("void test7()");

  OTC_Options args;
  args.initialise("program \"\\\"arg1-p1 arg1-p2\\\"\" arg2");
  tracer() << "numOptions = " << args.numOptions() << endl;
  for (u_int i=1; i<=args.numOptions(); i++)
    tracer() << "option " << i << " = " << args.option(i) << endl;
  OTC_Iterator<OTC_String> options(args.options());
  tracer() << "all options =";
  for (options.reset(); options.isValid(); options.next())
    tracer() << " " << options.item();
  tracer() << endl;
}

void test8()
{
  OTC_Tracer tracer("void test8()");

  OTC_Options args;
  args.initialise(3,ARGV);
  tracer() << "numOptions = " << args.numOptions() << endl;
  while (args.numOptions() != 0)
  {
    tracer() << args.option(1) << endl;
    args.shift();
  }
  args.restore();
  while (args.numOptions() != 0)
  {
    tracer() << args.option(1) << endl;
    args.shift();
  }
}

void test1()
{
  OTC_Tracer tracer("void test1()");

  test2();
  test3();
  test4();
  test5();
  test6();
  test7();
  test8();
}

/* ------------------------------------------------------------------------- */

typedef void (*testFunc)();

testFunc tests[] =
{
  test1,
  test2,
  test3,
  test4,
  test5,
  test6,
  test7,
  test8
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
