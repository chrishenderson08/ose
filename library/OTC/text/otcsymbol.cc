/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     text/otcsymbol.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//     Copyright 1994 TELSTRA CORPORATION LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/text/symbol.hh"
#endif
#endif

#include <OTC/text/symbol.hh>
#include <OTC/thread/mxreaper.hh>

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTC_Symbol::mutex_;
OTC_Symbol* OTC_Symbol::globNullSymbol_ = 0;

/* ------------------------------------------------------------------------- */
OTC_Symbol::~OTC_Symbol()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_Symbol::OTC_Symbol(char theChar, size_t theNum)
  : data_(nullSymbol().data_)
{
  OTC_String tmpString(theChar,theNum);
  data_ = OTC_RString(tmpString.string(),tmpString.length());
}

/* ------------------------------------------------------------------------- */
OTC_Symbol::OTC_Symbol(OTC_String const& theString)
  : data_(nullSymbol().data_)
{
  if (theString.rawString().isSymbol())
    data_ = theString.rawString();
  else
    data_ = OTC_RString(theString.string(),theString.length());
}

/* ------------------------------------------------------------------------- */
OTC_Symbol::OTC_Symbol(OTC_String const& theString, size_t theLength)
  : data_(nullSymbol().data_)
{
  OTCLIB_ENSURE_FN((theLength <= theString.length()),
   "OTC_Symbol(OTC_String const&, size_t)",
   "invalid range");

  if (theString.rawString().isSymbol() && theString.length() == theLength)
    data_ = theString.rawString();
  else
    data_ = OTC_RString(theString.string(),theLength);
}

/* ------------------------------------------------------------------------- */
OTC_Symbol::OTC_Symbol(OTC_TString const& theString)
  : data_(nullSymbol().data_)
{
  data_ = OTC_RString(theString.data_.string(),theString.data_.length());
}

/* ------------------------------------------------------------------------- */
char OTC_Symbol::operator[](size_t theIndex) const
{
  OTCLIB_ENSURE_FN((theIndex < data_.length()),
   "OTC_Symbol::operator[](size_t)",
   "index out of range");

  return data_.string()[theIndex];
}

/* ------------------------------------------------------------------------- */
bool OTC_Symbol::exists(char const* theString, size_t theLength)
{
  OTCLIB_ENSURE_FN((theString != 0 || theLength == 0),
   "OTC_Symbol::exists(char const*, size_t)",
   "null pointer but non zero count");

  if (theLength == 0)
    theString = "";

  return OTC_RString::symbolExists(theString,theLength);
}

/* ------------------------------------------------------------------------- */
OTC_Symbol const& OTC_Symbol::nullSymbol()
{
  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (globNullSymbol_ == 0)
  {
    globNullSymbol_ = new OTC_Symbol;
    OTCLIB_ASSERT_M(globNullSymbol_ != 0);
  }

  xxxMutex.release();

  mutex_.unlock();

  return *globNullSymbol_;
}

/* ------------------------------------------------------------------------- */
OTC_RString OTC_Symbol::rawString() const
{
  return data_;
}

/* ------------------------------------------------------------------------- */
OSE_EXPORT ostream& operator<<(ostream& outs, OTC_Symbol const& theSymbol)
{
  outs << theSymbol.rawString();
  return outs;
}

/* ------------------------------------------------------------------------- */
