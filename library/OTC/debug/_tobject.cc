/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     debug/_tobject.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1997-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/debug/tobject.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/debug/logger.hh>

#include <exception>
using std::set_terminate;

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

class TST_LetMeDie {};

class TST_Class : public OTC_TObject
{
  public:

			TST_Class(int anId)
			 : id_(anId) {}

			~TST_Class();

  protected:

    void		cleanup();

  private:

			TST_Class(TST_Class const&);

    int			id_;
};

TST_Class::~TST_Class()
{
  // Nothing to do.
}

void TST_Class::cleanup()
{
  OTC_Tracer tracer("void TST_Class::cleanup()");
  tracer() << "id = " << id_ << endl;
}

void test_terminate_function()
{
  OTC_TObject::terminateAll();
  OTC_Logger::notify(OTCLIB_LOG_ALERT,"Program terminating");
}

// = TEST 1
//     Test calls to terminate classes.

void test1()
{
  OTC_Tracer tracer("void test1()");

  TST_Class test1(1);
  TST_Class test2(2);
  TST_Class test3(3);

  throw TST_LetMeDie();

  tracer() << "ouch, shouldn't get here" << endl;
}

// = TEST 2
//     Test unlinking of terminate classes.

void test2()
{
  OTC_Tracer tracer("void test2()");

  TST_Class* test1 = new TST_Class(1);
  TST_Class* test2 = new TST_Class(2);
  TST_Class* test3 = new TST_Class(3);
  TST_Class* test4 = new TST_Class(4);
  TST_Class* test5 = new TST_Class(5);
  TST_Class* test6 = new TST_Class(6);

  delete test1;
  delete test3;
  delete test6;

  throw TST_LetMeDie();

  tracer() << "ouch, shouldn't get here" << endl;
}

// = TEST 3
//     Test calls to terminate function which doesn't exit.

void test3()
{
  OTC_Tracer tracer("void test3()");

  set_terminate(test_terminate_function);

  TST_Class test1(1);
  TST_Class test2(2);
  TST_Class test3(3);

  throw TST_LetMeDie();

  tracer() << "ouch, shouldn't get here" << endl;
}

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

  set_terminate(otclib_terminate_function);

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
