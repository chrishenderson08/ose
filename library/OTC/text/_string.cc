/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     text/_string.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1993-1994 TELSTRA CORPORATION LIMITED
//     Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/text/string.hh>
#include <OTC/text/symbol.hh>
#include <OTC/debug/tracer.hh>

#include <OTC/stdlib/strstream.h>

#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------------- */

// = TEST 2
//     Test creation and properties of an empty string.

void test2()
{
  OTC_Tracer tracer("void test2()");

  tracer() << "string1" << endl;
  OTC_String string1;

  tracer() << "string1.length() = " << flush;
  tracer() << string1.length() << endl;

  tracer() << "string1.isEmpty() = " << flush;
  tracer() << string1.isEmpty() << endl;

  tracer() << "(string1.string() == \"\") = " << flush;
  tracer() << !(*string1.string()) << endl;

  tracer() << "(string1.buffer() == 0) = " << flush;
  tracer() << !(string1.buffer()) << endl;

  tracer() << "string1.hash() = " << flush;
  tracer() << OTC_HashActions<OTC_String>::hash(string1) << endl;

  tracer() << "string1 = " << flush;
  tracer() << string1 << endl;
}

// = TEST 3
//     Test creation of string from null terminated string.

void test3()
{
  OTC_Tracer tracer("void test3()");

  tracer() << "string1(0)" << endl;
  OTC_String string1((char const*)0);

  tracer() << "string1.length() = " << flush;
  tracer() << string1.length() << endl;

  tracer() << "string1.isEmpty() = " << flush;
  tracer() << string1.isEmpty() << endl;

  tracer() << "(string1.string() == \"\") = " << flush;
  tracer() << !(*string1.string()) << endl;

  tracer() << "string1.hash() = " << flush;
  tracer() << OTC_HashActions<OTC_String>::hash(string1) << endl;

  tracer() << "string1 = " << flush;
  tracer() << string1 << endl;

  tracer() << endl;

  tracer() << "string2(\"\")" << endl;
  OTC_String string2("");

  tracer() << "string2.length() = " << flush;
  tracer() << string2.length() << endl;

  tracer() << "string2.isEmpty() = ";
  tracer() << string2.isEmpty() << endl;

  tracer() << "(string2.string() == \"\") = " << flush;
  tracer() << !(*string2.string()) << endl;

  tracer() << "string2.hash() = " << flush;
  tracer() << OTC_HashActions<OTC_String>::hash(string2) << endl;

  tracer() << "string2 = " << flush;
  tracer() << string2 << endl;

  tracer() << endl;

  tracer() << "string3(\"1234567\")" << endl;
  OTC_String string3("1234567");

  tracer() << "string3.length() = " << flush;
  tracer() << string3.length() << endl;

  tracer() << "string3.isEmpty() = ";
  tracer() << string3.isEmpty() << endl;

  tracer() << "string3.hash() = " << flush;
  tracer() << OTC_HashActions<OTC_String>::hash(string3) << endl;

  tracer() << "string3 = " << flush;
  tracer() << string3 << endl;
}

// = TEST 4
//     Test creation from string and character count.

void test4()
{
  OTC_Tracer tracer("void test4()");

  tracer() << "string1(0,0)" << endl;
  OTC_String string1((char const*)0,0);

  tracer() << "string1.length() = " << flush;
  tracer() << string1.length() << endl;

  tracer() << "string1.isEmpty() = " << flush;
  tracer() << string1.isEmpty() << endl;

  tracer() << "(string1.string() == \"\") = " << flush;
  tracer() << !(*string1.string()) << endl;

  tracer() << "string1.hash() = " << flush;
  tracer() << OTC_HashActions<OTC_String>::hash(string1) << endl;

  tracer() << "string1 = " << flush;
  tracer() << string1 << endl;

  tracer() << endl;

  tracer() << "string2(\"\",0)" << endl;
  OTC_String string2("",0);

  tracer() << "string2.length() = " << flush;
  tracer() << string2.length() << endl;

  tracer() << "string2.isEmpty() = ";
  tracer() << string2.isEmpty() << endl;

  tracer() << "(string2.string() == \"\") = " << flush;
  tracer() << !(*string2.string()) << endl;

  tracer() << "string2.hash() = " << flush;
  tracer() << OTC_HashActions<OTC_String>::hash(string2) << endl;

  tracer() << "string2 = " << flush;
  tracer() << string2 << endl;

  tracer() << endl;

  tracer() << "string3(\"1234567\",7)" << endl;
  OTC_String string3("1234567",7);

  tracer() << "string3.length() = " << flush;
  tracer() << string3.length() << endl;

  tracer() << "string3.isEmpty() = ";
  tracer() << string3.isEmpty() << endl;

  tracer() << "string3.hash() = " << flush;
  tracer() << OTC_HashActions<OTC_String>::hash(string3) << endl;

  tracer() << "string3 = " << flush;
  tracer() << string3 << endl;

  tracer() << endl;

  tracer() << "string4(\"1234567\\0890\",10)" << endl;
  char buf1[16];
  strcpy(&buf1[0],"1234567");
  strcpy(&buf1[8],"890");
  OTC_String string4(buf1,10);

  tracer() << "string4.length() = " << flush;
  tracer() << string4.length() << endl;

  tracer() << "string4.isEmpty() = ";
  tracer() << string4.isEmpty() << endl;

  tracer() << "string4.hash() = " << flush;
  tracer() << OTC_HashActions<OTC_String>::hash(string4) << endl;

  tracer() << "string4 = " << flush;
  tracer() << string4 << endl;
}

// = TEST 5
//     Test creation from single character and count.

void test5()
{
  OTC_Tracer tracer("void test5()");

  tracer() << "string1('1',0)" << endl;
  OTC_String string1('1',0);

  tracer() << "string1.length() = " << flush;
  tracer() << string1.length() << endl;

  tracer() << "string1.isEmpty() = " << flush;
  tracer() << string1.isEmpty() << endl;

  tracer() << "string1.hash() = " << flush;
  tracer() << OTC_HashActions<OTC_String>::hash(string1) << endl;

  tracer() << "string1 = " << flush;
  tracer() << string1 << endl;

  tracer() << endl;

  tracer() << "string2('1',7)" << endl;
  OTC_String string2('1',7);

  tracer() << "string2.length() = " << flush;
  tracer() << string2.length() << endl;

  tracer() << "string2.isEmpty() = ";
  tracer() << string2.isEmpty() << endl;

  tracer() << "string2.hash() = " << flush;
  tracer() << OTC_HashActions<OTC_String>::hash(string2) << endl;

  tracer() << "string2 = " << flush;
  tracer() << string2 << endl;

  tracer() << endl;

  tracer() << "string3(EOS,1)" << endl;
  OTC_String string3(EOS,1);

  tracer() << "string3.length() = " << flush;
  tracer() << string3.length() << endl;

  tracer() << "string3.isEmpty() = ";
  tracer() << string3.isEmpty() << endl;

  tracer() << "string3.hash() = " << flush;
  tracer() << OTC_HashActions<OTC_String>::hash(string3) << endl;

  tracer() << "string3 = " << flush;
  tracer() << string3 << endl;

  tracer() << endl;
}

// = TEST 6
//     Test creation from a string.

void test6()
{
  OTC_Tracer tracer("void test6()");

  tracer() << "string1(\"1234567\")" << endl;
  OTC_String string1("1234567");

  tracer() << "string1.length() = " << flush;
  tracer() << string1.length() << endl;

  tracer() << "string1.isEmpty() = " << flush;
  tracer() << string1.isEmpty() << endl;

  tracer() << "string1.hash() = " << flush;
  tracer() << OTC_HashActions<OTC_String>::hash(string1) << endl;

  tracer() << "string1 = " << flush;
  tracer() << string1 << endl;

  tracer() << endl;

  tracer() << "string2(string1)" << endl;
  OTC_String string2(string1);

  tracer() << "string2.length() = " << flush;
  tracer() << string2.length() << endl;

  tracer() << "string2.isEmpty() = ";
  tracer() << string2.isEmpty() << endl;

  tracer() << "string2.hash() = " << flush;
  tracer() << OTC_HashActions<OTC_String>::hash(string2) << endl;

  tracer() << "string2 = " << flush;
  tracer() << string2 << endl;

  tracer() << "(string1 == string2) = " << flush;
  tracer() << ((char const*)string1 == (char const*)string2) << endl;
}

// = TEST 7
//     Test creation from string class and length.

void test7()
{
  OTC_Tracer tracer("void test7()");

  tracer() << "string1(\"1234567\")" << endl;
  OTC_String string1("1234567");

  tracer() << "string1.length() = " << flush;
  tracer() << string1.length() << endl;

  tracer() << "string1.isEmpty() = " << flush;
  tracer() << string1.isEmpty() << endl;

  tracer() << "(string1.string() == \"\") = " << flush;
  tracer() << !(*string1.string()) << endl;

  tracer() << "string1.hash() = " << flush;
  tracer() << OTC_HashActions<OTC_String>::hash(string1) << endl;

  tracer() << "string1 = " << flush;
  tracer() << string1 << endl;

  tracer() << endl;

  tracer() << "string2(string1,0)" << endl;
  OTC_String string2(string1,0);

  tracer() << "string2.length() = " << flush;
  tracer() << string2.length() << endl;

  tracer() << "string2.isEmpty() = ";
  tracer() << string2.isEmpty() << endl;

  tracer() << "(string2.string() == \"\") = " << flush;
  tracer() << !(*string2.string()) << endl;

  tracer() << "string2.hash() = " << flush;
  tracer() << OTC_HashActions<OTC_String>::hash(string2) << endl;

  tracer() << "string2 = " << flush;
  tracer() << string2 << endl;

  tracer() << endl;

  tracer() << "string3(string1,7)" << endl;
  OTC_String string3(string1,7);

  tracer() << "string3.length() = " << flush;
  tracer() << string3.length() << endl;

  tracer() << "string3.isEmpty() = ";
  tracer() << string3.isEmpty() << endl;

  tracer() << "string3.hash() = " << flush;
  tracer() << OTC_HashActions<OTC_String>::hash(string3) << endl;

  tracer() << "string3 = " << flush;
  tracer() << string3 << endl;

  tracer() << endl;

  tracer() << "string4(string1,5)" << endl;
  OTC_String string4(string1,5);

  tracer() << "string4.length() = " << flush;
  tracer() << string4.length() << endl;

  tracer() << "string4.isEmpty() = ";
  tracer() << string4.isEmpty() << endl;

  tracer() << "string4.hash() = " << flush;
  tracer() << OTC_HashActions<OTC_String>::hash(string4) << endl;

  tracer() << "string4 = " << flush;
  tracer() << string4 << endl;
}

// = TEST 8
//     Test output of string to a stream.

void test8()
{
  OTC_Tracer tracer("void test8()");

  tracer() << "string1(\"1234567\")" << endl;
  OTC_String string1("1234567");

  tracer() << "string1 = " << string1 << endl;

  tracer() << "ios width = 4" << endl;
  tracer() << "string1 = " << flush;
  tracer().width(4);
  tracer() << string1 << endl;

  tracer() << "ios width = 10" << endl;
  tracer() << "ios fill = ." << endl;
  tracer() << "ios justify = default" << endl;
  tracer() << "string1 = " << flush;
  tracer().width(10);
  tracer().fill('.');
  tracer().unsetf(ios::adjustfield);
  tracer() << string1 << endl;

  tracer() << "ios width = 10" << endl;
  tracer() << "ios fill = ." << endl;
  tracer() << "ios justify = left" << endl;
  tracer() << "string1 = " << flush;
  tracer().width(10);
  tracer().fill('.');
  tracer().unsetf(ios::adjustfield);
  tracer().setf(ios::left);
  tracer() << string1 << endl;

  tracer() << "ios width = 10" << endl;
  tracer() << "ios fill = ." << endl;
  tracer() << "ios justify = right" << endl;
  tracer() << "string1 = " << flush;
  tracer().width(10);
  tracer().fill('.');
  tracer().unsetf(ios::adjustfield);
  tracer().setf(ios::right);
  tracer() << string1 << endl;

  tracer() << "ios width = 10" << endl;
  tracer() << "ios fill = ." << endl;
  tracer() << "ios justify = internal" << endl;
  tracer() << "string1 = " << flush;
  tracer().width(10);
  tracer().fill('.');
  tracer().unsetf(ios::adjustfield);
  tracer().setf(ios::internal);
  tracer() << string1 << endl;

  tracer() << "ios width = 10" << endl;
  tracer() << "ios fill = ." << endl;
  tracer() << "ios justify = left & right" << endl;
  tracer() << "string1 = " << flush;
  tracer().width(10);
  tracer().fill('.');
  tracer().unsetf(ios::adjustfield);
  tracer().setf(ios::right);
  tracer().setf(ios::left);
  tracer() << string1 << endl;
}

// = TEST 9
//     Test of duplicating a string.

void test9()
{
  OTC_Tracer tracer("void test9()");

  OTC_String string("1234567");

  char* duplicate = string.duplicate();

  tracer() << "string = " << string << endl;
  tracer() << "duplicate = " << duplicate << endl;
  tracer() << "(duplicate != string) = ";
  tracer() << (string.string() != duplicate) << endl;

  delete [] duplicate;
}

// = TEST 10
//     Test of operator[].

void test10()
{
  OTC_Tracer tracer("void test10");

  tracer() << "string1(\"0123456789\")" << endl;
  OTC_String string1("0123456789");

  size_t i;
  tracer() << "string1 = " << flush;
  for (i=0; i<string1.length(); i++)
    tracer() << string1[i] << flush;
  tracer() << endl;

  tracer() << "string2(string1)" << endl;
  OTC_String string2(string1);

  tracer() << "string2[i] = string2[i] + 1" << endl;

  for (i=0; i<string2.length(); i++)
    string2[i] = string2[i]+1;

  tracer() << "string2 = " << flush;
  for (i=0; i<string2.length(); i++)
    tracer() << string2[i] << flush;
  tracer() << endl;

  tracer() << "string1 = " << flush;
  for (i=0; i<string1.length(); i++)
    tracer() << string1[i] << flush;
  tracer() << endl;
}

// = TEST 11
//     Test of searching for characters and strings.

void test11()
{
  OTC_Tracer tracer("void test11()");

  OTC_String string("01234567890123456789");

  tracer() << "string = " << string << endl;
  tracer() << "string.index('a') = " << string.index('a') << endl;
  tracer() << "string.index(10,'a') = " << string.index(10,'a') << endl;
  tracer() << "string.index('6') = " << string.index('6') << endl;
  tracer() << "string.index('6',2) = " << string.index('6',2) << endl;
  tracer() << "string.index('6',3) = " << string.index('6',3) << endl;
  tracer() << "string.index('9') = " << string.index('9') << endl;
  tracer() << "string.index('9',2) = " << string.index('9',2) << endl;
  tracer() << "string.index('9',3) = " << string.index('9',3) << endl;
  tracer() << "string.index(10,'6') = " << string.index(10,'6') << endl;

  tracer() << "string.rindex('a') = " << string.rindex('a') << endl;
  tracer() << "string.rindex('6') = " << string.rindex('6') << endl;
  tracer() << "string.rindex('6',2) = " << string.rindex('6',2) << endl;
  tracer() << "string.rindex('6',3) = " << string.rindex('6',3) << endl;

  tracer() << "string.index(\"a\") = " << string.index("a") << endl;
  tracer() << "string.index(\"6\") = " << string.index("6") << endl;
  tracer() << "string.index(\"67\") = " << string.index("67") << endl;
  tracer() << "string.index(\"89\") = " << string.index("89") << endl;
  tracer() << "string.index(\"90\") = " << string.index("90") << endl;
}

// = TEST 12
//     Test of null string.

void test12()
{
  OTC_Tracer tracer("void test12()");

  OTC_String string = OTC_String::nullString();

  tracer() << "string.length() = " << string.length() << endl;
  tracer() << "string.isEmpty() = " << string.isEmpty() << endl;
  tracer() << "(*string.string() const == EOS) = ";
  tracer() << (*string.string() == EOS) << endl;
  tracer() << "string.hash() = "
	   << OTC_HashActions<OTC_String>::hash(string) << endl;
}

// = TEST 13
//     Test of comparison against null terminate string.

void test13()
{
  OTC_Tracer tracer("void test13()");

  OTC_String string1;
  tracer() << "string1 = " << string1 << endl;

  tracer() << "string1.compare(size_t(0),0) = ";
  tracer() << string1.compare(size_t(0),0) << endl;
  tracer() << "string1.compare(0,\"\") = ";
  tracer() << string1.compare(0,"") << endl;
  tracer() << "string1.compare(0,\"1234567\") = ";
  tracer() << string1.compare(0,"1234567") << endl;

  OTC_String string2("1234567");
  tracer() << "string2 = " << string2 << endl;

  tracer() << "string2.compare(size_t(0),0) = ";
  tracer() << string2.compare(size_t(0),0) << endl;
  tracer() << "string2.compare(0,\"1234\") = ";
  tracer() << string2.compare(0,"1234") << endl;
  tracer() << "string2.compare(0,\"7654321\") = ";
  tracer() << string2.compare(0,"7654321") << endl;
  tracer() << "string2.compare(0,\"1234567\") = ";
  tracer() << string2.compare(0,"1234567") << endl;
  tracer() << "string2.compare(1,\"2345\") = ";
  tracer() << string2.compare(1,"2345") << endl;
  tracer() << "string2.compare(1,\"654321\") = ";
  tracer() << string2.compare(1,"7654321") << endl;
  tracer() << "string2.compare(1,\"234567\") = ";
  tracer() << string2.compare(1,"234567") << endl;
}

// = TEST 14
//     Test of comparison against range of characters in a string.

void test14()
{
  OTC_Tracer tracer("void test14()");

  OTC_String string1;
  tracer() << "string1 = " << string1 << endl;

  tracer() << "string1.compare(size_t(0),0,0) = ";
  tracer() << string1.compare(size_t(0),(char*)0,size_t(0)) << endl;
  tracer() << "string1.compare(0,\"\",0) = ";
  tracer() << string1.compare(0,"",size_t(0)) << endl;
  tracer() << "string1.compare(0,\"1234567\",7) = ";
  tracer() << string1.compare(0,"1234567",size_t(7)) << endl;

  OTC_String string2("1234567");
  tracer() << "string2 = " << string2 << endl;

  tracer() << "string2.compare(size_t(0),0,0) = ";
  tracer() << string2.compare(size_t(0),(char*)0,size_t(0)) << endl;
  tracer() << "string2.compare(0,\"1234\",4) = ";
  tracer() << string2.compare(0,"1234",size_t(4)) << endl;
  tracer() << "string2.compare(0,\"7654321\",7) = ";
  tracer() << string2.compare(0,"7654321",size_t(7)) << endl;
  tracer() << "string2.compare(0,\"1234567\",7) = ";
  tracer() << string2.compare(0,"1234567",size_t(7)) << endl;
  tracer() << "string2.compare(1,\"2345\",4) = ";
  tracer() << string2.compare(1,"2345",size_t(4)) << endl;
  tracer() << "string2.compare(1,\"654321\",6) = ";
  tracer() << string2.compare(1,"7654321",size_t(6)) << endl;
  tracer() << "string2.compare(1,\"234567\",6) = ";
  tracer() << string2.compare(1,"234567",size_t(6)) << endl;
}

// = TEST 15
//     Test of rank.

void test15()
{
  OTC_Tracer tracer("void test15()");

  OTC_String string1;
  tracer() << "string1 = " << string1 << endl;

  OTC_String string2;
  tracer() << "string2 = " << string2 << endl;

  tracer() << "(string1.rank(string2) == 0) = ";
  tracer() << (string1.rank(string2) == 0) << endl;
  tracer() << "(string2.rank(string1) == 0) = ";
  tracer() << (string2.rank(string1) == 0) << endl;

  OTC_String string3("1234567");
  tracer() << "string3 = " << string3 << endl;

  tracer() << "(string3.rank(string2) > 0) = ";
  tracer() << (string3.rank(string2) > 0) << endl;
  tracer() << "(string2.rank(string3) < 0) = ";
  tracer() << (string2.rank(string3) < 0) << endl;

  OTC_String string4("1234567");
  tracer() << "string4 = " << string4 << endl;

  tracer() << "(string3.rank(string4) == 0) = ";
  tracer() << (string3.rank(string4) == 0) << endl;
  tracer() << "(string4.rank(string3) == 0) = ";
  tracer() << (string4.rank(string3) == 0) << endl;

  OTC_String string5("123456");
  tracer() << "string5 = " << string5 << endl;

  tracer() << "(string4.rank(string5) > 0) = ";
  tracer() << (string4.rank(string5) > 0) << endl;
  tracer() << "(string5.rank(string4) < 0) = ";
  tracer() << (string5.rank(string4) < 0) << endl;

  OTC_String string6("2345678");
  tracer() << "string6 = " << string6 << endl;

  tracer() << "(string4.rank(string6) < 0) = ";
  tracer() << (string4.rank(string6) < 0) << endl;
  tracer() << "(string6.rank(string4) > 0) = ";
  tracer() << (string6.rank(string4) > 0) << endl;
}

// = TEST 16
//     Test of character replacement routines.

void test16()
{
  OTC_Tracer tracer("void test16()");

  // Replace all characters in string with same number of '*'.

  OTC_String string1("0123456789");
  tracer() << "string1 = " << string1 << endl;
  tracer() << "string1.replace(0,string1.length(),'*',string1.length())";
  tracer() << endl;
  string1.replace(0,string1.length(),'*',string1.length());
  tracer() << "string1 = " << string1 << endl;
  tracer() << "string1.length() = " << string1.length() << endl;

  // Replace all characters in string with half the number of '*'.

  OTC_String string2("0123456789");
  tracer() << "string2 = " << string2 << endl;
  tracer() << "string2.replace(0,string2.length(),'*',string2.length()/2)";
  tracer() << endl;
  string2.replace(0,string2.length(),'*',string2.length()/2);
  tracer() << "string2 = " << string2 << endl;
  tracer() << "string2.length() = " << string2.length() << endl;

  // Replace all characters in string with twice the number of '*'.

  OTC_String string3("0123456789");
  tracer() << "string3 = " << string3 << endl;
  tracer() << "string3.replace(0,string3.length(),'*',2*string3.length())";
  tracer() << endl;
  string3.replace(0,string3.length(),'*',2*string3.length());
  tracer() << "string3 = " << string3 << endl;
  tracer() << "string3.length() = " << string3.length() << endl;

  // Insert characters at start.

  OTC_String string4("0123456789");
  tracer() << "string4 = " << string4 << endl;
  tracer() << "string4.replace(0,0,'*',string4.length())" << endl;
  string4.replace(0,0,'*',string4.length());
  tracer() << "string4 = " << string4 << endl;
  tracer() << "string4.length() = " << string4.length() << endl;

  // Add characters at end.

  OTC_String string5("0123456789");
  tracer() << "string5 = " << string5 << endl;
  tracer() << "string5.replace(string5.length(),0,'*',string5.length())";
  tracer() << endl;
  string5.replace(string5.length(),0,'*',string5.length());
  tracer() << "string5 = " << string5 << endl;
  tracer() << "string5.length() = " << string5.length() << endl;

  // Insert in middle.

  OTC_String string6("0123456789");
  tracer() << "string6 = " << string6 << endl;
  tracer() << "string6.replace(string6.length()/2,0,'*',string6.length())";
  tracer() << endl;
  string6.replace(string6.length()/2,0,'*',string6.length());
  tracer() << "string6 = " << string6 << endl;
  tracer() << "string6.length() = " << string6.length() << endl;

  // Replace range of characters in middle.

  OTC_String string7("0123456789");
  tracer() << "string7 = " << string7 << endl;
  tracer() << "string7.replace(1,string7.length()-2,'*',string7.length())";
  tracer() << endl;
  string7.replace(1,string7.length()-2,'*',string7.length());
  tracer() << "string7 = " << string7 << endl;
  tracer() << "string7.length() = " << string7.length() << endl;

  // Delete all characters.

  OTC_String string8("0123456789");
  tracer() << "string8 = " << string8 << endl;
  tracer() << "string8.replace(0,string8.length(),'*',0)" << endl;
  string8.replace(0,string8.length(),'*',0);
  tracer() << "string8 = " << string8 << endl;
  tracer() << "string8.length() = " << string8.length() << endl;

  // Delete range of characters from start.

  OTC_String string9("0123456789");
  tracer() << "string9 = " << string9 << endl;
  tracer() << "string9.replace(0,string9.length()-1,'*',0)" << endl;
  string9.replace(0,string9.length()-1,'*',0);
  tracer() << "string9 = " << string9 << endl;
  tracer() << "string9.length() = " << string9.length() << endl;

  // Delete range of characters from end.

  OTC_String string10("0123456789");
  tracer() << "string10 = " << string10 << endl;
  tracer() << "string10.replace(1,string10.length()-1,'*',0)" << endl;
  string10.replace(1,string10.length()-1,'*',0);
  tracer() << "string10 = " << string10 << endl;
  tracer() << "string10.length() = " << string10.length() << endl;

  // Delete range of characters from middle.

  OTC_String string11("0123456789");
  tracer() << "string11 = " << string11 << endl;
  tracer() << "string11.replace(1,string11.length()-2,'*',0)" << endl;
  string11.replace(1,string11.length()-2,'*',0);
  tracer() << "string11 = " << string11 << endl;
  tracer() << "string11.length() = " << string11.length() << endl;

  // Check that shared object is split.

  OTC_String string12("0123456789");
  OTC_String string13(string12);
  tracer() << "string12 = " << string12 << endl;
  tracer() << "string13 = " << string13 << endl;
  tracer() << "string12.replace(0,string12.length(),'*',string12.length())";
  tracer() << endl;
  string12.replace(0,string12.length(),'*',string12.length());
  tracer() << "string12 = " << string12 << endl;
  tracer() << "string12.length() = " << string12.length() << endl;
  tracer() << "string13 = " << string13 << endl;
  tracer() << "string13.length() = " << string13.length() << endl;
}

// = TEST 17
//     Test of string replacement routines.

void test17()
{
  OTC_Tracer tracer("void test17()");

  // Replace all characters in string with same number of '*'.

  OTC_String string1("0123456789");
  tracer() << "string1 = " << string1 << endl;
  tracer() << "string1.replace(0,string1.length(),\"**********\",10)" << endl;
  string1.replace(0,string1.length(),"**********",10);
  tracer() << "string1 = " << string1 << endl;
  tracer() << "string1.length() = " << string1.length() << endl;

  // Replace all characters in string with half the number of '*'.

  OTC_String string2("0123456789");
  tracer() << "string2 = " << string2 << endl;
  tracer() << "string2.replace(0,string2.length(),\"*****\",5)" << endl;
  string2.replace(0,string2.length(),"*****",5);
  tracer() << "string2 = " << string2 << endl;
  tracer() << "string2.length() = " << string2.length() << endl;

  // Replace all characters in string with twice the number of '*'.

  OTC_String string3("0123456789");
  tracer() << "string3 = " << string3 << endl;
  tracer() << "string3.replace(0,string3.length(),\"*******************\",20)";
  tracer() << endl;
  string3.replace(0,string3.length(),"********************",20);
  tracer() << "string3 = " << string3 << endl;
  tracer() << "string3.length() = " << string3.length() << endl;

  // Insert characters at start.

  OTC_String string4("0123456789");
  tracer() << "string4 = " << string4 << endl;
  tracer() << "string4.replace(0,0,\"**********\",10)" << endl;
  string4.replace(0,0,"**********",10);
  tracer() << "string4 = " << string4 << endl;
  tracer() << "string4.length() = " << string4.length() << endl;

  // Add characters at end.

  OTC_String string5("0123456789");
  tracer() << "string5 = " << string5 << endl;
  tracer() << "string5.replace(string5.length(),0,\"**********\",10)" << endl;
  string5.replace(string5.length(),0,"**********",10);
  tracer() << "string5 = " << string5 << endl;
  tracer() << "string5.length() = " << string5.length() << endl;

  // Insert in middle.

  OTC_String string6("0123456789");
  tracer() << "string6 = " << string6 << endl;
  tracer() << "string6.replace(string6.length()/2,0,\"**********\",10)" << endl;
  string6.replace(string6.length()/2,0,"**********",10);
  tracer() << "string6 = " << string6 << endl;
  tracer() << "string6.length() = " << string6.length() << endl;

  // Replace range of characters in middle.

  OTC_String string7("0123456789");
  tracer() << "string7 = " << string7 << endl;
  tracer() << "string7.replace(1,string7.length()-2,\"**********\",10)" << endl;
  string7.replace(1,string7.length()-2,"**********",10);
  tracer() << "string7 = " << string7 << endl;
  tracer() << "string7.length() = " << string7.length() << endl;

  // Delete all characters.

  OTC_String string8("0123456789");
  tracer() << "string8 = " << string8 << endl;
  tracer() << "string8.replace(0,string8.length(),\"\",0)" << endl;
  string8.replace(0,string8.length(),"",0);
  tracer() << "string8 = " << string8 << endl;
  tracer() << "string8.length() = " << string8.length() << endl;

  // Delete range of characters from start.

  OTC_String string9("0123456789");
  tracer() << "string9 = " << string9 << endl;
  tracer() << "string9.replace(0,string9.length()-1,\"\",0)" << endl;
  string9.replace(0,string9.length()-1,"",0);
  tracer() << "string9 = " << string9 << endl;
  tracer() << "string9.length() = " << string9.length() << endl;

  // Delete range of characters from end.

  OTC_String string10("0123456789");
  tracer() << "string10 = " << string10 << endl;
  tracer() << "string10.replace(1,string10.length()-1,\"\",0)" << endl;
  string10.replace(1,string10.length()-1,"",0);
  tracer() << "string10 = " << string10 << endl;
  tracer() << "string10.length() = " << string10.length() << endl;

  // Delete range of characters from middle.

  OTC_String string11("0123456789");
  tracer() << "string11 = " << string11 << endl;
  tracer() << "string11.replace(1,string11.length()-2,\"\",0)" << endl;
  string11.replace(1,string11.length()-2,"",0);
  tracer() << "string11 = " << string11 << endl;
  tracer() << "string11.length() = " << string11.length() << endl;

  // Check that shared object is split.

  OTC_String string12("0123456789");
  OTC_String string13(string12);
  tracer() << "string12 = " << string12 << endl;
  tracer() << "string13 = " << string13 << endl;
  tracer() << "string12.replace(0,string12.length(),\"**********\",10)" << endl;
  string12.replace(0,string12.length(),"**********",10);
  tracer() << "string12 = " << string12 << endl;
  tracer() << "string12.length() = " << string12.length() << endl;
  tracer() << "string13 = " << string13 << endl;
  tracer() << "string13.length() = " << string13.length() << endl;

  // Check overlapping copies in same string.

  OTC_String string14("0123456789");
  tracer() << "string14 = " << string14 << endl;
  tracer() << "string14.replace(1,9,string14,9)" << endl;
  string14.replace(1,9,string14,9);
  tracer() << "string14 = " << string14 << endl;

  OTC_String string15("0123456789");
  tracer() << "string15 = " << string15 << endl;
  tracer() << "string15.replace(0,9,string15.string()+1,9)" << endl;
  string15.replace(0,9,string15.string()+1,9);
  tracer() << "string15 = " << string15 << endl;
}

// = TEST 18
//     Test of assign and truncate.

void test18()
{
  OTC_Tracer tracer("void test18()");

  OTC_String string1("0123456789");
  tracer() << "string1 = " << string1 << endl;

  OTC_String string2("9876543210");
  tracer() << "string2 = " << string2 << endl;

  tracer() << "string1.assign(string2)" << endl;
  string1.assign(string2);

  tracer() << "string1 = " << string1 << endl;
  tracer() << "string2 = " << string1 << endl;

  tracer() << "string1.truncate(0)" << endl;
  string1.truncate(0);
  tracer() << "string1 = " << string1 << endl;

  tracer() << "string2.truncate(5)" << endl;
  string2.truncate(5);
  tracer() << "string2 = " << string2 << endl;
}

// = TEST 19
//     Test of slice and similar routines.

void test19()
{
  OTC_Tracer tracer("void test19()");

  OTC_String string1("0123456789");
  tracer() << "string1 = " << string1 << endl;

  OTC_String string2;

  // Extract complete string.

  tracer() << "string2 = string1.section(0,string1.length())" << endl;
  string2 = string1.section(0,string1.length());
  tracer() << "string2 = " << string2 << endl;

  // Extract portion.

  tracer() << "string2 = string1.section(1,string1.length()-2)" << endl;
  string2 = string1.section(1,string1.length()-2);
  tracer() << "string2 = " << string2 << endl;

  // Extract from between to indexes.

  tracer() << "string2 = string1.between(1,string1.length()-2)" << endl;
  string2 = string1.between(1,string1.length()-2);
  tracer() << "string2 = " << string2 << endl;

  // Extract from after an index.

  tracer() << "string2 = string1.after(1)" << endl;
  string2 = string1.after(1);
  tracer() << "string2 = " << string2 << endl;

  // Extract from an index.

  tracer() << "string2 = string1.from(1)" << endl;
  string2 = string1.from(1);
  tracer() << "string2 = " << string2 << endl;

  // Extract from before an index.

  tracer() << "string2 = string1.before(string1.length()-2)" << endl;
  string2 = string1.before(string1.length()-2);
  tracer() << "string2 = " << string2 << endl;

  // Extract through to an index.

  tracer() << "string2 = string1.through(string1.length()-2)" << endl;
  string2 = string1.through(string1.length()-2);
  tracer() << "string2 = " << string2 << endl;

  // Extract except from index to index.

  OTC_Range range1(1,string1.length()-2);
  tracer() << "string2 = string1.besides(1,string1.length()-2)" << endl;
  string2 = string1.besides(range1);
  tracer() << "string2 = " << string2 << endl;
}

// = TEST 20
//     Test of streams insertion.

void test20()
{
  OTC_Tracer tracer("void test20()");

  tracer() << "ss1 = qwerty\nytrewq\nqwerty" << endl;
  istrstream ss1("qwerty\nytrewq\nqwerty");

  OTC_String string1, string2, string3;
  ss1 >> string1 >> string2 >> string3;
  tracer() << "string1 = " << string1 << endl;
  tracer() << "string2 = " << string2 << endl;
  tracer() << "string3 = " << string3 << endl;

  tracer() << "ss2 = qwerty\nytrewq\nqwerty" << endl;
  istrstream ss2("qwerty\nytrewq\nqwerty");
  tracer() << "ss2.unsetf(ios::skipws)" << endl;
  ss2.unsetf(ios::skipws);

  OTC_String string4, string5, string6, string7, string8;
  ss2 >> string4 >> string5 >> ws >> string6 >> string7 >> ws >> string8;
  tracer() << "string4 = " << string4 << endl;
  tracer() << "string5 = " << string5 << endl;
  tracer() << "string6 = " << string6 << endl;
  tracer() << "string7 = " << string7 << endl;
  tracer() << "string8 = " << string8 << endl;

  OTC_String string9;
  tracer() << "ss3 = 1234567890" << endl;
  istrstream ss3("1234567890");
  tracer() << "ss3.width(5)" << endl;
  ss3.width(5);
  ss3 >> string9;
  tracer() << "string9 = " << string9 << endl;
  tracer() << "ss3.width() = " << ss3.width() << endl;
}

// = TEST 21
//     Test of get from stream.

void test21()
{
  OTC_Tracer tracer("void test21()");

  strstream ss1;
  OTC_String buffer0("qwerty\nytrewq\nqwerty\0ytrewq",27);
  tracer() << "ss1 = " << buffer0 << endl;
  ss1 << buffer0;

  OTC_String string1;
  tracer() << "string1 = OTC_String::get(ss1,EOL)" << endl;
  string1 = OTC_String::get(ss1,EOL);
  tracer() << "string1 = " << string1 << endl;

  tracer() << "ss1.ignore(1)" << endl;
  ss1.ignore(1);
  OTC_String string2;
  tracer() << "string2 = OTC_String::get(ss1,EOL)" << endl;
  string2 = OTC_String::get(ss1,EOL);
  tracer() << "string2 = " << string2 << endl;

  tracer() << "ss1.ignore(1)" << endl;
  ss1.ignore(1);
  OTC_String string3;
  string3 = OTC_String::get(ss1,EOL);
  tracer() << "string3 = " << string3 << endl;

  buffer0 = "qwerty\nytrewq\nqwerty";
  strstream ss2;
  tracer() << "ss2 = " << buffer0 << endl;
  ss2 << buffer0;

  OTC_String string5;
  string5 = OTC_String::get(ss2,EOS);
  tracer() << "string5 = " << string5 << endl;
}

// = TEST 22
//     test of getline from stream.

void test22()
{
  OTC_Tracer tracer("void test22()");

  strstream ss1;
  OTC_String buffer0("qwerty\nytrewq\nqwerty\0ytrewq",27);
  tracer() << "ss1 = " << buffer0 << endl;
  ss1 << buffer0;

  OTC_String string1;
  tracer() << "string1 = OTC_String::getline(ss1,EOL)" << endl;
  string1 = OTC_String::getline(ss1,EOL);
  tracer() << "string1 = " << string1 << endl;

  OTC_String string2;
  tracer() << "string2 = OTC_String::getline(ss1,EOL)" << endl;
  string2 = OTC_String::getline(ss1,EOL);
  tracer() << "string2 = " << string2 << endl;

  OTC_String string3;
  tracer() << "string3 = OTC_String::getline(ss1,EOL)" << endl;
  string3 = OTC_String::getline(ss1,EOL);
  tracer() << "string3 = " << string3 << endl;

  buffer0 = "qwerty\nytrewq\nqwerty";
  strstream ss2;
  tracer() << "ss2 = " << buffer0 << endl;
  ss2 << buffer0;

  OTC_String string4;
  tracer() << "string4 = OTC_String::getline(ss2,EOS)" << endl;
  string4 = OTC_String::getline(ss2,EOS);
  tracer() << "string4 = " << string4 << endl;
}

// = TEST 23
//     Test of read from a stream.

void test23()
{
  OTC_Tracer tracer("void test23()");

  strstream ss1;
  OTC_String string0("1234567890");
  tracer() << "ss1 = " << string0 << endl;
  ss1 << string0;

  OTC_String string1;
  tracer() << "string1 = OTC_String::read(ss1,5)" << endl;
  string1 = OTC_String::read(ss1,5);
  tracer() << "string1 = " << string1 << endl;

  OTC_String string2;
  tracer() << "string2 = OTC_String::read(ss1,2)" << endl;
  string2 = OTC_String::read(ss1,2);
  tracer() << "string2 = " << string2 << endl;

  OTC_String string3;
  tracer() << "string3 = OTC_String::read(ss1,5)" << endl;
  string3 = OTC_String::read(ss1,5);
  tracer() << "string3 = " << string3 << endl;
}

// = TEST 24
//     Test of hash function.

void test24()
{
  OTC_Tracer tracer("void test24()");

  OTC_String string1;
  tracer() << "string1.hash() = "
	   << OTC_HashActions<OTC_String>::hash(string1) << endl;

  OTC_String string2("01234567890");
  tracer() << "string2.hash() = "
	   << OTC_HashActions<OTC_String>::hash(string2) << endl;
}

// = TEST 25
//     Test of upper and lower.

void test25()
{
  OTC_Tracer tracer("void test25()");

  OTC_String string1("qwerty123456");
  tracer() << "string1 = " << string1 << endl;
  string1.upper(1);
  tracer() << "string1 = " << string1 << endl;
  string1.upper();
  tracer() << "string1 = " << string1 << endl;
  string1.lower(1);
  tracer() << "string1 = " << string1 << endl;
  string1.lower();
  tracer() << "string1 = " << string1 << endl;
}

// = TEST 26
//     Test of trim functions.

void test26()
{
  OTC_Tracer tracer("void test26()");

  OTC_String string1(" \t\f\nqwerty\n\f\t ");
  tracer() << "string1 = " << string1 << endl;
  string1.ltrim();
  tracer() << "string1 = " << string1 << endl;
  tracer() << "string1.length() = " << string1.length() << endl;
  string1.rtrim();
  tracer() << "string1 = " << string1 << endl;
  tracer() << "string1.length() = " << string1.length() << endl;

  OTC_String string2(" \t\f\nqwerty\n\f\t ");
  tracer() << "string2 = " << string2 << endl;
  string2.rtrim();
  tracer() << "string2 = " << string2 << endl;
  tracer() << "string2.length() = " << string2.length() << endl;
  string2.ltrim();
  tracer() << "string2 = " << string2 << endl;
  tracer() << "string2.length() = " << string2.length() << endl;

  OTC_String string3(" \t\f\nqwerty\n\f\t ");
  tracer() << "string3 = " << string3 << endl;
  string3.trim();
  tracer() << "string3 = " << string3 << endl;
  tracer() << "string3.length() = " << string3.length() << endl;
}

// = TEST 27
//     Test of string reversal.

void test27()
{
  OTC_Tracer tracer("void test27()");

  OTC_String string1("*");
  tracer() << "string1 = " << string1 << endl;
  string1.reverse();
  tracer() << "string1 = " << string1 << endl;

  OTC_String string2("qwerty!");
  tracer() << "string2 = " << string2 << endl;
  string2.reverse();
  tracer() << "string2 = " << string2 << endl;

  OTC_String string3 = string2;
  tracer() << "string3 = string2" << endl;
  string3.reverse();
  tracer() << "string2 = " << string2 << endl;
  tracer() << "string3 = " << string3 << endl;
}

// = TEST 28
//     Test of justification.

void test28()
{
  OTC_Tracer tracer("void test28()");

  OTC_String string("***");

  tracer() << "string = " << string << endl;

  tracer() << "string.ljustify(6,'.')" << endl;
  string.ljustify(6,'.');

  tracer() << "string = " << string << endl;

  tracer() << "string.rjustify(9,'.')" << endl;
  string.rjustify(9,'.');

  tracer() << "string = " << string << endl;
}

// = TEST 29
//    Test of adding String and SObject (Symbol).

void test29()
{
  OTC_Tracer tracer("void test29()");

  OTC_String string1 = "one";
  OTC_Symbol symbol1 = "two";
  OTC_String string2;

  tracer() << "string1 = " << string1 << endl;
  tracer() << "symbol1 = " << symbol1 << endl;
  tracer() << "string2 = " << string2 << endl;

  tracer() << "string2 = string1 + symbol1" << endl;
  string2 = string1 + symbol1;

  tracer() << "string2 = " << string2 << endl;

  tracer() << "string2 = symbol1 + string1" << endl;
  string2 = symbol1 + string1;

  tracer() << "string2 = " << string2 << endl;

  tracer() << "string2 = \"\"" << endl;
  string2 = "";

  tracer() << "string2 += string1 + symbol1" << endl;
  string2 += string1 + symbol1;

  tracer() << "string2 = " << string2 << endl;

  tracer() << "string2 = \"\"" << endl;
  string2 = "";

  tracer() << "string2 += symbol1 + string1" << endl;
  string2 += symbol1 + string1;

  tracer() << "string2 = " << string2 << endl;
}

// = TEST 30
//     Test getline() with long input.

void test30()
{
  OTC_Tracer tracer("void test30()");

  char* data =
   "123456789012345678901234567890\n"
   "123456789012345678901234567890"
   "123456789012345678901234567890"
   "12345678901234567890\n"
   "1234567890123456789012345678901234567890";

  istrstream ins(data,strlen(data));

  OTC_String line;

  line = "";
  OTC_String::getline(line,ins);
  tracer() << "line = " << line << endl;

  line = "";
  OTC_String::getline(line,ins);
  tracer() << "line = " << line << endl;

  line = "";
  OTC_String::getline(line,ins);
  tracer() << "line = " << line << endl;
}

// = TEST 31
//     Test get() with long input.

void test31()
{
  OTC_Tracer tracer("void test31()");

  char* data =
   "123456789012345678901234567890\n"
   "123456789012345678901234567890"
   "123456789012345678901234567890"
   "12345678901234567890\n"
   "1234567890123456789012345678901234567890";

  istrstream ins(data,strlen(data));

  OTC_String line;

  line = "";
  OTC_String::get(line,ins);
  tracer() << "line = " << line << endl;
  ins.ignore(1);

  line = "";
  OTC_String::get(line,ins);
  tracer() << "line = " << line << endl;
  ins.ignore(1);

  line = "";
  OTC_String::get(line,ins);
  tracer() << "line = " << line << endl;
}

// = TEST 32
//     Test index()/rinex() with OTC_CBitSet.

void test32()
{
  OTC_Tracer tracer("void test32()");

  OTC_String string1;

  string1 = "01234567890123456789";

  tracer() << "string1 = " << string1 << endl;

  OTC_CBitSet set1;
  set1.set('0');
  set1.set('5');

  tracer() << "string1.index([05]) = " << string1.index(set1) << endl;
  tracer() << "string1.index([05],2) = " << string1.index(set1,2) << endl;
  tracer() << "string1.index([05],3) = " << string1.index(set1,3) << endl;
  tracer() << "string1.index([05],4) = " << string1.index(set1,4) << endl;
  tracer() << "string1.index([05],5) = " << string1.index(set1,5) << endl;

  tracer() << "string1.index(2,[05]) = " << string1.index(2,set1) << endl;
  tracer() << "string1.index(2,[05],2) = " << string1.index(2,set1,2) << endl;
  tracer() << "string1.index(2,[05],3) = " << string1.index(2,set1,3) << endl;
  tracer() << "string1.index(2,[05],4) = " << string1.index(2,set1,4) << endl;
  tracer() << "string1.index(2,[05],5) = " << string1.index(2,set1,5) << endl;

  tracer() << "string1.rindex([05]) = " << string1.rindex(set1) << endl;
  tracer() << "string1.rindex([05],2) = " << string1.rindex(set1,2) << endl;
  tracer() << "string1.rindex([05],3) = " << string1.rindex(set1,3) << endl;
  tracer() << "string1.rindex([05],4) = " << string1.rindex(set1,4) << endl;
  tracer() << "string1.rindex([05],5) = " << string1.rindex(set1,5) << endl;
}

// = TEST 33
//     Test of rindex() with string.

void test33()
{
  OTC_Tracer tracer("void test33()");

  OTC_String string("01234567890123456789");

  tracer() << "string = " << string << endl;
  tracer() << "string.rindex(\"a\") = " << string.rindex("a") << endl;
  tracer() << "string.rindex(\"6\") = " << string.rindex("6") << endl;
  tracer() << "string.rindex(\"67\") = " << string.rindex("67") << endl;
  tracer() << "string.rindex(\"89\") = " << string.rindex("89") << endl;
  tracer() << "string.rindex(\"90\") = " << string.rindex("90") << endl;
}

// = TEST 34
//     Test of getline() with long input.

void test34()
{
  OTC_Tracer tracer("void test34()");

  strstream ss1;
  OTC_String buffer0('0',1001);
  char c = 'A';
  for (size_t i=0; i<buffer0.length(); i+=50,c++)
    buffer0[i] = c;
  tracer() << "ss1 = " << buffer0 << endl;
  ss1 << buffer0;

  OTC_String string1;
  tracer() << "string1 = OTC_String::getline(ss1)" << endl;
  string1 = OTC_String::getline(ss1);
  tracer() << "string1 = " << string1 << endl;
}

// = TEST 35
//     Test of rank, using locale ordering.

void test35()
{
  OTC_Tracer tracer("void test35()");

  OTC_String string1;
  tracer() << "string1 = " << string1 << endl;

  OTC_String string2;
  tracer() << "string2 = " << string2 << endl;

  tracer() << "(string1.rank(string2,OTCLIB_LOCALEORDER) == 0) = ";
  tracer() << (string1.rank(string2,OTCLIB_LOCALEORDER) == 0) << endl;
  tracer() << "(string2.rank(string1,OTCLIB_LOCALEORDER) == 0) = ";
  tracer() << (string2.rank(string1,OTCLIB_LOCALEORDER) == 0) << endl;

  OTC_String string3("1234567");
  tracer() << "string3 = " << string3 << endl;

  tracer() << "(string3.rank(string2,OTCLIB_LOCALEORDER) > 0) = ";
  tracer() << (string3.rank(string2,OTCLIB_LOCALEORDER) > 0) << endl;
  tracer() << "(string2.rank(string3,OTCLIB_LOCALEORDER) < 0) = ";
  tracer() << (string2.rank(string3,OTCLIB_LOCALEORDER) < 0) << endl;

  OTC_String string4("1234567");
  tracer() << "string4 = " << string4 << endl;

  tracer() << "(string3.rank(string4,OTCLIB_LOCALEORDER) == 0) = ";
  tracer() << (string3.rank(string4,OTCLIB_LOCALEORDER) == 0) << endl;
  tracer() << "(string4.rank(string3,OTCLIB_LOCALEORDER) == 0) = ";
  tracer() << (string4.rank(string3,OTCLIB_LOCALEORDER) == 0) << endl;

  OTC_String string5("123456");
  tracer() << "string5 = " << string5 << endl;

  tracer() << "(string4.rank(string5,OTCLIB_LOCALEORDER) > 0) = ";
  tracer() << (string4.rank(string5,OTCLIB_LOCALEORDER) > 0) << endl;
  tracer() << "(string5.rank(string4,OTCLIB_LOCALEORDER) < 0) = ";
  tracer() << (string5.rank(string4,OTCLIB_LOCALEORDER) < 0) << endl;

  OTC_String string6("2345678");
  tracer() << "string6 = " << string6 << endl;

  tracer() << "(string4.rank(string6,OTCLIB_LOCALEORDER) < 0) = ";
  tracer() << (string4.rank(string6,OTCLIB_LOCALEORDER) < 0) << endl;
  tracer() << "(string6.rank(string4,OTCLIB_LOCALEORDER) > 0) = ";
  tracer() << (string6.rank(string4,OTCLIB_LOCALEORDER) > 0) << endl;

  OTC_String string7;
  string7.append("234");
  string7.append(EOS);
  string7.append("678");
  tracer() << "string7 = " << string7 << endl;

  tracer() << "(string4.rank(string7,OTCLIB_LOCALEORDER) < 0) = ";
  tracer() << (string4.rank(string7,OTCLIB_LOCALEORDER) < 0) << endl;
  tracer() << "(string7.rank(string4,OTCLIB_LOCALEORDER) > 0) = ";
  tracer() << (string7.rank(string4,OTCLIB_LOCALEORDER) > 0) << endl;
}

// = TEST 36
//     Test of readToken().

void test36()
{
  OTC_Tracer tracer("void test36()");

  istrstream ins("one two three four");

  tracer() << "tokens =";

  OTC_String string1;
  while (ins.good())
  {
    string1.readToken(ins);
    if (!ins.fail())
      tracer() << " (" << string1 << ")";
  }
  tracer() << endl;
}

// = TEST 37
//     Test of capacity.

void test37()
{
  OTC_Tracer tracer("void test37()");

  OTC_String s1(OTC_Capacity(10));

  tracer() << "s1.capacity() = " << s1.capacity() << endl;

  OTC_String s2;
  s2.capacity(10);

  tracer() << "s2.capacity() = " << s2.capacity() << endl;
}

// = TEST 38
//     Test of operator>>() with more than 16 characters input.

void test38()
{
  OTC_Tracer tracer("void test38()");

  strstream ss;
  ss << "12345678901234567";

  OTC_String s1;
  ss >> s1;

  tracer() << "s1 = " << s1 << endl;
}

// = TEST 39
//     Test of readFile().

void test39()
{
  OTC_Tracer tracer("void test39()");

  OTC_String i1;

  i1.append('A',100);
  i1 += char(0xff);
  i1.append('B',100);
  i1 += char(0xff);
  i1.append('C',100);
  i1 += char(0xff);
  i1.append('D',100);
  i1 += char(0xff);
  i1.append('E',100);
  i1 += char(0xff);
  i1.append('F',100);

  tracer() << "i1 = " << i1 << endl;
  tracer() << "i1.length() = " << i1.length() << endl;

  istrstream ss((char*)i1.string(),i1.length());

  tracer() << "good = " << !!ss.good() << endl;
  tracer() << "eof = " << !!ss.eof() << endl;
  tracer() << "fail = " << !!ss.fail() << endl;
  tracer() << "bad = " << !!ss.bad() << endl;

  OTC_String s1;
  s1.readFile(ss);

  tracer() << "s1 = " << s1 << endl;
  tracer() << "s1.length() = " << s1.length() << endl;

  tracer() << "good = " << !!ss.good() << endl;
  tracer() << "eof = " << !!ss.eof() << endl;
  tracer() << "fail = " << !!ss.fail() << endl;
  tracer() << "bad = " << !!ss.bad() << endl;
}

// = TEST 40
//     Test of istream state for operator>>().

void test40()
{
  OTC_Tracer tracer("void test40()");

  OTC_String i1;

  i1 += "   ABCD   ";
  i1 += char(0xff);
  i1 += "   EFGH   ";
  i1 += "   IJKL";

  tracer() << "i1 = \"" << i1 << "\"" << endl;
  tracer() << "i1.length() = " << i1.length() << endl;

  istrstream ss1((char*)i1.string(),i1.length());

  tracer() << "good = " << !!ss1.good() << endl;
  tracer() << "eof = " << !!ss1.eof() << endl;
  tracer() << "fail = " << !!ss1.fail() << endl;
  tracer() << "bad = " << !!ss1.bad() << endl;

  while (ss1.good())
  {
    OTC_String s1;

    ss1 >> s1;

    if (!ss1.fail())
    {
      tracer() << "s1 = \"" << s1 << "\"" << endl;
      tracer() << "s1.length() = " << s1.length() << endl;

      tracer() << "good = " << !!ss1.good() << endl;
      tracer() << "eof = " << !!ss1.eof() << endl;
      tracer() << "fail = " << !!ss1.fail() << endl;
      tracer() << "bad = " << !!ss1.bad() << endl;
    }
    else
    {
      tracer() << "s1 (fail) = \"" << s1 << "\"" << endl;
      tracer() << "s1.length() = " << s1.length() << endl;
    }
  }

  tracer() << "good = " << !!ss1.good() << endl;
  tracer() << "eof = " << !!ss1.eof() << endl;
  tracer() << "fail = " << !!ss1.fail() << endl;
  tracer() << "bad = " << !!ss1.bad() << endl;

  OTC_String i2;

  i2 += "   ABCD   ";
  i2 += char(0xff);
  i2 += "   EFGH   ";
  i2 += "   IJKL   ";

  tracer() << "i2 = \"" << i2 << "\"" << endl;
  tracer() << "i2.length() = " << i2.length() << endl;

  istrstream ss2((char*)i2.string(),i2.length());

  tracer() << "good = " << !!ss2.good() << endl;
  tracer() << "eof = " << !!ss2.eof() << endl;
  tracer() << "fail = " << !!ss2.fail() << endl;
  tracer() << "bad = " << !!ss2.bad() << endl;

  while (ss2.good())
  {
    OTC_String s1;

    ss2 >> s1;

    if (!ss2.fail())
    {
      tracer() << "s1 = \"" << s1 << "\"" << endl;
      tracer() << "s1.length() = " << s1.length() << endl;

      tracer() << "good = " << !!ss2.good() << endl;
      tracer() << "eof = " << !!ss2.eof() << endl;
      tracer() << "fail = " << !!ss2.fail() << endl;
      tracer() << "bad = " << !!ss2.bad() << endl;
    }
    else
    {
      tracer() << "s1 (fail) = \"" << s1 << "\"" << endl;
      tracer() << "s1.length() = " << s1.length() << endl;
    }
  }

  tracer() << "good = " << !!ss2.good() << endl;
  tracer() << "eof = " << !!ss2.eof() << endl;
  tracer() << "fail = " << !!ss2.fail() << endl;
  tracer() << "bad = " << !!ss2.bad() << endl;
}

// = TEST 41
//     Test of istream state for get().

void test41()
{
  OTC_Tracer tracer("void test41()");

  OTC_String i1;

  i1 += "ABCD\n";
  i1 += char(0xff);
  i1 += "\n";
  i1 += "EFGH\n";
  i1 += "IJKL\n";

  tracer() << "i1 = \"" << i1 << "\"" << endl;
  tracer() << "i1.length() = " << i1.length() << endl;

  istrstream ss1((char*)i1.string(),i1.length());

  tracer() << "good = " << !!ss1.good() << endl;
  tracer() << "eof = " << !!ss1.eof() << endl;
  tracer() << "fail = " << !!ss1.fail() << endl;
  tracer() << "bad = " << !!ss1.bad() << endl;

  while (ss1.good())
  {
    OTC_String s1;
    s1 = OTC_String::get(ss1,'\n');

    if (!ss1.fail())
    {
      tracer() << "s1 = \"" << s1 << "\"" << endl;
      tracer() << "s1.length() = " << s1.length() << endl;

      tracer() << "good = " << !!ss1.good() << endl;
      tracer() << "eof = " << !!ss1.eof() << endl;
      tracer() << "fail = " << !!ss1.fail() << endl;
      tracer() << "bad = " << !!ss1.bad() << endl;

      ss1.get();
    }
    else
    {
      tracer() << "s1 (fail) = \"" << s1 << "\"" << endl;
      tracer() << "s1.length() = " << s1.length() << endl;
    }
  }

  tracer() << "good = " << !!ss1.good() << endl;
  tracer() << "eof = " << !!ss1.eof() << endl;
  tracer() << "fail = " << !!ss1.fail() << endl;
  tracer() << "bad = " << !!ss1.bad() << endl;

  OTC_String i2;

  i2 += "ABCD\n";
  i2 += char(0xff);
  i2 += "\n";
  i2 += "EFGH\n";
  i2 += "IJKL";

  tracer() << "i2 = \"" << i2 << "\"" << endl;
  tracer() << "i2.length() = " << i2.length() << endl;

  istrstream ss2((char*)i2.string(),i2.length());

  tracer() << "good = " << !!ss2.good() << endl;
  tracer() << "eof = " << !!ss2.eof() << endl;
  tracer() << "fail = " << !!ss2.fail() << endl;
  tracer() << "bad = " << !!ss2.bad() << endl;

  while (ss2.good())
  {
    OTC_String s1;
    s1 = OTC_String::get(ss2,'\n');

    if (!ss2.fail())
    {
      tracer() << "s1 = \"" << s1 << "\"" << endl;
      tracer() << "s1.length() = " << s1.length() << endl;

      tracer() << "good = " << !!ss2.good() << endl;
      tracer() << "eof = " << !!ss2.eof() << endl;
      tracer() << "fail = " << !!ss2.fail() << endl;
      tracer() << "bad = " << !!ss2.bad() << endl;

      ss2.get();
    }
    else
    {
      tracer() << "s1 (fail) = \"" << s1 << "\"" << endl;
      tracer() << "s1.length() = " << s1.length() << endl;
    }
  }

  tracer() << "good = " << !!ss2.good() << endl;
  tracer() << "eof = " << !!ss2.eof() << endl;
  tracer() << "fail = " << !!ss2.fail() << endl;
  tracer() << "bad = " << !!ss2.bad() << endl;
}

// = TEST 42
//     Test of istream state for getline().

void test42()
{
  OTC_Tracer tracer("void test42()");

  OTC_String i1;

  i1 += "ABCD\n";
  i1 += char(0xff);
  i1 += "\n";
  i1 += "EFGH\n";
  i1 += "IJKL\n";

  tracer() << "i1 = \"" << i1 << "\"" << endl;
  tracer() << "i1.length() = " << i1.length() << endl;

  istrstream ss1((char*)i1.string(),i1.length());

  tracer() << "good = " << !!ss1.good() << endl;
  tracer() << "eof = " << !!ss1.eof() << endl;
  tracer() << "fail = " << !!ss1.fail() << endl;
  tracer() << "bad = " << !!ss1.bad() << endl;

  while (ss1.good())
  {
    OTC_String s1;
    s1 = OTC_String::getline(ss1,'\n');

    if (!ss1.fail())
    {
      tracer() << "s1 = \"" << s1 << "\"" << endl;
      tracer() << "s1.length() = " << s1.length() << endl;

      tracer() << "good = " << !!ss1.good() << endl;
      tracer() << "eof = " << !!ss1.eof() << endl;
      tracer() << "fail = " << !!ss1.fail() << endl;
      tracer() << "bad = " << !!ss1.bad() << endl;
    }
    else
    {
      tracer() << "s1 (fail) = \"" << s1 << "\"" << endl;
      tracer() << "s1.length() = " << s1.length() << endl;
    }
  }

  tracer() << "good = " << !!ss1.good() << endl;
  tracer() << "eof = " << !!ss1.eof() << endl;
  tracer() << "fail = " << !!ss1.fail() << endl;
  tracer() << "bad = " << !!ss1.bad() << endl;

  OTC_String i2;

  i2 += "ABCD\n";
  i2 += char(0xff);
  i2 += "\n";
  i2 += "EFGH\n";
  i2 += "IJKL";

  tracer() << "i2 = \"" << i2 << "\"" << endl;
  tracer() << "i2.length() = " << i2.length() << endl;

  istrstream ss2((char*)i2.string(),i2.length());

  tracer() << "good = " << !!ss2.good() << endl;
  tracer() << "eof = " << !!ss2.eof() << endl;
  tracer() << "fail = " << !!ss2.fail() << endl;
  tracer() << "bad = " << !!ss2.bad() << endl;

  while (ss2.good())
  {
    OTC_String s1;
    s1 = OTC_String::getline(ss2,'\n');

    if (!ss2.fail())
    {
      tracer() << "s1 = \"" << s1 << "\"" << endl;
      tracer() << "s1.length() = " << s1.length() << endl;

      tracer() << "good = " << !!ss2.good() << endl;
      tracer() << "eof = " << !!ss2.eof() << endl;
      tracer() << "fail = " << !!ss2.fail() << endl;
      tracer() << "bad = " << !!ss2.bad() << endl;
    }
    else
    {
      tracer() << "s1 (fail) = \"" << s1 << "\"" << endl;
      tracer() << "s1.length() = " << s1.length() << endl;
    }
  }

  tracer() << "good = " << !!ss2.good() << endl;
  tracer() << "eof = " << !!ss2.eof() << endl;
  tracer() << "fail = " << !!ss2.fail() << endl;
  tracer() << "bad = " << !!ss2.bad() << endl;
}

// = TEST 43
//     Test of istream state for read().

void test43()
{
  OTC_Tracer tracer("void test43()");

  OTC_String i1;
  i1.append('A',30);

  tracer() << "i1 = \"" << i1 << "\"" << endl;
  tracer() << "i1.length() = " << i1.length() << endl;

  istrstream ss1((char*)i1.string(),i1.length());

  tracer() << "good = " << !!ss1.good() << endl;
  tracer() << "eof = " << !!ss1.eof() << endl;
  tracer() << "fail = " << !!ss1.fail() << endl;
  tracer() << "bad = " << !!ss1.bad() << endl;

  OTC_String s1;
  s1 = OTC_String::read(ss1,20);

  tracer() << "s1 = \"" << s1 << "\"" << endl;
  tracer() << "s1.length() = " << s1.length() << endl;

  tracer() << "good = " << !!ss1.good() << endl;
  tracer() << "eof = " << !!ss1.eof() << endl;
  tracer() << "fail = " << !!ss1.fail() << endl;
  tracer() << "bad = " << !!ss1.bad() << endl;

  OTC_String i2;
  i2.append('A',30);

  tracer() << "i2 = \"" << i2 << "\"" << endl;
  tracer() << "i2.length() = " << i2.length() << endl;

  istrstream ss2((char*)i2.string(),i2.length());

  tracer() << "good = " << !!ss2.good() << endl;
  tracer() << "eof = " << !!ss2.eof() << endl;
  tracer() << "fail = " << !!ss2.fail() << endl;
  tracer() << "bad = " << !!ss2.bad() << endl;

  OTC_String s2;
  s2 = OTC_String::read(ss2,30);

  tracer() << "s2 = \"" << s2 << "\"" << endl;
  tracer() << "s2.length() = " << s2.length() << endl;

  tracer() << "good = " << !!ss2.good() << endl;
  tracer() << "eof = " << !!ss2.eof() << endl;
  tracer() << "fail = " << !!ss2.fail() << endl;
  tracer() << "bad = " << !!ss2.bad() << endl;

  OTC_String i3;
  i3.append('A',30);

  tracer() << "i3 = \"" << i3 << "\"" << endl;
  tracer() << "i3.length() = " << i3.length() << endl;

  istrstream ss3((char*)i3.string(),i3.length());

  tracer() << "good = " << !!ss3.good() << endl;
  tracer() << "eof = " << !!ss3.eof() << endl;
  tracer() << "fail = " << !!ss3.fail() << endl;
  tracer() << "bad = " << !!ss3.bad() << endl;

  OTC_String s3;
  s3 = OTC_String::read(ss3,40);

  tracer() << "s3 = \"" << s3 << "\"" << endl;
  tracer() << "s3.length() = " << s3.length() << endl;

  tracer() << "good = " << !!ss3.good() << endl;
  tracer() << "eof = " << !!ss3.eof() << endl;
  tracer() << "fail = " << !!ss3.fail() << endl;
  tracer() << "bad = " << !!ss3.bad() << endl;
}

// = TEST 44
//     Test of istream state for readToken().

void test44()
{
  OTC_Tracer tracer("void test44()");

  OTC_String i1;

  i1 += "   ABCD   ";
  i1 += char(0xff);
  i1 += "   EFGH   ";
  i1 += "   IJKL";

  tracer() << "i1 = \"" << i1 << "\"" << endl;
  tracer() << "i1.length() = " << i1.length() << endl;

  istrstream ss1((char*)i1.string(),i1.length());

  tracer() << "good = " << !!ss1.good() << endl;
  tracer() << "eof = " << !!ss1.eof() << endl;
  tracer() << "fail = " << !!ss1.fail() << endl;
  tracer() << "bad = " << !!ss1.bad() << endl;

  while (ss1.good())
  {
    OTC_String s1;

    s1.readToken(ss1);

    if (!ss1.fail())
    {
      tracer() << "s1 = \"" << s1 << "\"" << endl;
      tracer() << "s1.length() = " << s1.length() << endl;

      tracer() << "good = " << !!ss1.good() << endl;
      tracer() << "eof = " << !!ss1.eof() << endl;
      tracer() << "fail = " << !!ss1.fail() << endl;
      tracer() << "bad = " << !!ss1.bad() << endl;
    }
    else
    {
      tracer() << "s1 (fail) = \"" << s1 << "\"" << endl;
      tracer() << "s1.length() = " << s1.length() << endl;
    }
  }

  tracer() << "good = " << !!ss1.good() << endl;
  tracer() << "eof = " << !!ss1.eof() << endl;
  tracer() << "fail = " << !!ss1.fail() << endl;
  tracer() << "bad = " << !!ss1.bad() << endl;

  OTC_String i2;

  i2 += "   ABCD   ";
  i2 += char(0xff);
  i2 += "   EFGH   ";
  i2 += "   IJKL   ";

  tracer() << "i2 = \"" << i2 << "\"" << endl;
  tracer() << "i2.length() = " << i2.length() << endl;

  istrstream ss2((char*)i2.string(),i2.length());

  tracer() << "good = " << !!ss2.good() << endl;
  tracer() << "eof = " << !!ss2.eof() << endl;
  tracer() << "fail = " << !!ss2.fail() << endl;
  tracer() << "bad = " << !!ss2.bad() << endl;

  while (ss2.good())
  {
    OTC_String s1;

    s1.readToken(ss2);

    if (!ss2.fail())
    {
      tracer() << "s1 = \"" << s1 << "\"" << endl;
      tracer() << "s1.length() = " << s1.length() << endl;

      tracer() << "good = " << !!ss2.good() << endl;
      tracer() << "eof = " << !!ss2.eof() << endl;
      tracer() << "fail = " << !!ss2.fail() << endl;
      tracer() << "bad = " << !!ss2.bad() << endl;
    }
    else
    {
      tracer() << "s1 (fail) = \"" << s1 << "\"" << endl;
      tracer() << "s1.length() = " << s1.length() << endl;
    }
  }

  tracer() << "good = " << !!ss2.good() << endl;
  tracer() << "eof = " << !!ss2.eof() << endl;
  tracer() << "fail = " << !!ss2.fail() << endl;
  tracer() << "bad = " << !!ss2.bad() << endl;
}

// = TEST 1
//     All tests.

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
  test10();
  test11();
  test12();
  test13();
  test14();
  test15();
  test16();
  test17();
  test18();
  test19();
  test20();
  test21();
  test22();
  test23();
  test24();
  test25();
  test26();
  test27();
  test28();
  test29();
  test30();
  test31();
  test32();
  test33();
  test34();
  test35();
  test36();
  test37();
  test38();
  test39();
  test40();
  test41();
  test42();
  test43();
  test44();
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
  test9,
  test10,
  test11,
  test12,
  test13,
  test14,
  test15,
  test16,
  test17,
  test18,
  test19,
  test20,
  test21,
  test22,
  test23,
  test24,
  test25,
  test26,
  test27,
  test28,
  test29,
  test30,
  test31,
  test32,
  test33,
  test34,
  test35,
  test36,
  test37,
  test38,
  test39,
  test40,
  test41,
  test42,
  test43,
  test44
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
