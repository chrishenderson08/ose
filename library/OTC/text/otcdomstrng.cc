/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     text/otcdomstrng.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/text/domstrng.hh"
#endif
#endif

#include <OTC/text/domstrng.hh>

/* ------------------------------------------------------------------------- */
OTC_DOMString::~OTC_DOMString()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_DOMString::OTC_DOMString(char const* theString, size_t theLength)
  : string_(theString), length_(theLength)
{
  OTCLIB_ENSURE_FN((theLength == 0 || theString != 0),
   "OTC_DOMString(char const*, size_t)",
   "null string but non zero length");

  if (string_ == 0)
    string_ = "";
}

/* ------------------------------------------------------------------------- */
OSE_EXPORT ostream& operator<<(ostream& outs, OTC_DOMString const& theString)
{
  // Special streams prefix operations.

#if 0
  if (!outs.opfx())
    return outs;
#endif

  // If number of characters in string greater than width defined in
  // stream then just write it out. If not we must do some formatting.

  size_t theWidth = outs.width();
  size_t theLength = theString.length();

  if (theLength >= theWidth)
  {
    outs.write(theString.string(),theLength);
  }
  else
  {
    if (!(outs.flags() & ios::left))
    {
      for (int i=0; i<int(theWidth-theLength); i++)
	outs.put((char)outs.fill());
    }
    outs.write(theString.string(),theLength);
    if (outs.flags() & ios::left)
    {
      for (int i=0; i<int(theWidth-theLength); i++)
	outs.put((char)outs.fill());
    }
  }

  // Reset the stream width setting.

  outs.width(0);

  // Sepcial streams suffix operations.

#if 0
  outs.osfx();
#endif

  return outs;
}

/* ------------------------------------------------------------------------- */
