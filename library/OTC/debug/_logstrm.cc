/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     debug/_logstrm.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//     Copyright 1999-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/debug/logstrm.hh>
#include <OTC/debug/tracer.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

void test1()
{
  OTC_Tracer tracer("void test1()");

  OTC_LogStream out;
  out << "message 1" << endl;
  out << "message 2" << "\n";
  out << "message 3" << endl;
  out << "message 4" << endl;
  out << "message 5" << flush;
  out << "message 6" << endl;
  out << "123456789012345678901234567890123456789012345678901234567890";
  out << "123456789012345678901234567890123456789012345678901234567890";
  out << "123456789012345678901234567890123456789012345678901234567890";
  out << "123456789012345678901234567890123456789012345678901234567890";
}

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTCLIB_LOGGER(OTCLIB_LOG_EMERGENCY) << "OTCLIB_LOG_EMERGENCY" << endl;
  OTCLIB_LOGGER(OTCLIB_LOG_ALERT) << "OTCLIB_LOG_ALERT" << endl;
  OTCLIB_LOGGER(OTCLIB_LOG_CRITICAL) << "OTCLIB_LOG_CRITICAL" << endl;
  OTCLIB_LOGGER(OTCLIB_LOG_ERROR) << "OTCLIB_LOG_ERROR" << endl;
  OTCLIB_LOGGER(OTCLIB_LOG_WARNING) << "OTCLIB_LOG_WARNING" << endl;
  OTCLIB_LOGGER(OTCLIB_LOG_NOTICE) << "OTCLIB_LOG_NOTICE" << endl;
  OTCLIB_LOGGER(OTCLIB_LOG_INFO) << "OTCLIB_LOG_INFO" << endl;
  OTCLIB_LOGGER(OTCLIB_LOG_DEBUG) << "OTCLIB_LOG_DEBUG" << endl;
}

typedef void (*testFunc)();

testFunc tests[] =
{
  test1,
  test2
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
