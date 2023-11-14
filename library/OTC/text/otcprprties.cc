/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     text/otcprprties.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1998-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/text/prprties.hh"
#endif
#endif

#include <OTC/text/prprties.hh>

#include <OTC/text/osstream.hh>
#include <OTC/debug/logstrm.hh>

/* ------------------------------------------------------------------------- */
OTC_Properties::~OTC_Properties()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_Properties::OTC_Properties()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_Properties::OTC_Properties(
 OTC_Properties const& theDatabase
)
  : standard_(theDatabase.standard_),
    wildcard_(theDatabase.wildcard_)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void OTC_Properties::removeAll()
{
  standard_.removeAll();
  wildcard_.removeAll();
}

/* ------------------------------------------------------------------------- */
void OTC_Properties::remove(
 char const* theKeyPrefix,
 char const* theKey
)
{
  OTC_String tmpKey;
  
  if (theKeyPrefix != 0)
  {
    tmpKey += theKeyPrefix;
    tmpKey += '.';
  }

  tmpKey += theKey;

  standard_.remove(tmpKey,OTCLIB_UNSAFE);
  wildcard_.remove(tmpKey,OTCLIB_UNSAFE);
}

/* ------------------------------------------------------------------------- */
void OTC_Properties::load(OTC_Properties const& theDatabase)
{
  standard_ = theDatabase.standard_;
  wildcard_ = theDatabase.wildcard_;
}

/* ------------------------------------------------------------------------- */
void OTC_Properties::merge(
 char const* theKeyPrefix,
 OTC_Properties const& theDatabase
)
{
  OTC_PairIterator<OTC_String,OTC_String> thePairs;

  if (theKeyPrefix != 0)
  {
    OTC_String tmpKey;

    thePairs = theDatabase.standard_.pairs();
    while (thePairs.isValid())
    {
      tmpKey.truncate();

      tmpKey += theKeyPrefix;
      tmpKey += '.';
      tmpKey += thePairs.key();

      if (tmpKey.index('*') != -1)
	wildcard_.add(tmpKey,thePairs.item(),OTCLIB_REPLACE);
      else
	standard_.add(tmpKey,thePairs.item(),OTCLIB_REPLACE);

      thePairs.next();
    }

    thePairs = theDatabase.wildcard_.pairs();
    while (thePairs.isValid())
    {
      tmpKey.truncate();

      tmpKey += theKeyPrefix;
      tmpKey += '.';
      tmpKey += thePairs.key();

      wildcard_.add(tmpKey,thePairs.item(),OTCLIB_REPLACE);

      thePairs.next();
    }
  }
  else
  {
    thePairs = theDatabase.standard_.pairs();
    while (thePairs.isValid())
    {
      standard_.add(thePairs.key(),thePairs.item(),OTCLIB_REPLACE);

      thePairs.next();
    }

    thePairs = theDatabase.wildcard_.pairs();
    while (thePairs.isValid())
    {
      wildcard_.add(thePairs.key(),thePairs.item(),OTCLIB_REPLACE);

      thePairs.next();
    }
  }
}

/* ------------------------------------------------------------------------- */
void OTC_Properties::merge(
 char const* theKeyPrefix,
 istream& theStream,
 char const* theChannel
)
{
  OTC_LogStream theLogger;
  theLogger.setChannel(theChannel);

  u_int theLineCount = 0;

  // Since we are reading config files, the
  // length of lines will on average grow out
  // to 80 characters. The default string
  // buffer size is 63 bytes which means that
  // a reallocation will most likely be
  // required. Thus make the string capacity
  // much larger from the outset.

  OTC_Capacity theCapacity(1024);

  OTC_String theLine(theCapacity);
  OTC_String theInput(theCapacity);

  bool isComment;
  isComment = false;

  OTC_CBitSet tmpWhiteSpace(" \t");

  while (theStream.good())
  {
    theLineCount++;

    theLine.readLine(theStream,false);

    if (!theStream.fail())
    {
      if (!theInput.isEmpty())
	theInput += EOL;

      if (!theLine.isEmpty() && theLine[u_int(0)] == '!')
	isComment = true;

      theInput += theLine;

      theInput.ltrim();
      theInput.rtrim(tmpWhiteSpace);
    }

    if (!theInput.isEmpty())
    {
      if (!theStream.fail() && theInput[theInput.length()-1] == '\\')
      {
	theInput.truncate(theInput.length()-1);
      }
      else
      {
	int theIndex;

	if (isComment == false)
	{
	  theIndex = theInput.index(':');
	  if (theIndex != -1)
	  {
	    OTC_String theKey;
	    OTC_String theValue;

	    theKey = theInput.before(theIndex);
	    theKey.rtrim();

	    theValue = theInput.after(theIndex);
	    theValue.ltrim();

	    merge(theKeyPrefix,theKey,theValue);
	  }
	  else
	  {
	    theLogger << "colon missing on line ";
	    theLogger << theLineCount;
	    theLogger << ", ignoring line";
	    theLogger << flush;
	  }
	}

	theInput.truncate();
	isComment = false;
      }
    }
  }
}

/* ------------------------------------------------------------------------- */
void OTC_Properties::merge(
 char const* theKeyPrefix,
 char const* theKey,
 char const* theValue
)
{
  OTC_String tmpKey;
  
  if (theKeyPrefix != 0)
  {
    tmpKey += theKeyPrefix;
    tmpKey += '.';
  }

  tmpKey += theKey;

  OTC_String tmpValue(theValue);

  if (tmpKey.index('*') == -1)
    standard_.add(tmpKey,tmpValue,OTCLIB_REPLACE);
  else
    wildcard_.add(tmpKey,tmpValue,OTCLIB_REPLACE);
}

/* ------------------------------------------------------------------------- */
OTC_String const& OTC_Properties::lookup(
 char const* theKeyPrefix,
 char const* theKey
) const
{
  OTC_String tmpKey;

  if (theKeyPrefix != 0)
  {
    tmpKey += theKeyPrefix;
    tmpKey += '.';
  }

  tmpKey += theKey;

  if (standard_.contains(tmpKey))
    return standard_.item(tmpKey);

  OTC_PairIterator<OTC_String,OTC_String> thePairs;
  thePairs = wildcard_.pairs(OTCLIB_BACKWARD);
  while (thePairs.isValid())
  {
    if (matchPattern_(thePairs.key().string(),tmpKey.string()))
      return thePairs.item();

    thePairs.next();
  }

  return OTC_String::undefinedString();
}

/* ------------------------------------------------------------------------- */
void OTC_Properties::save(ostream& theStream) const
{
  OTC_PairIterator<OTC_String,OTC_String> thePairs;

  thePairs = standard_.pairs();
  while (thePairs.isValid())
  {
    output_(theStream,thePairs.key());

    theStream << " : ";

    output_(theStream,thePairs.item());

    theStream << endl;

    thePairs.next();
  }

  thePairs = wildcard_.pairs();
  while (thePairs.isValid())
  {
    output_(theStream,thePairs.key());

    theStream << " : ";

    output_(theStream,thePairs.item());

    theStream << endl;

    thePairs.next();
  }
}

/* ------------------------------------------------------------------------- */
OTC_String OTC_Properties::encode() const
{
  OTC_String theResult;

  OTC_OSStream theStream(theResult,OTCLIB_BUFFERED);

  save(theStream);

  theStream << flush;

  return theResult;
}

/* ------------------------------------------------------------------------- */
void OTC_Properties::output_(
 ostream& theStream,
 OTC_String const& theString
) const
{
  char const* tmpString;
  tmpString = theString.string();

  for (u_int i=0; i<theString.length(); i++)
  {
    if (tmpString[i] == EOL)
      theStream << '\\';

    theStream << tmpString[i];
  }
}

/* ------------------------------------------------------------------------- */
bool OTC_Properties::matchPattern_(
 const char* theSubjectPattern,
 const char* theSubjectName
) const
{
  char c;
  int s = 1;

  char const* theSubjectString;
  theSubjectString = theSubjectName;

  while (1)
  {
    switch (c = *theSubjectPattern++)
    {
      case EOS:
      {
	// Must be end of string.

	return *theSubjectString == EOS;

	break;
      }

      case '*':
      {
	// Must be at the start of the element.

	if (s == 0)
	  return false;

	// Next pattern character must be '.' or EOS.

	if (*theSubjectPattern != EOS && *theSubjectPattern != '.')
	  return false;

	// Skip characters until end of the element.

	while (*theSubjectString != EOS && *theSubjectString != '.')
	  theSubjectString++;

	// If either at end both must be at end.

	if (*theSubjectPattern == EOS || *theSubjectString == EOS)
	  return *theSubjectPattern == *theSubjectString;

	// Should both be '.', move to next.

	theSubjectString++;
	theSubjectPattern++;

	s = 1;

	break;
      }

      case '.':
      {
	// Must match character in string.

	if (c != *theSubjectString)
	  return false;

	theSubjectString++;

	s = 1;

	break;
      }

      default:
      {
	s = 0;

	if (c != *theSubjectString)
	  return false;

	theSubjectString++;

	break;
      }
    }
  }

  return false;
}

/* ------------------------------------------------------------------------- */
