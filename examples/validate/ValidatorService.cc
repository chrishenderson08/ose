/*
// ============================================================================
//
// = FILENAME
//     ValidatorService.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include "ValidatorService.hh"

/* ------------------------------------------------------------------------- */
TST_ValidatorService::TST_ValidatorService(
 char const* theServiceName,
 char const* theServiceAudience
)
  : ONS_Service(theServiceName,theServiceAudience,OTCLIB_SERVICE_HIDDEN)
{
  // Subscribe to the startup callback. There
  // is no need for the session manager to
  // subscribe to shutdown events because the
  // individual active session objects will do
  // that. The code which creates the session
  // manager will still though need to call the
  // <scheduleStartup()> method for anything
  // to happen.

  subscribeStartup(&TST_ValidatorService::serviceActivation);
}

/* ------------------------------------------------------------------------- */
void TST_ValidatorService::serviceActivation()
{
  // Only use a typedef for "THIS" here so that
  // everything fits on one line. There is
  // nothing special about the use of the
  // typedef and using the name of the class
  // explicitly is how it would normally be
  // done.

  typedef TST_ValidatorService THIS;

  exportMethod(&THIS::arrayOfStructsTest,"arrayOfStructsTest");
  exportMethod(&THIS::countTheEntities,"countTheEntities");
  exportMethod(&THIS::easyStructTest,"easyStructTest");
  exportMethod(&THIS::echoStructTest,"echoStructTest");
  exportMethod(&THIS::manyTypesTest,"manyTypesTest");
  exportMethod(&THIS::moderateSizeArrayCheck,"moderateSizeArrayCheck");
  exportMethod(&THIS::nestedStructTest,"nestedStructTest");
  exportMethod(&THIS::simpleStructReturnTest,"simpleStructReturnTest");

  // Service was initially hidden, so must make
  // it visible by announcing its availability.

  announceAvailability();
}

/* ------------------------------------------------------------------------- */
int TST_ValidatorService::arrayOfStructsTest(
 OTC_Deque<OTC_HMap<OTC_String,int> > const& theArray
) const
{
  int theResult = 0;
  OTC_Iterator<OTC_HMap<OTC_String,int> > theItems;

  theItems = theArray.items();
  while (theItems.isValid())
  {
    theResult += theItems.item().item("curly");

    theItems.next();
  }

  return theResult;
}

/* ------------------------------------------------------------------------- */
OTC_PairIterator<OTC_String,int> TST_ValidatorService::countTheEntities(
 char const* theString
) const
{
  OTC_HMap<char,int> theCounts;

  theCounts.add('<',0);
  theCounts.add('>',0);
  theCounts.add('&',0);
  theCounts.add('\'',0);
  theCounts.add('"',0);

  while (*theString != EOS)
  {
    if (theCounts.contains(*theString))
      theCounts.add(*theString,theCounts.item(*theString)+1,OTCLIB_REPLACE);
    else
      theCounts.add(*theString,1);

    theString++;
  }

  OTC_HMap<OTC_String,int> theResult;

  theResult.add("ctLeftAngleBrackets",theCounts.item('<'));
  theResult.add("ctRightAngleBrackets",theCounts.item('>'));
  theResult.add("ctAmpersands",theCounts.item('&'));
  theResult.add("ctApostrophes",theCounts.item('\''));
  theResult.add("ctQuotes",theCounts.item('"'));

  return theResult.pairs();
}

/* ------------------------------------------------------------------------- */
int TST_ValidatorService::easyStructTest(
 OTC_HMap<OTC_String,int> const& theStruct
) const
{
  int theResult = 0;

  theResult += theStruct.item("moe");
  theResult += theStruct.item("larry");
  theResult += theStruct.item("curly");

  return theResult;
}

/* ------------------------------------------------------------------------- */
void TST_ValidatorService::echoStructTest(OTC_ROPayload theArgument) const
{
  // The test definition says that the argument
  // is a struct, but doesn't say what the
  // types of its members are. Because of this,
  // easier to echo the argument back no matter
  // what type it is, even if it isn't a struct
  // at all.

  returnResponse(theArgument);
}

/* ------------------------------------------------------------------------- */
void TST_ValidatorService::manyTypesTest(
 int theInteger,
 bool theBoolean,
 char const* theString,
 double theFloat,
 OTC_Time theDateTime,
 ONS_Binary theBinary
) const
{
  // Because the returned array holds differing
  // types, it is easier to construct the
  // result direct into a message payload
  // object. The use of "[-1]" on the payload
  // object implicitly creates an array and
  // also ensures the new entry is added to the
  // end of the array.

  OTC_SVPayload theResult;

  theResult[-1] <<= theInteger;
  theResult[-1] <<= theBoolean;
  theResult[-1] <<= theString;
  theResult[-1] <<= theFloat;
  theResult[-1] <<= theDateTime;
  theResult[-1] <<= theBinary;

  returnResponse(theResult);
}

/* ------------------------------------------------------------------------- */
OTC_String TST_ValidatorService::moderateSizeArrayCheck(
 OTC_Deque<OTC_String> const& theArray
) const
{
  OTC_String theResult;

  theResult.append(theArray.first());
  theResult.append(theArray.last());

  return theResult;
}

/* ------------------------------------------------------------------------- */
int TST_ValidatorService::nestedStructTest(OTC_ROPayload theArgument) const
{
  // Rather than use a highly nest data
  // structures, go direct into the payload as
  // it is simpler. Don't bother with raising a
  // failure response for bad argument format
  // for the purpose of the test.

  int theValue1 = 0;
  int theValue2 = 0;
  int theValue3 = 0;

  bool theStatus = true;

  (theArgument["2000.04.01.moe"],theStatus) >>= theValue1;
  (theArgument["2000.04.01.curly"],theStatus) >>= theValue2;
  (theArgument["2000.04.01.larry"],theStatus) >>= theValue3;

  theValue1 += theValue2;
  theValue1 += theValue3;

  return theValue1;
}

/* ------------------------------------------------------------------------- */
OTC_PairIterator<OTC_String,int> TST_ValidatorService::simpleStructReturnTest(
 int theValue
) const
{
  OTC_HMap<OTC_String,int> theResult;

  theResult.add("times10",10*theValue);
  theResult.add("times100",100*theValue);
  theResult.add("times1000",1000*theValue);

  return theResult.pairs();
}

/* ------------------------------------------------------------------------- */
