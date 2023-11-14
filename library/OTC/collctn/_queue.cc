/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/_queue.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992 OTC LIMITED
//     Copyright 1997-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/queue.hh>
#include <OTC/debug/logger.hh>
#include <OTC/debug/tracer.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

// = TEST 2
//      Test of empty queue.

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTC_Queue<int> queue;

  tracer() << "queue.isEmpty() = " << queue.isEmpty() << endl;
}

// = TEST 3
//     Test addition and removal of items from queue.

void test3()
{
  OTC_Tracer tracer("void test3()");

  OTC_Queue<int> queue;

  for (int i=0; i<10; i++)
    queue.add(i);

  tracer() << "queue.isEmpty() = " << queue.isEmpty() << endl;

  tracer() << "queue.head()/remove() =" << flush;
  while (!queue.isEmpty())
  {
    // XXX - if we call these in place on a Pyramid they are evaluated
    // in the wrong order. Ouch.
    int j = queue.head();
    int k = queue.remove();
    tracer() << " " << j << "/" << k << flush;
  }
  tracer() << endl;
}

// = TEST 4
//     Test of removing item from empty queue.

void test4()
{
  OTC_Tracer tracer("void test4()");

  OTC_Logger::notify(OTCLIB_LOG_INFO,"This test is meant to fail");

  OTC_Queue<int> queue;

  tracer() << "queue.remove()" << endl;
  queue.remove();

  tracer() << "XXX - shouldn't reach here" << endl;
}

// = TEST 5
//     Test of looking at first item on empty queue.

void test5()
{
  OTC_Tracer tracer("void test5()");

  OTC_Logger::notify(OTCLIB_LOG_INFO,"This test is meant to fail");

  OTC_Queue<int> queue;

  tracer() << "queue.head()" << endl;
  queue.head();

  tracer() << "XXX - shouldn't reach here" << endl;
}

// = TEST 6
//     Test of count() and clear().

void test6()
{
  OTC_Tracer tracer("void test6()");

  OTC_Queue<int> queue;

  tracer() << "queue.count() = " << queue.count() << endl;

  for (int i=0; i<10; i++)
    queue.add(i);

  tracer() << "queue.count() = " << queue.count() << endl;

  tracer() << "queue.clear()" << endl;
  queue.clear();

  for (int j=0; j<10; j++)
    queue.add(j);

  tracer() << "queue.count() = " << queue.count() << endl;
}

// = TEST 7
//     Test of removing set number of items from queue.

void test7()
{
  OTC_Tracer tracer("void test7()");

  OTC_Queue<int> queue;

  tracer() << "queue.count() = " << queue.count() << endl;

  tracer() << "queue =" << flush;
  for (int i=0; i<10; i++)
  {
    tracer() << " " << i << flush;
    queue.add(i);
  }
  tracer() << endl;

  tracer() << "queue.count() = " << queue.count() << endl;

  tracer() << "queue.discard(4)" << endl;
  queue.discard(4);

  tracer() << "queue.count() = " << queue.count() << endl;

  tracer() << "queue.remove() = " << queue.remove() << endl;
  tracer() << "queue.remove() = " << queue.remove() << endl;

  tracer() << "queue.count() = " << queue.count() << endl;

  tracer() << "queue.discard(4)" << endl;
  queue.discard(4);

  tracer() << "queue.count() = " << queue.count() << endl;
}

// = TEST 8
//     Test of copy constructor.

void test8()
{
  OTC_Tracer tracer("void test8()");

  OTC_Queue<int> queue1;

  for (int i=0; i<10; i++)
    queue1.add(i);

  OTC_Queue<int> queue2 = queue1;

  tracer() << "queue1.remove() =" << flush;
  while (!queue1.isEmpty())
    tracer() << " " << queue1.remove() << flush;
  tracer() << endl;

  tracer() << "queue2.remove() =" << flush;
  while (!queue2.isEmpty())
    tracer() << " " << queue2.remove() << flush;
  tracer() << endl;
}

// = TEST 9
//     Test of assignment.

void test9()
{
  OTC_Tracer tracer("void test9()");

  OTC_Queue<int> queue1;

  for (int i=0; i<10; i++)
    queue1.add(i);

  OTC_Queue<int> queue2;
  queue2 = queue1;

  tracer() << "queue1.remove() =" << flush;
  while (!queue1.isEmpty())
    tracer() << " " << queue1.remove() << flush;
  tracer() << endl;

  tracer() << "queue2.remove() =" << flush;
  while (!queue2.isEmpty())
    tracer() << " " << queue2.remove() << flush;
  tracer() << endl;
}

// = TEST 1
//     All tests.

void test1()
{
  OTC_Tracer tracer("void test1()");

  test2();
  test3();
  test6();
  test7();
  test8();
  test9();
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
  test8,
  test9
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
