/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     text/_symbol.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//     Copyright 1996-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/text/symbol.hh>
#include <OTC/collctn/deque.hh>
#include <OTC/debug/tracer.hh>

#include <OTC/stdlib/fstream.h>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

void test1()
{
  OTC_Tracer tracer("void test1()");

  OTC_Symbol sym2 = "two";
  OTC_Symbol sym3 = "three";

  tracer() << "sym2(" << int(sym2.string()) << ") = " << sym2 << endl;
  tracer() << "sym3(" << int(sym3.string()) << ") = " << sym3 << endl;

  tracer() << "exists(\"one\") = " << OTC_Symbol::exists("one") << endl;
  tracer() << "exists(\"two\") = " << OTC_Symbol::exists("two") << endl;
  tracer() << "exists(\"three\") = " << OTC_Symbol::exists("three") << endl;
  tracer() << "exists(\"four\") = " << OTC_Symbol::exists("four") << endl;

  tracer() << "(sym2 == \"two\") = " << (sym2 == "two") << endl;
  tracer() << "(sym2 != \"two\") = " << (sym2 != "two") << endl;
  tracer() << "(\"two\" == sym2) = " << ("two" == sym2) << endl;
  tracer() << "(\"two\" != sym2) = " << ("two" != sym2) << endl;

  OTC_String str1 = "eno";
  tracer() << "str1 = \"eno\"" << endl;

  tracer() << "(sym2 == str1) = " << (sym2 == str1) << endl;
  tracer() << "(sym2 != str1) = " << (sym2 != str1) << endl;
  tracer() << "(str1 == sym2) = " << (str1 == sym2) << endl;
  tracer() << "(str1 != sym2) = " << (str1 != sym2) << endl;
}

void test2()
{
  OTC_Tracer tracer("void test2()");

  char buf[1024];
  ifstream in("/usr/dict/words");

  OTC_Deque<OTC_Symbol> theList;

  OTC_Symbol s1;

  while (in.good())
  {
    in >> buf;
    if (!in.fail())
      theList.addLast(buf);
  }

  cout << "loaded" << endl;
  cout << "> " << flush;

  OTC_String theString;
  while (cin.good())
  {
    OTC_String::getline(theString,cin);
    if (!cin.fail())
    {
      if (!OTC_Symbol::exists(theString))
      {
	cout << "create --> " << flush;
	theList.addLast(theString);
      }

      s1 = theString;
      theString = s1;
      s1 = theString;
      cout << s1 << " = " << int(s1.string()) << endl;
      cout << "> " << flush;
      theString.truncate();
    }
  }
}

void test3()
{
  OTC_Tracer tracer("void test3()");

  OTC_Symbol theSymbol = "";

  cout << "\"" << theSymbol << "\"" << endl;
}

void test4()
{
  OTC_Tracer tracer("void test4()");

  OTC_Symbol theSymbol = OTC_Symbol::nullSymbol();

  cout << "\"" << theSymbol << "\"" << endl;
}

void test5()
{
  OTC_Tracer tracer("void test5()");

  int i = 0;
  char buf[256];

  for (i=0; i<10000000; i++)
  {
    sprintf(buf,"%010d",i);

    OTC_Symbol theSymbol(buf);
  }
}

void test6()
{
  OTC_Tracer tracer("void test6()");

  int i = 0;
  char buf[256];

  for (i=0; i<10000000; i++)
  {
    sprintf(buf,"%010d",i);

    OTC_Symbol theSymbol(buf);
    OTC_String theString(theSymbol);
  }
}

void test7()
{
  OTC_Tracer tracer("void test7()");

  int i = 0;
  char buf[256];

  for (i=0; i<10000000; i++)
  {
    sprintf(buf,"%010d",i);

    OTC_Symbol theSymbol;
    theSymbol = buf;

    OTC_String theString(theSymbol);
  }
}

void test8()
{
  OTC_Tracer tracer("void test8()");

  int i = 0;
  char buf[256];

  for (i=0; i<10000000; i++)
  {
    sprintf(buf,"%010d",i);

    OTC_Symbol theSymbol;
    theSymbol = buf;

    OTC_String theString;
    theString = theSymbol;
  }
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
