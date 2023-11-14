/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     debug/_logger.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992 OTC LIMITED
//     Copyright 1999-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/debug/logger.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/OTC.h>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

class TST_Logger : public OTC_Logger
{
  public:

			~TST_Logger();

			TST_Logger() {}

  protected:

    void		log(
			 char const* theChannel,
			 OTC_LogLevel theLevel,
			 char const* theMessage,
			 u_int theLength
			);

  private:

			TST_Logger(TST_Logger const&);
};

TST_Logger::~TST_Logger()
{
  // Nothing to do.
}

void TST_Logger::log(
 char const* theChannel,
 OTC_LogLevel theLevel,
 char const* theMessage,
 u_int
)
{
  OTC_Tracer tracer("void TST_Logger::log()");

  if (theChannel == 0)
    theChannel = "<void>";

  tracer() << "channel = " << theChannel << endl;
  tracer() << "level = " << OTCLIB_LOGLEVELMESSAGES[theLevel] << endl;
  tracer() << "message = " << theMessage << endl;
}

// = TEST 1
//     Test of output to logger with no user supplied loggers.

void test1()
{
  OTC_Tracer tracer("void test1()");

  OTC_Logger::notify(OTCLIB_LOG_EMERGENCY,"EMERGENCY level");
  OTC_Logger::notify(OTCLIB_LOG_ALERT,"ALERT level");
  OTC_Logger::notify(OTCLIB_LOG_CRITICAL,"CRITICAL level");
  OTC_Logger::notify(OTCLIB_LOG_ERROR,"ERROR level");
  OTC_Logger::notify(OTCLIB_LOG_WARNING,"WARNING level");
  OTC_Logger::notify(OTCLIB_LOG_NOTICE,"NOTICE level");
  OTC_Logger::notify(OTCLIB_LOG_INFO,"INFO level");
  OTC_Logger::notify(OTCLIB_LOG_DEBUG,"DEBUG level");
}

// = TEST 2
//     Test of output of logger with a user supplied logger.

void test2()
{
  OTC_Tracer tracer("void test2()");

  TST_Logger logger;

  OTC_Logger::notify(OTCLIB_LOG_EMERGENCY,"EMERGENCY level");
  OTC_Logger::notify(OTCLIB_LOG_ALERT,"ALERT level");
  OTC_Logger::notify(OTCLIB_LOG_CRITICAL,"CRITICAL level");
  OTC_Logger::notify(OTCLIB_LOG_ERROR,"ERROR level");
  OTC_Logger::notify(OTCLIB_LOG_WARNING,"WARNING level");
  OTC_Logger::notify(OTCLIB_LOG_NOTICE,"NOTICE level");
  OTC_Logger::notify(OTCLIB_LOG_INFO,"INFO level");
}

// = TEST 3
//     Test of output with embedded newlines.

void test3()
{
  OTC_Tracer tracer("void test3()");

  TST_Logger logger;

  OTC_Logger::notify(OTCLIB_LOG_INFO,"01234\n56789\n01234\n56789");
}

// = TEST 4
//     Test of channel names.

void test4()
{
  OTC_Tracer tracer("void test4()");

  TST_Logger logger;

  OTC_Logger::notify("CHANNEL",OTCLIB_LOG_INFO,"CHANNEL");

  OTC_Logger::notify("@ CHANNEL",OTCLIB_LOG_INFO,"@-CHANNEL");
  OTC_Logger::notify("1 CHANNEL",OTCLIB_LOG_INFO,"1 CHANNEL");
  OTC_Logger::notify("# CHANNEL",OTCLIB_LOG_INFO,"# CHANNEL");
  OTC_Logger::notify("  CHANNEL",OTCLIB_LOG_INFO,"  CHANNEL");
  OTC_Logger::notify("_ CHANNEL",OTCLIB_LOG_INFO,"_ CHANNEL");
}

typedef void (*testFunc)();

testFunc tests[] =
{
  test1,
  test2,
  test3,
  test4
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
