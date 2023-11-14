/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/_svpyload.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/svpyload.hh>

#include <OTC/debug/tracer.hh>
#include <OTC/stdlib/fstream.h>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

void test2()
{
  OTC_Tracer tracer("void test2()");

  // Empty object.

  OTC_SVPayload theObject;

  theObject.setName("name");

  tracer() << "node=" << theObject.nodeType() << endl;
  tracer() << "name=" << theObject.nodeName() << endl;

  tracer() << "value=" << theObject.value() << endl;

  tracer() << endl;

  tracer() << theObject << endl;

  tracer() << theObject.encode() << endl;
}

void test3()
{
  OTC_Tracer tracer("void test3()");

  // Simple value.

  OTC_SVPayload theObject;

  theObject.setName("name");

  theObject <<= "value";

  tracer() << endl;

  tracer() << "node=" << theObject.nodeType() << endl;
  tracer() << "name=" << theObject.nodeName() << endl;

  tracer() << "value=" << theObject.value() << endl;

  tracer() << theObject << endl;

  tracer() << theObject.encode() << endl;
}

void test4()
{
  OTC_Tracer tracer("void test4()");

  // Simple array.

  OTC_SVPayload theObject;

  theObject.setName("name");

  theObject[0] <<= "item0";
  theObject[1] <<= "item1";
  theObject[2] <<= "item2";

  tracer() << "node=" << theObject.nodeType() << endl;
  tracer() << "name=" << theObject.nodeName() << endl;

  tracer() << "value=" << theObject.value() << endl;

  tracer() << endl;

  tracer() << theObject << endl;

  tracer() << theObject.firstChild() << endl;
  tracer() << theObject[-1] << endl;
}

void test5()
{
  OTC_Tracer tracer("void test5()");

  // Array with user defined indicies.
  // Can be sparse.

  OTC_SVPayload theObject;

  OTC_RWPayload theWriter;

  theWriter = theObject[-1];
  theWriter <<= "item0";
  theWriter.setProperty("index","0");

  theWriter = theObject[-1];
  theWriter <<= "item2";
  theWriter.setProperty("index","2");

  theWriter = theObject[-1];
  theWriter <<= "item4";
  theWriter.setProperty("index","4");

  tracer() << theObject << endl;

  tracer() << theObject.firstChild() << endl;
  tracer() << theObject[1] << endl;
}

void test6()
{
  OTC_Tracer tracer("void test6()");

  // Array traveral.

  OTC_SVPayload theObject;

  OTC_RWPayload theWriter;

  theWriter = theObject[-1];
  theWriter <<= "item0";
  theWriter.setProperty("index","0");

  theWriter = theObject[-1];
  theWriter <<= "item2";
  theWriter.setProperty("index","2");

  theWriter = theObject[-1];
  theWriter <<= "item4";
  theWriter.setProperty("index","4");

  OTC_ROPayload theReader;

  theReader = theObject.firstChild();

  while (theReader.isValid())
  {
    tracer() << "index=" << theReader.property("index") << endl;

    tracer() << endl;

    tracer() << theReader << endl;

    theReader = theReader.nextSibling();
  }
}

void test7()
{
  OTC_Tracer tracer("void test7()");

  // Simple object.

  OTC_SVPayload theObject;

  theObject["one"] <<= "text1";
  theObject["two"] <<= "text2";
  theObject["six"] <<= "text3";

  tracer() << "node=" << theObject.nodeType() << endl;
  tracer() << "name=" << theObject.nodeName() << endl;

  tracer() << "value=" << theObject.value() << endl;

  tracer() << endl;

  tracer() << theObject << endl;

  tracer() << theObject["one"] << endl;
}

void test8()
{
  OTC_Tracer tracer("void test8()");

  // Complex object.

  OTC_SVPayload theObject;

  theObject["one"] <<= "text1";
  theObject["two"] <<= "text2";

  theObject["six"][0] <<= "text3";
  theObject["six"][1] <<= "text4";

  theObject["ten.one"] <<= "text5";
  theObject["ten"]["two"] <<= "text6";

  theObject["ten"]["six"][0] <<= "XXX";
  theObject["ten.six"][0] <<= "text7";
  theObject["ten.six"][1] <<= "text8";
  theObject["ten"]["six"][2] <<= "text9";

  tracer() << theObject << endl;
}

void test9()
{
  OTC_Tracer tracer("void test9()");

  // Properties.

  OTC_SVPayload theObject;

  theObject["one"] <<= "text1";
  theObject["two"] <<= "text2";

  theObject["one"].setProperty("id","1");

  theObject["two"].setProperty("-id","2");
  theObject["two"].setProperty("name","six");

  tracer() << theObject << endl;
}

void test10()
{
  OTC_Tracer tracer("void test10()");

  // Content encoding.

  OTC_SVPayload theObject;

  theObject[-1] <<= "text &<>\r\n\t'\" text";
  theObject[-1].setProperty("data","text &<>\r\n\t'\" text");

  tracer() << theObject << endl;
}

void test11()
{
  OTC_Tracer tracer("void test11()");

  // Reassigning of element.

  OTC_SVPayload theObject;

  theObject <<= "text";

  tracer() << theObject << endl;

  theObject[-1] <<= "text";

  tracer() << theObject << endl;

  theObject["one"] <<= "text";

  tracer() << theObject << endl;

  theObject[1] <<= "text";

  tracer() << theObject << endl;
}

void test12()
{
  OTC_Tracer tracer("void test12()");

  // Typed value.

  OTC_SVPayload theObject;

  theObject <<= 1;

  tracer() << "type=" << theObject.valueType() << endl;

  tracer() << endl;

  tracer() << theObject << endl;

  theObject[-1] <<= -1;

  tracer() << "type=" << theObject[-1].valueType() << endl;

  tracer() << endl;

  tracer() << theObject << endl;

  theObject["one"] <<= 1.5;

  tracer() << "type=" << theObject["one"].valueType() << endl;

  tracer() << endl;

  tracer() << theObject << endl;

  theObject[1] <<= -1.5;

  tracer() << "type=" << theObject[1].valueType() << endl;

  tracer() << endl;

  tracer() << theObject << endl;
}

void test13()
{
  OTC_Tracer tracer("void test13()");

  // Copy constructors.

  OTC_SVPayload theObject1;

  theObject1["one"] <<= "text1";

  theObject1["two"][-1] <<= "text2";
  theObject1["two"][-1] <<= "text3";

  theObject1["six.one"] <<= "text4";
  theObject1["six.two"] <<= "text5";

  OTC_SVPayload theObject2(theObject1);

  tracer() << theObject2 << endl;

  OTC_SVPayload theObject3(theObject1["six"]);

  tracer() << theObject3 << endl;
}

void test14()
{
  OTC_Tracer tracer("void test14()");

  // Element assignment.

  OTC_SVPayload theObject;

  theObject["one"] <<= "text1";

  theObject["two"][-1] <<= "text2";
  theObject["two"][-1] <<= "text3";

  theObject["six.one"] <<= "text4";
  theObject["six.two"] <<= "text5";

  tracer() << theObject << endl;

  theObject["ten"] <<= theObject;

  tracer() << theObject << endl;

  theObject <<= theObject["six"];

  tracer() << theObject << endl;

  theObject["one"] <<= theObject["one"];

  tracer() << theObject << endl;

  theObject <<= OTC_ROPayload(0);

  tracer() << theObject << endl;

  theObject <<= theObject;

  tracer() << theObject << endl;

  theObject["one"] <<= theObject["one"];

  tracer() << theObject << endl;
}

void test15()
{
  OTC_Tracer tracer("void test15()");

  // Property search.

  OTC_SVPayload theObject1;

  theObject1["one"] <<= "text1";
  theObject1["one"].setProperty("id","1");

  theObject1["two"][-1] <<= "text2";
  theObject1["two"][-1].setProperty("id","1");

  theObject1["two"][-1] <<= "text3";
  theObject1["two"][-1].setProperty("id","2");

  theObject1["two"].setProperty("id","2");

  theObject1["six.one"] <<= "text4";
  theObject1["six.one"].setProperty("id","1");

  theObject1["six.two"] <<= "text5";
  theObject1["six.two"].setProperty("id","2");

  theObject1["six"].setProperty("id","6");

  tracer() << theObject1 << endl;

  OTC_ROPayload theObject2;

  theObject2 = theObject1.findProperty("id","?");

  if (theObject2.isValid())
    tracer() << theObject2 << endl;

  theObject2 = theObject1.findProperty("id","2");

  if (theObject2.isValid())
    tracer() << theObject2 << endl;

  theObject2 = theObject1.findProperty("id","6");

  if (theObject2.isValid())
    tracer() << theObject2 << endl;

  theObject2 = theObject1["six"].findProperty("id","2");

  if (theObject2.isValid())
    tracer() << theObject2 << endl;
}

void test16()
{
  OTC_Tracer tracer("void test16()");

  // Test all types.

  OTC_SVPayload theObject;

  theObject["false"] <<= false;
  theObject["true"] <<= true;

  theObject["char"] <<= char(0x7F);
  theObject["short"] <<= short(0x7FFF);
  theObject["int"] <<= int(0x7FFFFFFF);
  theObject["long"] <<= long(0x7FFFFFFF);

  theObject["-char"] <<= char(0xFF);
  theObject["-short"] <<= short(0xFFFF);
  theObject["-int"] <<= int(0xFFFFFFFF);
  theObject["-long"] <<= long(0xFFFFFFFF);

  theObject["u_char"] <<= u_char(0xFF);
  theObject["u_short"] <<= u_short(0xFFFF);
  theObject["u_int"] <<= u_int(0xFFFFFFFF);
  theObject["u_long"] <<= u_long(0xFFFFFFFF);

  theObject["float"] <<= float(1.5);
  theObject["double"] <<= double(1.5);

  theObject["bool[]"][-1] <<= false;
  theObject["bool[]"][-1] <<= true;

  theObject["char[]"][-1] <<= char(0x7F);
  theObject["char[]"][-1] <<= char(0xFF);
  theObject["char[]"][-1] <<= u_char(0xFF);

  theObject["short[]"][-1] <<= short(0x7FFF);
  theObject["short[]"][-1] <<= short(0xFFFF);
  theObject["short[]"][-1] <<= u_short(0xFFFF);

  theObject["int[]"][-1] <<= int(0x7FFFFFFF);
  theObject["int[]"][-1] <<= int(0xFFFFFFFF);
  theObject["int[]"][-1] <<= u_int(0xFFFFFFFF);

  theObject["long[]"][-1] <<= long(0x7FFFFFFF);
  theObject["long[]"][-1] <<= long(0xFFFFFFFF);
  theObject["long[]"][-1] <<= u_long(0xFFFFFFFF);

  theObject["float[]"][-1] <<= float(1.5);
  theObject["float[]"][-1] <<= float(-1.5);
  theObject["float[]"][-1] <<= float(1e+234);
  theObject["float[]"][-1] <<= float(-1e+234);
  theObject["float[]"][-1] <<= float(1e-234);
  theObject["float[]"][-1] <<= float(-1e-234);

  theObject["double[]"][-1] <<= double(1.5);
  theObject["double[]"][-1] <<= double(-1.5);
  theObject["double[]"][-1] <<= double(1e+234);
  theObject["double[]"][-1] <<= double(-1e+234);
  theObject["double[]"][-1] <<= double(1e-234);
  theObject["double[]"][-1] <<= double(-1e-234);

  theObject["binary"][-1].assign("0123456789","xsd:base64Binary");
  theObject["binary"][-1].assign("aAbBcCdDeE","xsd:base64Binary");
  theObject["binary"][-1].assign(" \t\r\n\v ","xsd:base64Binary");
  theObject["binary"][-1].assign("\001\002","xsd:base64Binary");

  OTC_Date theDate1(1,1,2001);
  OTC_Date theDate2(1,7,2001);
  OTC_Time theTime1(theDate1,1,1,1);
  OTC_Duration theDuration(theTime1-theDate2);

  theObject["date"] <<= theDate1;
  theObject["dateTime"] <<= theTime1;
  theObject["duration"] <<= theDuration;

  tracer() << theObject << endl;
}

void test17()
{
  OTC_Tracer tracer("void test17()");

  // Self assignment of non existant elements.

  OTC_SVPayload theObject;

  theObject["one"] <<= theObject["one"];

  tracer() << theObject << endl;
}


void test18()
{
  OTC_Tracer tracer("void test18()");

  // Null elements.

  OTC_SVPayload theObject;

  theObject["one"] <<= OTC_ROPayload::nullValue();

  theObject["two"][0] <<= OTC_ROPayload::nullValue();
  theObject["two"][1] <<= OTC_ROPayload::nullValue();
  theObject["two"][-1] <<= OTC_ROPayload::nullValue();

  tracer() << theObject << endl;
}

void test19()
{
  OTC_Tracer tracer("void test19()");

  // Pruning.

  OTC_SVPayload theObject;

  theObject["1.1"] <<= "1.1";
  theObject["1.2"] <<= "1.2";
  theObject["1.3"] <<= "1.3";
  theObject["2.1"] <<= "2.1";

  tracer() << theObject << endl;

#if 0
  theObject.pruneChild("1.3");

  tracer() << theObject << endl;

  theObject["1"].pruneChild("2");

  tracer() << theObject << endl;

  theObject.pruneChild("1.1");
  theObject.pruneChild("2.1");

  tracer() << theObject << endl;
#endif
}

void test20()
{
  OTC_Tracer tracer("void test20()");

  // Reading typed values.

  OTC_String theString;
  bool theBool;
  char theChar;
  short theShort;
  int theInt;
  long theLong;
  float theFloat;
  double theDouble;

  bool theStatus;
  OTC_ROPayload theReader;

  OTC_SVPayload theObject;

  theObject["null"] <<= OTC_ROPayload::nullValue();
  theObject["string0"] <<= "";
  theObject["string1"] <<= "X";
  theObject["false"] <<= false;
  theObject["true"] <<= true;
  theObject["char"] <<= char(100);
  theObject["short"] <<= short(10000);
  theObject["int"] <<= long(1000000);
  theObject["float"] <<= float(1000000);
  theObject["double"] <<= double(1000000);

  tracer() << theObject << endl;

  // String.

  theReader = theObject.firstChild();
  while (theReader.isValid())
  {
    theStatus = true;
    theString.truncate();

    (theReader,theStatus) >>= theString;

    tracer() << "node=" << theReader.nodeName() << endl;
    tracer() << "status=" << theStatus << endl;

    if (theStatus)
      tracer() << "string=" << theString << endl;

    theReader = theReader.nextSibling();
  }

  tracer() << endl;


  // Boolean.

  theReader = theObject.firstChild();
  while (theReader.isValid())
  {
    theStatus = true;
    theBool = false;

    (theReader,theStatus) >>= theBool;

    tracer() << "node=" << theReader.nodeName() << endl;
    tracer() << "status=" << theStatus << endl;

    if (theStatus)
      tracer() << "bool=" << theBool << endl;

    theReader = theReader.nextSibling();
  }

  tracer() << endl;

  // Char.

  theReader = theObject.firstChild();
  while (theReader.isValid())
  {
    theStatus = true;
    theChar = '?';

    (theReader,theStatus) >>= theChar;

    tracer() << "node=" << theReader.nodeName() << endl;
    tracer() << "status=" << theStatus << endl;

    if (theStatus)
      tracer() << "char=" << int(theChar) << endl;

    theReader = theReader.nextSibling();
  }

  tracer() << endl;

  // Short.

  theReader = theObject.firstChild();
  while (theReader.isValid())
  {
    theStatus = true;
    theShort = 0;

    (theReader,theStatus) >>= theShort;

    tracer() << "node=" << theReader.nodeName() << endl;
    tracer() << "status=" << theStatus << endl;

    if (theStatus)
      tracer() << "short=" << theShort << endl;

    theReader = theReader.nextSibling();
  }

  tracer() << endl;

  // Integer.

  theReader = theObject.firstChild();
  while (theReader.isValid())
  {
    theStatus = true;
    theInt = 0;

    (theReader,theStatus) >>= theInt;

    tracer() << "node=" << theReader.nodeName() << endl;
    tracer() << "status=" << theStatus << endl;

    if (theStatus)
      tracer() << "int=" << theInt << endl;

    theReader = theReader.nextSibling();
  }

  tracer() << endl;

  // Long.

  theReader = theObject.firstChild();
  while (theReader.isValid())
  {
    theStatus = true;
    theLong = 0;

    (theReader,theStatus) >>= theLong;

    tracer() << "node=" << theReader.nodeName() << endl;
    tracer() << "status=" << theStatus << endl;

    if (theStatus)
      tracer() << "long=" << theLong << endl;

    theReader = theReader.nextSibling();
  }

  tracer() << endl;

  // Float.

  theReader = theObject.firstChild();
  while (theReader.isValid())
  {
    theStatus = true;
    theFloat = 0;

    (theReader,theStatus) >>= theFloat;

    tracer() << "node=" << theReader.nodeName() << endl;
    tracer() << "status=" << theStatus << endl;

    if (theStatus)
      tracer() << "float=" << theFloat << endl;

    theReader = theReader.nextSibling();
  }

  tracer() << endl;

  // Double.

  theReader = theObject.firstChild();
  while (theReader.isValid())
  {
    theStatus = true;
    theDouble = 0;

    (theReader,theStatus) >>= theDouble;

    tracer() << "node=" << theReader.nodeName() << endl;
    tracer() << "status=" << theStatus << endl;

    if (theStatus)
      tracer() << "double=" << theDouble << endl;

    theReader = theReader.nextSibling();
  }

  tracer() << endl;

  // Typical use.

  theObject["name"] <<= "value";
  theObject["count"] <<= 100;
  theObject["average"] <<= 49.9;

  tracer() << theObject << endl;

  theStatus = true;

  (theObject["name"],theStatus) >>= theString;
  (theObject["count"],theStatus) >>= theLong;
  (theObject["average"],theStatus) >>= theDouble;

  if (theStatus)
  {
    tracer() << "name=" << theString << endl;
    tracer() << "count=" << theLong << endl;
    tracer() << "average=" << theDouble << endl;
  }

  // Invalid node.

  theStatus = true;

  (theObject["description"],theStatus) >>= theString;

  if (theStatus)
    tracer() << "description=" << theString << endl;
}

void test21()
{
  OTC_Tracer tracer("void test21()");

  // Input into lists.

  OTC_SVPayload theObject;

  theObject[-1] <<= 1;
  theObject[-1] <<= 2;
  theObject[-1] <<= 3;
  theObject[-1] <<= 4;
  theObject[-1] <<= 5;

  tracer() << theObject << endl;

  bool theStatus;

  OTC_Deque<OTC_String> theDeque;

  theStatus = true;

  (theObject,theStatus) >>= theDeque;

  if (theStatus)
  {
    OTC_Iterator<OTC_String> theItems;
    theItems = theDeque.items();
    while (theItems.isValid())
    {
      tracer() << theItems.item() << endl;
      theItems.next();
    }
  }

  OTC_List<long> theList;

  theStatus = true;

  (theObject,theStatus) >>= theList;

  if (theStatus)
  {
    OTC_Iterator<long> theItems;
    theItems = theList.items();
    while (theItems.isValid())
    {
      tracer() << theItems.item() << endl;
      theItems.next();
    }
  }

  OTC_HMap<OTC_String,OTC_String> theMap;

  theStatus = true;

  (theObject,theStatus) >>= theMap;

  if (theStatus)
  {
    OTC_PairIterator<OTC_String,OTC_String> thePairs;
    thePairs = theMap.pairs();
    while (thePairs.isValid())
    {
      tracer() << thePairs.key() << " " << thePairs.item() << endl;
      thePairs.next();
    }
  }

  theObject[-1] <<= theObject;

  tracer() << theObject << endl;

  theStatus = true;

  (theObject,theStatus) >>= theDeque;

  if (theStatus)
  {
    OTC_Iterator<OTC_String> theItems;
    theItems = theDeque.items();
    while (theItems.isValid())
    {
      tracer() << theItems.item() << endl;
      theItems.next();
    }
  }
}

void test22()
{
  OTC_Tracer tracer("void test22()");

  // Input into maps.

  OTC_SVPayload theObject;

  theObject["one"] <<= 1;
  theObject["two"] <<= 2;
  theObject["three"] <<= 3;
  theObject["four"] <<= 4;
  theObject["five"] <<= 5;

  tracer() << theObject << endl;

  bool theStatus;

  OTC_HMap<OTC_String,OTC_String> theHMap;

  theStatus = true;

  (theObject,theStatus) >>= theHMap;

  if (theStatus)
  {
    OTC_PairIterator<OTC_String,OTC_String> thePairs;
    thePairs = theHMap.pairs();
    while (thePairs.isValid())
    {
      tracer() << thePairs.key() << " " << thePairs.item() << endl;
      thePairs.next();
    }
  }

  OTC_TMap<OTC_String,long> theTMap;

  theStatus = true;

  (theObject,theStatus) >>= theTMap;

  if (theStatus)
  {
    OTC_PairIterator<OTC_String,long> thePairs;
    thePairs = theTMap.pairs();
    while (thePairs.isValid())
    {
      tracer() << thePairs.key() << " " << thePairs.item() << endl;
      thePairs.next();
    }
  }
}

void test23()
{
  OTC_Tracer tracer("void test23()");

  // Date, time and duration input.

  OTC_Date theDate(1,1,2001);
  OTC_Time theTime(theDate,1,1,1);
  OTC_Duration theDuration(theTime-theDate);

  OTC_SVPayload theObject;

  theObject["date"] <<= theDate;
  theObject["dateTime"] <<= theTime;
  theObject["duration"] <<= theDuration;

  tracer() << theObject << endl;

  bool theStatus;

  theStatus = true;

  (theObject["date"],theStatus) >>= theDate;

  if (theStatus)
    tracer() << "date=" << theDate << endl;

  (theObject["dateTime"],theStatus) >>= theTime;

  if (theStatus)
    tracer() << "dateTime=" << theTime << endl;

  (theObject["duration"],theStatus) >>= theDuration;

  if (theStatus)
    tracer() << "duration=" << theDuration << endl;
}

void test24()
{
  OTC_Tracer tracer("void test24()");

  // Output using iterators.

  OTC_HMap<OTC_String,int> theMap;

  tracer() << OTC_ROPayload::nullValue() << endl;
  tracer() << OTC_ROPayload::nullArray() << endl;
  tracer() << OTC_ROPayload::nullIndex() << endl;

  OTC_SVPayload theObject;

  theObject["key-iterator"] <<= theMap.keys();
  theObject["item-iterator"] <<= theMap.items();
  theObject["pair-iterator"] <<= theMap.pairs();

  tracer() << theObject << endl;

  theMap.add("one",1);
  theMap.add("two",2);
  theMap.add("three",3);
  theMap.add("four",4);
  theMap.add("five",5);

  theObject["key-iterator"] <<= theMap.keys();
  theObject["item-iterator"] <<= theMap.items();
  theObject["pair-iterator"] <<= theMap.pairs();

  tracer() << theObject << endl;
}

void test25()
{
  OTC_Tracer tracer("void test25()");

  OTC_SVPayload theObject;

  tracer() << theObject << endl;

  theObject.sibling("1.1") <<= "1.1";
  theObject.sibling("1.2") <<= "1.2";
  theObject.sibling("1.3") <<= "1.3";

  tracer() << theObject << endl;

  OTC_HMap<OTC_String,OTC_String> theMap;
  bool theStatus;

  theStatus = true;
  (theObject,theStatus) >>= theMap;

  theObject["1.0"] <<= theMap.pairs();

  tracer() << theObject << endl;

  theObject <<= OTC_ROPayload::nullIndex();

  theObject["1.1"] <<= "1.1";
  theObject["1.2"] <<= "1.2";
  theObject["1.3"] <<= "1.3";

  theStatus = true;
  (theObject["1"],theStatus) >>= theMap;

  theObject["1.0"] <<= theMap.pairs();

  tracer() << theObject << endl;
}

void test26()
{
  OTC_Tracer tracer("void test26()");

  OTC_SVPayload theObject;

  theObject.setName("name");

  theObject[0] <<= "item0";
  theObject[1] <<= "item1";
  theObject[2] <<= "item2";

  tracer() << theObject << endl;

  OTC_Iterator<OTC_String> theItems;

  bool theStatus;

  theStatus = true;
  (theObject,theStatus) >>= theItems;

  while (theItems.isValid())
  {
    tracer() << theItems.item() << endl;

    theItems.next();
  }
}

void test27()
{
  OTC_Tracer tracer("void test27()");

  OTC_SVPayload theObject;

  theObject.setName("name");

  theObject["0"] <<= "item0";
  theObject["1"] <<= "item1";
  theObject["2"] <<= "item2";

  tracer() << theObject << endl;

  OTC_PairIterator<OTC_String,OTC_String> thePairs;

  bool theStatus;

  theStatus = true;
  (theObject,theStatus) >>= thePairs;

  while (thePairs.isValid())
  {
    tracer() << thePairs.key() << " - " << thePairs.item() << endl;

    thePairs.next();
  }
}

void test28()
{
  OTC_Tracer tracer("void test28()");

  OTC_SVPayload theObject;

  tracer() << theObject << endl;

  ifstream ins("_svpyload.xml");

  OTC_String theContent;
  theContent.readFile(ins);

  theObject.decode(theContent);

  if (ins.fail())
    cout << "DECODE FAILED!" << endl << endl;

  tracer() << theObject << endl;

  OTC_String theString;

  ins.clear();

  theString.readFile(ins);

  tracer() << theString << endl;
}

void test29()
{
  OTC_Tracer tracer("void test29()");

  ifstream ins("/usr/dict/words");

  OTC_String theLine;

  OTC_SVPayload theObject;

  theObject.setName("dictionary");

  u_int i = 1;

  while (ins.good())
  {
    theLine.readLine(ins);

    if (!ins.fail())
    {
      theObject[-1] <<= theLine;

      char buf[32];
      sprintf(buf,"%d",i++);

      theObject[-1].setProperty("id",buf);
    }
  }

  tracer() << theObject << endl;
}

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
  test29
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
