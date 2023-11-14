/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/_bndstack.cc
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

#include <OTC/collctn/bndstack.hh>
#include <OTC/debug/tracer.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

// = TEST 2
//     Test of empty stack.

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTC_BoundedStack<int> stack1(0);
  tracer() << "stack1.isEmpty() = " << stack1.isEmpty() << endl;
  tracer() << "stack1.isFull() = " << stack1.isFull() << endl;

  OTC_BoundedStack<int> stack2(1);
  tracer() << "stack2.isEmpty() = " << stack2.isEmpty() << endl;
  tracer() << "stack2.isFull() = " << stack2.isFull() << endl;

  OTC_BoundedStack<int> stack3(2);
  tracer() << "stack3.isEmpty() = " << stack3.isEmpty() << endl;
  tracer() << "stack3.isFull() = " << stack3.isFull() << endl;
}

// = TEST 3
//     Test of filling stack and then emptying it.

void test3()
{
  OTC_Tracer tracer("void test3()");

  OTC_BoundedStack<int> stack1(10);
  int i=0;
  tracer() << "stack1.push() =" << flush;
  while (!stack1.isFull())
  {
    tracer() << " " << i << flush;
    stack1.push(i);
    if (stack1.isEmpty())
      tracer() << "XXX - this should never happen" << endl;
    i++;
  }
  tracer() << endl;

  tracer() << "stack1.pop() =" << flush;
  while (!stack1.isEmpty())
  {
    int top = stack1.top();
    i = stack1.pop();
    if (top != i)
      tracer() << "XXX - this should never happen" << endl;
    tracer() << " " << i << flush;
    if (stack1.isFull())
      tracer() << "XXX - this should never happen" << endl;
  }
  tracer() << endl;
}

// = TEST 4
//     Test of count() and clear().

void test4()
{
  OTC_Tracer tracer("void test4()");

  OTC_BoundedStack<int> stack1(10);

  tracer() << "stack1.capacity() = " << stack1.capacity() << endl;

  int i=0;
  tracer() << "stack1.push() =" << flush;
  while (!stack1.isFull())
  {
    tracer() << " " << i << flush;
    stack1.push(i);
    i++;
  }
  tracer() << endl;

  tracer() << "stack1.count() = " << stack1.count() << endl;

  tracer() << "stack1.clear()" << endl;
  stack1.clear();

  tracer() << "stack1.count() = " << stack1.count() << endl;
}

// = TEST 5
//     Test of removing set number of items from stack.

void test5()
{
  OTC_Tracer tracer("void test5()");

  OTC_BoundedStack<int> stack1(10);

  tracer() << "stack1.capacity() = " << stack1.capacity() << endl;

  int i=0;
  tracer() << "stack1.push() =" << flush;
  while (!stack1.isFull())
  {
    tracer() << " " << i << flush;
    stack1.push(i);
    i++;
  }
  tracer() << endl;

  tracer() << "stack1.count() = " << stack1.count() << endl;

  tracer() << "stack1.discard(4)" << endl;
  stack1.discard(4);

  tracer() << "stack1.count() = " << stack1.count() << endl;

  tracer() << "stack1.pop() = " << stack1.pop() << endl;
  tracer() << "stack1.pop() = " << stack1.pop() << endl;

  tracer() << "stack1.discard(4)" << endl;
  stack1.discard(4);

  tracer() << "stack1.count() = " << stack1.count() << endl;
}

// = TEST 6
//     Test of array being on the stack.

void test6()
{
  OTC_Tracer tracer("void test6()");

  int array[10];
  OTC_BoundedStack<int> stack1(10,array);

  int i=0;
  tracer() << "stack1.push() =" << flush;
  while (!stack1.isFull())
  {
    tracer() << " " << i << flush;
    stack1.push(i);
    i++;
  }
  tracer() << endl;

  tracer() << "stack1.pop() =" << flush;
  while (!stack1.isEmpty())
  {
    i = stack1.pop();
    tracer() << " " << i << flush;
  }
  tracer() << endl;
}

// = TEST 7
//     Test of peek().

void test7()
{
  OTC_Tracer tracer("void test7()");

  int array[10];
  OTC_BoundedStack<int> stack1(10,array);

  int i=0;
  tracer() << "stack1.push() =" << flush;
  while (!stack1.isFull())
  {
    tracer() << " " << i << flush;
    stack1.push(i);
    i++;
  }
  tracer() << endl;

  for (i=0; i<10; i++)
    tracer() << "stack1.peek(" << i << ") = " << stack1.peek(i) << endl;
}

// = TEST 8
//     Test of copy constructor.

void test8()
{
  OTC_Tracer tracer("void test8()");

  OTC_BoundedStack<int> stack1(10);
  int i=0;
  tracer() << "stack1.push() =" << flush;
  while (!stack1.isFull())
  {
    tracer() << " " << i << flush;
    stack1.push(i);
    if (stack1.isEmpty())
      tracer() << "XXX - this should never happen" << endl;
    i++;
  }
  tracer() << endl;

  OTC_BoundedStack<int> stack2 = stack1;

  tracer() << "stack1.pop() =" << flush;
  while (!stack1.isEmpty())
  {
    int top = stack1.top();
    i = stack1.pop();
    if (top != i)
      tracer() << "XXX - this should never happen" << endl;
    tracer() << " " << i << flush;
    if (stack1.isFull())
      tracer() << "XXX - this should never happen" << endl;
  }
  tracer() << endl;

  tracer() << "stack2.pop() =" << flush;
  while (!stack2.isEmpty())
  {
    int top = stack2.top();
    i = stack2.pop();
    if (top != i)
      tracer() << "XXX - this should never happen" << endl;
    tracer() << " " << i << flush;
    if (stack2.isFull())
      tracer() << "XXX - this should never happen" << endl;
  }
  tracer() << endl;
}

// = TEST 9
//     Test of assignment.

void test9()
{
  OTC_Tracer tracer("void test9()");

  OTC_BoundedStack<int> stack1(10);
  int i=0;
  tracer() << "stack1.push() =" << flush;
  while (!stack1.isFull())
  {
    tracer() << " " << i << flush;
    stack1.push(i);
    if (stack1.isEmpty())
      tracer() << "XXX - this should never happen" << endl;
    i++;
  }
  tracer() << endl;

  OTC_BoundedStack<int> stack2(10);
  stack2 = stack1;

  tracer() << "stack1.pop() =" << flush;
  while (!stack1.isEmpty())
  {
    int top = stack1.top();
    i = stack1.pop();
    if (top != i)
      tracer() << "XXX - this should never happen" << endl;
    tracer() << " " << i << flush;
    if (stack1.isFull())
      tracer() << "XXX - this should never happen" << endl;
  }
  tracer() << endl;

  tracer() << "stack2.pop() =" << flush;
  while (!stack2.isEmpty())
  {
    int top = stack2.top();
    i = stack2.pop();
    if (top != i)
      tracer() << "XXX - this should never happen" << endl;
    tracer() << " " << i << flush;
    if (stack2.isFull())
      tracer() << "XXX - this should never happen" << endl;
  }
  tracer() << endl;
}

// = TEST 1
//     All tests

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
  test9();
}

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
