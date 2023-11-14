/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     text/otcoptions.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1992 OTC LIMITED
//     Copyright 1996-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/text/options.hh"
#endif
#endif

#include <OTC/text/options.hh>

#include <ctype.h>

/* ------------------------------------------------------------------------- */
OTC_Options::OTC_Options()
 : offset_(0)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_Options::OTC_Options(int argc, char* argv[])
 : offset_(0)
{
  initialise(argc,argv);
}

/* ------------------------------------------------------------------------- */
OTC_Options::OTC_Options(char const* theArgs)
 : offset_(0)
{
  initialise(theArgs);
}

/* ------------------------------------------------------------------------- */
OTC_Options::OTC_Options(OTC_Options const& theOptions)
{
  offset_ = theOptions.offset_;
  options_ = theOptions.options_;
}

/* ------------------------------------------------------------------------- */
OTC_Options& OTC_Options::operator=(OTC_Options const& theOptions)
{
  offset_ = theOptions.offset_;
  options_ = theOptions.options_;
  return *this;
}

/* ------------------------------------------------------------------------- */
void OTC_Options::initialise(int argc, char* argv[])
{
  OTCLIB_ENSURE_FN((argv != 0 && argc >= 0),
   "OTC_Options::initialise(int, char*[])",
   "invalid arguments");

  offset_ = 0;
  options_.removeAll();
  for (int i=0; i<argc; i++)
    options_.addLast(argv[i]);
}

/* ------------------------------------------------------------------------- */
void OTC_Options::initialise(char const* theArgs)
{
  OTCLIB_ENSURE_FN((theArgs != 0),
   "OTC_Options::initialise(char const*)",
   "invalid argument");

  offset_ = 0;
  options_.removeAll();

  // Skip initial whitespace.

  while (*theArgs && isspace(*theArgs))
    theArgs++;
  if (!*theArgs)
    return;

  // Now for the rest.

  OTC_String anArg;
  bool inString = false;
  while (*theArgs)
  {
    if (!inString && isspace(*theArgs))
    {
      options_.addLast(anArg);
      anArg.truncate(0);
      while (*theArgs && isspace(*theArgs))
	theArgs++;
      if (!*theArgs)
	return;
      theArgs--;
    }
    else
    {
      switch (*theArgs)
      {
	case '"':
	  inString = inString != false ? false : true;
	  break;

	case '\\':
	  if (*(theArgs+1))
	  {
	    theArgs++;
	    anArg += *theArgs;
	  }
	  break;

	default:
	  anArg += *theArgs;
	  break;
      }
    }
    theArgs++;
  }
  options_.addLast(anArg);
}

/* ------------------------------------------------------------------------- */
u_int OTC_Options::numOptions() const
{
  int theNum = options_.population() - offset_;
  return theNum < 0 ? 0 : theNum;
}

/* ------------------------------------------------------------------------- */
OTC_String const& OTC_Options::option(u_int theNum) const
{
  OTCLIB_ENSURE_FN((theNum != 0 && theNum <= numOptions()),
   "OTC_Options::option(u_int)",
   "index out of range");

  return options_.item(offset_+theNum-1);
}

/* ------------------------------------------------------------------------- */
OTC_Iterator<OTC_String> OTC_Options::options(
 OTC_Direction theDirection,
 OTC_Protection theProtection
) const
{
  OTC_Range range(offset_,numOptions());
  return options_.items(range,theDirection,theProtection);
}

/* ------------------------------------------------------------------------- */
OTC_Iterator<OTC_String> OTC_Options::options(
 u_int theStart,
 u_int theLength,
 OTC_Direction theDirection,
 OTC_Protection theProtection
) const
{
  OTC_Range range(offset_+theStart,theLength);
  return options_.items(range,theDirection,theProtection);
}

/* ------------------------------------------------------------------------- */
OTC_Iterator<OTC_String> OTC_Options::options(
 OTC_Range const& theRange,
 OTC_Direction theDirection,
 OTC_Protection theProtection
) const
{
  OTC_Range range(offset_+theRange.lower(),theRange.length());
  return options_.items(range,theDirection,theProtection);
}

/* ------------------------------------------------------------------------- */
