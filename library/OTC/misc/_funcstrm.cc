/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     misc/_funcstrm.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1997-2001 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/misc/funcstrm.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

void dump(char const* theBuffer, size_t theSize)
{
  cerr << "(";
  cerr.write(theBuffer,theSize);
  cerr << ")";
  cerr << flush;
}

void test1()
{
  char buf[128];
  OTC_FuncStream out(dump,buf,sizeof(buf));

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
  out << "123456789012345678901234567890123456789012345678901234567890";
  out << "123456789012345678901234567890123456789012345678901234567890";
  out << "123456789012345678901234567890123456789012345678901234567890";
  out << "123456789012345678901234567890123456789012345678901234567890";
  out << "123456789012345678901234567890123456789012345678901234567890";
  out << "123456789012345678901234567890123456789012345678901234567890";
  out << "123456789012345678901234567890123456789012345678901234567890";
  out << "123456789012345678901234567890123456789012345678901234567890";
  out << "123456789012345678901234567890123456789012345678901234567890";
  out << "123456789012345678901234567890123456789012345678901234567890";
  out << "123456789012345678901234567890123456789012345678901234567890";
  out << "123456789012345678901234567890123456789012345678901234567890";
  out << "123456789012345678901234567890123456789012345678901234567890";
  out << "123456789012345678901234567890123456789012345678901234567890";
  out << "123456789012345678901234567890123456789012345678901234567890";
  out << "123456789012345678901234567890123456789012345678901234567890";
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
