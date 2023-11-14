/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     text/otcrecord.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1997-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/text/record.hh"
#endif
#endif

#include <OTC/text/record.hh>
#include <OTC/collctn/cbitset.hh>

#include <OTC/stdlib/strstream.h>

#include <string.h>
#include <ctype.h>

/* ------------------------------------------------------------------------- */
OTC_Record::OTC_Record(OTC_String const& theString)
 : record_(theString)
{
  split_(" \n\t\r\f",false);
}

/* ------------------------------------------------------------------------- */
OTC_Record::OTC_Record(OTC_String const& theString, char theFS)
 : record_(theString)
{
  split_(theFS,true);
}

/* ------------------------------------------------------------------------- */
OTC_Record::OTC_Record(OTC_String const& theString, OTC_String const& theFS)
 : record_(theString)
{
  OTCLIB_ENSURE_FN((!theFS.isEmpty()),
   "OTC_Record::OTC_Record(OTC_String const&, OTC_String const&)",
   "must supply at least one separator");

  split_(theFS,false);
}

/* ------------------------------------------------------------------------- */
OTC_Record::OTC_Record(OTC_Record const& theRecord)
 : record_(theRecord.record_), fields_(theRecord.fields_)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_Record& OTC_Record::operator=(OTC_Record const& theRecord)
{
  if (&theRecord != this)
  {
    record_ = theRecord.record_;
    fields_ = theRecord.fields_;
  }

  return *this;
}

/* ------------------------------------------------------------------------- */
OTC_String const& OTC_Record::field(u_int theIndex) const
{
  if (theIndex == 0)
    return record_;

  return fields_.item(theIndex-1);
}

/* ------------------------------------------------------------------------- */
void OTC_Record::split_(OTC_String const& theFS, bool isAwk)
{
  // Construct bitset for field separators.

  OTC_CBitSet ccl(theFS.string(),theFS.length());

  // Cache pointer to string to split up. Also grab the length.

  char const* theString = record_.string();
  u_int theLength = record_.length();

  // If not using AWK semantics then skip field separators at start of
  // record.

  if (isAwk == false)
  {
    while (theLength > 0 && ccl.test(*theString) != 0)
    {
      theLength--;
      theString++;
    }
  }

  while (theLength > 0)
  {
    // Find what is in field.

    char const* tmpString1 = theString;
    while (theLength > 0 && ccl.test(*theString) == 0)
    {
      theLength--;
      theString++;
    }

    OTC_CString tmpString2(tmpString1,theString-tmpString1);
    fields_.addLast(tmpString2);

    if (theLength > 0)
    {
      if (isAwk != false)
      {
	theString++;
	theLength--;
	if (theLength ==0)
	  fields_.addLast(OTC_String::nullString());
      }
      else
      {
	while (theLength > 0 && ccl.test(*theString) != 0)
	{
	  theLength--;
	  theString++;
	}
      }
    }
  }
}

/* ------------------------------------------------------------------------- */
