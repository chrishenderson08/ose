/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/_stack.cc
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

#include <OTC/collctn/stack.hh>
#include <OTC/debug/logger.hh>
#include <OTC/debug/tracer.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

// = TEST 2
//      Test of empty stack.

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTC_Stack<int> stack;

  tracer() << "stack.isEmpty() = " << stack.isEmpty() << endl;
}

// = TEST 3
//     Test of stack operations.

void test3()
{
  OTC_Tracer tracer("void test3()");

  OTC_Stack<int> stack;

  for (int i=0; i<10; i++)
    stack.push(i);

  tracer() << "stack.isEmpty() = " << stack.isEmpty() << endl;

  tracer() << "stack.top()/pop() =" << flush;
  while (!stack.isEmpty())
  {
    // XXX - if we call these in place on a Pyramid they are evaluated
    // in the wrong order. Ouch.
    int j = stack.top();
    int k = stack.pop();
    tracer() << " " << j << "/" << k << flush;
  }
  tracer() << endl;
}

// = TEST 4
//     Test of popping item off empty stack.

void test4()
{
  OTC_Tracer tracer("void test4()");

  OTC_Logger::notify(OTCLIB_LOG_INFO,"This test is meant to fail");

  OTC_Stack<int> stack;

  tracer() << "stack.pop()" << endl;
  stack.pop();

  tracer() << "XXX - shouldn't reach here" << endl;
}

// = TEST 5
//     Test of looking at top item on empty stack.

void test5()
{
  OTC_Tracer tracer("void test5()");

  OTC_Logger::notify(OTCLIB_LOG_INFO,"This test is meant to fail");

  OTC_Stack<int> stack;

  tracer() << "stack.top()" << endl;
  stack.top();

  tracer() << "XXX - shouldn't reach here" << endl;
}

// = TEST 6
//     Test use of count() and clear().

void test6()
{
  OTC_Tracer tracer("void test6()");

  OTC_Stack<int> stack;

  for (int i=0; i<10; i++)
    stack.push(i);

  tracer() << "stack.count() = " << stack.count() << endl;

  tracer() << "stack.clear()" << endl;
  stack.clear();

  tracer() << "stack.count() = " << stack.count() << endl;

  for (int j=0; j<10; j++)
    stack.push(j);

  tracer() << "stack.count() = " << stack.count() << endl;
}

// = TEST 7
//     Test of removing set number of items.

void test7()
{
  OTC_Tracer tracer("void test7()");

  OTC_Stack<int> stack;

  tracer() << "stack =" << flush;
  for (int i=0; i<10; i++)
  {
    tracer() << " " << i << flush;
    stack.push(i);
  }
  tracer() << endl;

  tracer() << "stack.count() = " << stack.count() << endl;

  tracer() << "stack.discard(4)" << endl;
  stack.discard(4);

  tracer() << "stack.count() = " << stack.count() << endl;

  tracer() << "stack.pop() = " << stack.pop() << endl;
  tracer() << "stack.pop() = " << stack.pop() << endl;

  tracer() << "stack.count() = " << stack.count() << endl;

  tracer() << "stack.discard(4)" << endl;
  stack.discard(4);

  tracer() << "stack.count() = " << stack.count() << endl;
}

// = TEST 8
//     Test of copy constructor.

void test8()
{
  OTC_Tracer tracer("void test8()");

  OTC_Stack<int> stack1;

  for (int i=0; i<10; i++)
    stack1.push(i);

  OTC_Stack<int> stack2 = stack1;

  tracer() << "stack1.pop() =" << flush;
  while (!stack1.isEmpty())
    tracer() << " " << stack1.pop() << flush;
  tracer() << endl;

  tracer() << "stack2.pop() =" << flush;
  while (!stack2.isEmpty())
    tracer() << " " << stack2.pop() << flush;
  tracer() << endl;
}

// = TEST 9
//     Test of assignment.

void test9()
{
  OTC_Tracer tracer("void test9()");

  OTC_Stack<int> stack1;

  for (int i=0; i<10; i++)
    stack1.push(i);

  OTC_Stack<int> stack2;
  stack2 = stack1;

  tracer() << "stack1.pop() =" << flush;
  while (!stack1.isEmpty())
    tracer() << " " << stack1.pop() << flush;
  tracer() << endl;

  tracer() << "stack2.pop() =" << flush;
  while (!stack2.isEmpty())
    tracer() << " " << stack2.pop() << flush;
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
