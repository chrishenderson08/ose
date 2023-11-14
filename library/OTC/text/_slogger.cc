/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     text/_slogger.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1998-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/text/slogger.hh>
#include <OTC/debug/logstrm.hh>

#include <OTC/debug/tracer.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

void test1()
{
  OTC_Tracer tracer("void test1()");

  OTC_SLogger tmpLogger("@CHANNEL2");

  OTC_LogStream tmpLogStream;

  tmpLogStream.setChannel("@CHANNEL1");
  tmpLogStream << "CHANNEL1" << endl;

  tmpLogStream.setChannel("@CHANNEL2");
  tmpLogStream << "CHANNEL2" << endl;

  tmpLogStream.setChannel("@CHANNEL3");
  tmpLogStream << "CHANNEL3" << endl;

  tracer() << tmpLogger.messages() << endl;
  tmpLogger.clearLog();

  tmpLogStream.setChannel("@CHANNEL1");
  tmpLogStream << "CHANNEL1" << endl;

  tmpLogStream.setChannel("@CHANNEL2");
  tmpLogStream << "CHANNEL2" << endl;

  tmpLogStream.setChannel("@CHANNEL3");
  tmpLogStream << "CHANNEL3" << endl;

  tracer() << tmpLogger.messages() << endl;
  tmpLogger.clearLog();
}

typedef void (*testFunc)();

testFunc tests[] =
{
  test1
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
