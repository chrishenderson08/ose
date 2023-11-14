/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     system/_program.cc
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

#include <OTC/system/program.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/stdlib/strstream.h>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

char* argv1[] =
{
  "program", "arg1", "arg2", 0
};

void test1()
{
  OTC_Tracer tracer("void test1()");

  OTC_Program::initialise(3,argv1);
  tracer() << "name = " << OTC_Program::name() << endl;
  tracer() << "numOptions = " << OTC_Program::numOptions() << endl;
  for (int i=0; i<=OTC_Program::numOptions(); i++)
    tracer() << "option " << i << " = " << OTC_Program::option(i) << endl;
  OTC_Iterator<OTC_String> options(OTC_Program::options());
  tracer() << "all options =";
  for (options.reset(); options.isValid(); options.next())
    tracer() << " " << options.item();
  tracer() << endl;
}

char* argv2[] =
{
  "program", "-v", "-DONE", "+i", "-DTWO=TWO", "one.h", "two.h", 0
};

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTC_Program::initialise(7,argv2);
  tracer() << "name = " << OTC_Program::name() << endl;
  tracer() << "numOptions = " << OTC_Program::numOptions() << endl;

  while (OTC_Program::numOptions() != 0)
  {
    if (OTC_Program::match("-D*=*"))
    {
      tracer() << "define+value - " << OTC_Program::option() << endl;
      OTC_Program::shift();
    }
    else if (OTC_Program::match("-D*"))
    {
      tracer() << "define - " << OTC_Program::option() << endl;
      OTC_Program::shift();
    }
    else if (OTC_Program::match("[+-]*"))
    {
      tracer() << "option - " << OTC_Program::option() << endl;
      OTC_Program::shift();
    }
    else if (OTC_Program::match("*.h"))
    {
      tracer() << "file - " << OTC_Program::option() << endl;
      OTC_Program::shift();
    }
    else
    {
      tracer() << "all - " << OTC_Program::option() << endl;
      OTC_Program::shift();
    }
  }
  OTC_Program::restore();
}

void test3()
{
  OTC_Tracer tracer("void test3()");

  for (int i=1; i<100; i++)
  {
    tracer() << OTC_Program::uniqueId(OTCLIB_ID_LONG_FORMAT) << endl;
    tracer() << OTC_Program::uniqueId(OTCLIB_ID_SHORT_FORMAT) << endl;
    tracer() << OTC_Program::uniqueId("$ID?",OTCLIB_ID_LONG_FORMAT) << endl;
    tracer() << OTC_Program::uniqueId("$ID?",OTCLIB_ID_SHORT_FORMAT) << endl;
  }
}

char* text =
 "*.one : value1\n"
 "one.two : value2\n"
 "one.two.three : value3\n"
 "one.*.two.three : value4\n"
 "one.two.*.three : value5\n"
 "one.two.three.four : value5\\\nvalue6\n"
 "one.two.three.five value7";

void test4()
{
  OTC_Tracer tracer("void test4()");

  istrstream ins(text,strlen(text));

  OTC_Program::loadConfig(ins);

  OTC_String theLine;

  while (cin.good())
  {
    theLine.readLine(cin);
    if (!cin.fail())
    {
      OTC_String theValue;

      theValue = OTC_Program::lookupConfig(theLine);

      if (theValue.isUndefined())
	tracer() << "<undefined>" << endl;
      else
	tracer() << theValue << endl;
    }
  }
}

void test5()
{
  OTC_Tracer tracer("void test5()");

  OTC_String theValue;

  theValue = OTC_Program::lookupEnviron("TZ");

  tracer() << "TZ=" << theValue << endl;

  if (getenv("TZ") == 0)
    tracer() << "getenv(TZ)=<undefined>" << endl;
  else
    tracer() << "getenv(TZ)=" << getenv("TZ") << endl;

  OTC_Program::mergeEnviron("TZ","GMT");

  tracer() << "TZ=" << OTC_Program::lookupEnviron("TZ") << endl;
  tracer() << "getenv(TZ)=" << getenv("TZ") << endl;

  OTC_Program::mergeEnviron("TZ",theValue);

  tracer() << "TZ=" << OTC_Program::lookupEnviron("TZ") << endl;

  if (getenv("TZ") == 0)
    tracer() << "getenv(TZ)=<undefined>" << endl;
  else
    tracer() << "getenv(TZ)=" << getenv("TZ") << endl;
}

void test6()
{
  OTC_Tracer tracer("void test6()");

  char const* s1 = "(SHELL=${SHELL})";
  char const* s2 = "(SHELL=${SHELL})(EDITOR=${EDITOR})";
  char const* s3 = "(SHELL=$SHELL)";
  char const* s4 = "(SHELL=$SHELL)(EDITOR=${EDITOR})";
  char const* s5 = "(SHELL=${SHELL})(EDITOR=$EDITOR)";

  tracer() << OTC_Program::expandEnviron(s1) << endl;
  tracer() << OTC_Program::expandEnviron(s2) << endl;
  tracer() << OTC_Program::expandEnviron(s3) << endl;
  tracer() << OTC_Program::expandEnviron(s4) << endl;
  tracer() << OTC_Program::expandEnviron(s5) << endl;
}

void test7()
{
  OTC_Tracer tracer("void test7()");

  OTC_String theLine;

  while (cin.good())
  {
    theLine.readLine(cin);
    if (!cin.fail())
    {
      OTC_String theValue;

      theValue = OTC_Program::expandEnviron(theLine);

      tracer() << theValue << endl;
    }
  }
}

void test8()
{
  OTC_Tracer tracer("void test8()");

  cout << OTC_Program::hostname() << endl;
  cout << OTC_Program::processId() << endl;
  cout << OTC_Program::processIdentity() << endl;
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
