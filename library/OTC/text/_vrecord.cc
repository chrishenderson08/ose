/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     text/_vrecord.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1997-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/text/vrecord.hh>

#include <OTC/text/string.hh>
#include <OTC/debug/tracer.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

void test1()
{
  OTC_Tracer tracer("void test1()");

  char theDelimiter;

  cin >> theDelimiter;
  cin >> ws;

  OTC_String theString;
  theString.readLine(cin);

  OTC_VRecord theRecord(
   theString.string(),theString.length(),theDelimiter);

  tracer() << "numFields = " << theRecord.numFields() << endl;

  tracer() << endl;

  u_int i;

  for (i=1; i<=theRecord.numFields(); i++)
  {
    tracer() << "field[" << i << "] = ";
    tracer() << theRecord.length(i) << " \"";
    tracer() << theRecord.field(i) << "\"" << endl;
  }

  OTC_VRecord tmpRecord;

  tmpRecord = theRecord;
  while (tmpRecord.numFields() > 1)
  {
    tracer() << endl;

    tmpRecord = tmpRecord.slice(1,tmpRecord.numFields()-1);
    for (i=1; i<=tmpRecord.numFields(); i++)
    {
      tracer() << "field[" << i << "] = ";
      tracer() << tmpRecord.length(i) << " \"";
      tracer() << tmpRecord.field(i) << "\"" << endl;
    }
  }

  u_int theOffset = 0;

  tmpRecord = theRecord;
  while (tmpRecord.numFields() > 1)
  {
    tracer() << endl;

    theOffset++;

    tmpRecord = tmpRecord.slice(2,tmpRecord.numFields()-1);
    for (i=1; i<=tmpRecord.numFields(); i++)
    {
      tracer() << "field[" << i << "] = ";
      tracer() << tmpRecord.length(i) << " \"";
      tracer() << tmpRecord.field(i) << "\"" << endl;
    }
  }
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
