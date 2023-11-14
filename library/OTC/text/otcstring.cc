/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     text/otcstring.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1993-1994 TELSTRA CORPORATION LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/text/string.hh"
#pragma implementation "OTC/text/tstring.hh"
#pragma implementation "OTC/text/sobject.hh"
#endif
#endif

#include <OTC/text/string.hh>
#include <OTC/thread/mxreaper.hh>

#include <ctype.h>

/* ------------------------------------------------------------------------- */
OTC_SObject::OTC_SObject()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_SObject::~OTC_SObject()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTC_String::mutex_;
bool OTC_String::globInitialised_ = false;
OTC_String const* OTC_String::globNullString_ = 0;
OTC_String const* OTC_String::globUndefinedString_ = 0;

/* ------------------------------------------------------------------------- */
OTC_String::~OTC_String()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_String::OTC_String(char const* theString, size_t theNum)
  : data_(theNum)
{
  OTCLIB_ENSURE_FN((theString != 0 || theNum == 0),
   "OTC_String(char const*, size_t)",
   "invalid character count");

  if (theString != 0)
    OTC_Copy::copy(data_.string(),theString,data_.length());
}

/* ------------------------------------------------------------------------- */
OTC_String::OTC_String(OTC_String const& theString, size_t theNum)
  : data_(theNum)
{
  OTCLIB_ENSURE_FN((theNum <= theString.length()),
   "OTC_String(OTC_String const&, size_t)",
   "invalid character count");

  OTC_Copy::copy(data_.string(),theString.string(),data_.length());
}

/* ------------------------------------------------------------------------- */
OSE_EXPORT istream& operator>>(istream& ins, OTC_String& theString)
{
  if (!ins.good())
    return ins;

  // Go directly to raw string to make things quick.

  OTC_RString& theData = theString.rawString();

  // Set initial string size. Characters will be read directly into the
  // string, growing it further if necessary, and the length readjusted
  // when finished. Use initial size of <BLKSIZE-1> instead of <BLKSIZE> as
  // raw string adds <1> to this to accomodate a null terminator. <BLKSIZE>
  // must be greater than <1>.

  int const BLKSIZE = 16;

  size_t theCapacity = BLKSIZE-1;
  theData.reallocate(theCapacity);

  // Special streams prefix operations. Set string size to zero thus
  // creating an empty string if this fails. Note that <ipfx()> skips
  // leading white space if stream is configured to skip white space.

#if 0
  if (!ins.ipfx(0))
  {
    theData.reallocate(0);
    return ins;
  }
#else
  if ((ins.flags() & ios::skipws) != 0)
    ins >> ws;
#endif

  // Set the length. This will be incremented as each character is read
  // and will be used to readjust the string length at the end. This
  // will also be used to determine if we have exceeded the capacity of
  // the string and thus it needs to be resized, and whether we have
  // exceeded the width request programmed into the stream.

  size_t theLength = 0;

  // If non zero, the maximum number of characters to read in.

  size_t theWidth = ins.width();

  // Read the characters in.

  int c;

  // c = ins.get();
  c = ins.peek();

  if (c == EOF)
    ins.get();

  if (c != EOF && !isspace(c))
  {
    theData[theLength++] = c;

    // c = ins.get();
    ins.ignore(1);
    c = ins.peek();

    while (c != EOF && !isspace(c) && (theWidth == 0 || theLength < theWidth))
    {
      if (theLength == theCapacity)
      {
	theCapacity += BLKSIZE;
        theData.reallocate(theCapacity);
      }
      theData[theLength++] = char(c);

      // c = ins.get();
      ins.ignore(1);
      c = ins.peek();
    }
  }

  // Set the length of the string to what it should be.

  theData.reallocate(theLength);

  // Put back the last character into the stream.

  // if (c != EOF)
  //   ins.putback(char(c));

  // Reset the stream width setting.

  ins.width(0);

  //  Special streams suffix operations.

#if 0
  ins.isfx();
#endif

  return ins;
}

/* ------------------------------------------------------------------------- */
OSE_EXPORT ostream& operator<<(ostream& outs, OTC_String const& theString)
{
  return outs << theString.rawString();
}

/* ------------------------------------------------------------------------- */
OTC_String OTC_String::get(istream& ins, char theDelim)
{
  OTC_String s;
  return get(s,ins,theDelim);
}

/* ------------------------------------------------------------------------- */
OTC_String& OTC_String::get(
 OTC_String& theString,
 istream& ins,
 char theDelim
)
{
  if (!ins.good())
    return theString;

  // Do a quick check to see if next character is the delimiter. We
  // do this because ANSI C++ standard changed the behaviour of streams
  // such that get() when next charcater is delimiter will cause the
  // fail bit to be set. We want to avoid having to clear the fail bit,
  // also avoids unecessarily growing string buffer.

  if (ins.peek() == theDelim)
    return theString;

  // Characters will be read directly into the string, growing it further if
  // necessary, and the length readjusted when finished.

  // <BLKSIZE> is the amount we increase the capacity of the string.
  // The size of this is really only relevant when string buffering is
  // turned off in which case this overrides it to a degree.

  int const BLKSIZE = 32;

  OTC_RString& theData = theString.data_;

  // The accumulated length of the string, the capacity, and amount of
  // available space within that capacity.

  size_t theLength = theString.data_.length();
  size_t theCapacity = theString.data_.capacity();
  size_t theSpace = theCapacity - theLength;

  // If current capacity of string is filled up, or string is shared,
  // increase the capacity of the string.

  if (theSpace == 0 || theString.data_.isShared())
  {
    theData.reallocate(theLength+BLKSIZE);
    theCapacity = theString.data_.capacity();
    theSpace = theCapacity - theLength;
  }

  // Ensure length of string is same as capacity so that when further
  // reallocations are done all data read so far is copied.

  theData.reallocate(theCapacity);

  // Flag for when we have finished.

  bool theEnd = false;

  // Read in the characters. Note that <istream::get()> will write a
  // null character over the raw string null terminator, but this is okay.

  while (theEnd == false)
  {
    char* tmpString = theData.string() + theLength;
    ins.get(tmpString,theSpace+1,theDelim);
    theLength += ins.gcount();

    if ((long)ins.gcount() != (long)theSpace)
    {
      theEnd = true;
    }
    else
    {
      // Check if the next character is the delimiter and bail out now.

      if (ins.peek() != theDelim)
      {
	theData.reallocate(theLength+BLKSIZE);
	theCapacity = theString.data_.capacity();
	theSpace = theCapacity - theLength;

	theData.reallocate(theCapacity);
      }
      else
	theEnd = true;
    }
  }

  // Readjust the length of the string.

  theData.reallocate(theLength);

  return theString;
}

/* ------------------------------------------------------------------------- */
OTC_String OTC_String::getline(istream& ins, char theDelim)
{
  OTC_String s;
  return getline(s,ins,theDelim);
}

/* ------------------------------------------------------------------------- */
OTC_String& OTC_String::getline(
 OTC_String& theString,
 istream& ins,
 char theDelim
)
{
  if (!ins.good())
    return theString;

  // Do a quick check to see if next character is the delimiter. We
  // do this because ANSI C++ standard changed the behaviour of streams
  // such that get() when next charcater is delimiter will cause the
  // fail bit to be set. We want to avoid having to clear the fail bit,
  // also avoids unecessarily growing string buffer.

  if (ins.peek() == theDelim)
  {
    ins.ignore(1);
    return theString;
  }

  // Characters will be read directly into the string, growing it further if
  // necessary, and the length readjusted when finished.

  // <BLKSIZE> is the amount we increase the capacity of the string.
  // The size of this is really only relevant when string buffering is
  // turned off in which case this overrides it to a degree.

  int const BLKSIZE = 32;

  OTC_RString& theData = theString.data_;

  // The accumulated length of the string, the capacity, and amount of
  // available space within that capacity.

  size_t theLength = theString.data_.length();
  size_t theCapacity = theString.data_.capacity();
  size_t theSpace = theCapacity - theLength;

  // If current capacity of string is filled up, or string is shared,
  // increase the capacity of the string.

  if (theSpace == 0 || theString.data_.isShared())
  {
    theData.reallocate(theLength+BLKSIZE);
    theCapacity = theString.data_.capacity();
    theSpace = theCapacity - theLength;
  }

  // Ensure length of string is same as capacity so that when further
  // reallocations are done all data read so far is copied.

  theData.reallocate(theCapacity);

  // Flag for when we have finished.

  bool theEnd = false;

  // Read in the characters. Note that <istream::get()> will write a
  // null character over the raw string null terminator, but this is okay.
  // Also, we use <istream::get()> instead of <istream::getline()> so
  // we we do not have to adjust <istream::gcount()> when we get to
  // the <EOF> and do not find the delimiter.

  while (theEnd == false)
  {
    char* tmpString = theData.string() + theLength;
    ins.get(tmpString,theSpace+1,theDelim);
    theLength += ins.gcount();

    if ((long)ins.gcount() != (long)theSpace)
    {
      theEnd = true;
    }
    else
    {
      // Check if the next character is the delimiter and bail out now.

      if (ins.peek() != theDelim)
      {
	theData.reallocate(theLength+BLKSIZE);
	theCapacity = theString.data_.capacity();
	theSpace = theCapacity - theLength;

	theData.reallocate(theCapacity);
      }
      else
	theEnd = true;
    }
  }

  // Readjust the length of the string.

  theData.reallocate(theLength);

  // Discard the delimiter.

  if (!ins.eof())
    ins.ignore(1);

  return theString;
}

/* ------------------------------------------------------------------------- */
OTC_String OTC_String::read(istream& ins, size_t theNum)
{
  OTC_String s;
  return read(s,ins,theNum);
}

/* ------------------------------------------------------------------------- */
OTC_String& OTC_String::read(OTC_String& theString, istream& ins, size_t theNum)
{
  if (!ins.good())
    return theString;

  if (theNum > 0)
  {
    OTC_RString& theData = theString.data_;

    size_t oldLength = theData.length();

    theData.reallocate(oldLength+theNum);

    ins.read(theData.string()+oldLength,theNum);

    theData.reallocate(oldLength+ins.gcount());
  }

  return theString;
}

/* ------------------------------------------------------------------------- */
OTC_String& OTC_String::readFile(istream& theStream)
{
  length(0);

  // Bailout if stream no longer good.

  if (!theStream.good())
    return *this;

  // Characters will be read directly into the string, growing it further if
  // necessary, and the length readjusted when finished.

  // <BLKSIZE> is the amount we increase the capacity of the string.
  // The size of this is really only relevant when string buffering is
  // turned off in which case this overrides it to a degree.

  int const BLKSIZE = 32;

  // The accumulated length of the string, the capacity, and amount of
  // available space within that capacity.

  size_t theLength = data_.length();
  size_t theCapacity = data_.capacity();
  size_t theSpace = theCapacity - theLength;

  // If current capacity of string is filled up, or string is shared,
  // increase the capacity of the string.

  if (theSpace == 0 || data_.isShared())
  {
    data_.reallocate(theLength+BLKSIZE);
    theCapacity = data_.capacity();
    theSpace = theCapacity - theLength;
  }

  // Ensure length of string is same as capacity so that when further
  // reallocations are done all data read so far is copied.

  data_.reallocate(theCapacity);

  // Flag for when we have finished.

  bool theEnd = false;

  // Read in the characters.

  while (theEnd == false)
  {
    char* tmpString = data_.string() + theLength;
    theStream.read(tmpString,theSpace);
    theLength += theStream.gcount();

    if ((long)theStream.gcount() != (long)theSpace)
    {
      if (theStream.eof() && theStream.fail())
      {
        theStream.clear();
        theStream.clear(ios::eofbit | theStream.rdstate());
      }

      theEnd = true;
    }
    else
    {
      data_.reallocate(theLength+BLKSIZE);
      theCapacity = data_.capacity();
      theSpace = theCapacity - theLength;

      data_.reallocate(theCapacity);
    }
  }

  // Readjust the length of the string.

  data_.reallocate(theLength);

  return *this;
}

/* ------------------------------------------------------------------------- */
OTC_String& OTC_String::readLine(istream& theStream, bool theSkipWhite)
{
  if (!theStream.good())
    return *this;

  if (theSkipWhite != false)
    theStream >> ws;

  if (theStream.eof())
    theStream.clear(ios::failbit | theStream.rdstate());

  length(0);
  OTC_String::getline(*this,theStream,EOL);

  return *this;
}

/* ------------------------------------------------------------------------- */
OTC_String& OTC_String::readString(istream& theStream)
{
  length(0);
  OTC_String::getline(*this,theStream,EOS);

  return *this;
}

/* ------------------------------------------------------------------------- */
OTC_String& OTC_String::readToDelim(istream& theStream, char theDelim)
{
  length(0);
  OTC_String::getline(*this,theStream,theDelim);

  return *this;
}

/* ------------------------------------------------------------------------- */
OTC_String& OTC_String::readToken(istream& theStream)
{
  // Reset string length as we want to replace anything in the string.

  data_.reallocate(0);

  // Return straight away if stream not in a good state.

  if (!theStream.good())
    return *this;

  // Skip leading whitespace.

  theStream >> ws;

  // Go directly to raw string to make things quick.

  OTC_RString& theData = data_;

  // Set initial string size. Characters will be read directly into the
  // string, growing it further if necessary, and the length readjusted
  // when finished. Use initial size of <BLKSIZE-1> instead of <BLKSIZE> as
  // raw string adds <1> to this to accomodate a null terminator. <BLKSIZE>
  // must be greater than <1>.

  int const BLKSIZE = 16;

  theData.reallocate(BLKSIZE-1);
  size_t theCapacity = theData.capacity();

  // Skip leading whitespace.

  theStream >> ws;

  // Set the length. This will be incremented as each character is read
  // and will be used to readjust the string length at the end. This
  // will also be used to determine if we have exceeded the capacity of
  // the string and thus it needs to be resized.

  size_t theLength = 0;

  // Read the characters in.

  int c;

  // c = theStream.get();
  c = theStream.peek();

  if (c == EOF)
    theStream.get();

  if (c != EOF)
  {
    theData[theLength++] = char(c);

    // c = theStream.get();
    theStream.ignore(1);
    c = theStream.peek();

    while (c != EOF && !isspace(c))
    {
      if (theLength == theCapacity)
      {
	theCapacity += BLKSIZE;
        theData.reallocate(theCapacity);
      }
      theData[theLength++] = char(c);

      // c = theStream.get();
      theStream.ignore(1);
      c = theStream.peek();
    }
  }

  // Set the length of the string to what it should be.

  theData.reallocate(theLength);

  // Put back the last character into the stream.

  // if (c != EOF)
  //   theStream.putback(char(c));

  return *this;
}

/* ------------------------------------------------------------------------- */
OTC_String& OTC_String::readData(istream& theStream, size_t theNum)
{
  length(0);
  OTC_String::read(*this,theStream,theNum);

  return *this;
}

/* ------------------------------------------------------------------------- */
char* OTC_String::buffer()
{
  if (data_.length() == 0)
    return 0;

  if (data_.isShared())
    data_.sync();

  return data_.string();
}

/* ------------------------------------------------------------------------- */
char OTC_String::operator[](size_t theIndex) const
{
  OTCLIB_ENSURE_FN((theIndex < data_.length()),
   "OTC_String::operator[](size_t)",
   "index out of range");

  return data_[theIndex];
}

/* ------------------------------------------------------------------------- */
char& OTC_String::operator[](size_t theIndex)
{
  OTCLIB_ENSURE_FN((theIndex < data_.length()),
   "OTC_String::operator[](size_t)",
   "index out of range");

  if (data_.isShared())
    data_.sync();

  return data_[theIndex];
}

/* ------------------------------------------------------------------------- */
void OTC_String::capacity(size_t theCapacity)
{
  size_t theLength = data_.length();

  if (data_.isShared())
  {
    if (theCapacity > theLength)
    {
      data_.reallocate(theCapacity,OTCLIB_UNBUFFERED);
      data_.reallocate(theLength);
    }
    else
    {
      data_.reallocate(theLength,OTCLIB_UNBUFFERED);
    }
  }
  else
  {
    if (theCapacity > data_.capacity())
    {
      data_.reallocate(theCapacity,OTCLIB_UNBUFFERED);
      data_.reallocate(theLength);
    }
  }
}

/* ------------------------------------------------------------------------- */
char* OTC_String::duplicate() const
{
  char* tmpString;
  tmpString = new char[data_.length()+1];
  OTCLIB_ASSERT_M(tmpString != 0);

  OTC_Copy::copy(tmpString,data_.string(),data_.length()+1);
  return tmpString;
}

/* ------------------------------------------------------------------------- */
OTC_String& OTC_String::assign(OTC_String const& theString, size_t theNum)
{
  OTCLIB_ENSURE_FN((theNum <= theString.data_.length()),
   "OTC_String::assign(OTC_String const&, size_t)",
   "invalid length");

  replace_(0,data_.length(),theString.string(),theNum);

  return *this;
}

/* ------------------------------------------------------------------------- */
OSE_EXPORT OTC_TString operator+(OTC_String const& s1, OTC_String const& s2)
{
  OTC_RString tmpString(s1.rawString().length()+s2.rawString().length());
  char* tmpBuffer = tmpString.string();
  OTC_Copy::copy(tmpBuffer,s1.string(),s1.rawString().length());
  OTC_Copy::copy(tmpBuffer+s1.rawString().length(),s2.string(),
   s2.rawString().length());
  return OTC_TString(tmpString);
}

/* ------------------------------------------------------------------------- */
OSE_EXPORT OTC_TString operator+(OTC_String const& s1, char const* s2)
{
  size_t theLength = (s2 ? ::strlen(s2) : 0);
  OTC_RString tmpString(s1.rawString().length()+theLength);
  char* tmpBuffer = tmpString.string();
  OTC_Copy::copy(tmpBuffer,s1.string(),s1.rawString().length());
  OTC_Copy::copy(tmpBuffer+s1.rawString().length(),s2,theLength);
  return OTC_TString(tmpString);
}

/* ------------------------------------------------------------------------- */
OSE_EXPORT OTC_TString operator+(char const* s1, OTC_String const& s2)
{
  size_t theLength = (s1 ? ::strlen(s1) : 0);
  OTC_RString tmpString(theLength+s2.rawString().length());
  char* tmpBuffer = tmpString.string();
  OTC_Copy::copy(tmpBuffer,s1,theLength);
  OTC_Copy::copy(tmpBuffer+theLength,s2.string(),s2.rawString().length());
  return OTC_TString(tmpString);
}

/* ------------------------------------------------------------------------- */
OSE_EXPORT OTC_TString operator+(OTC_String const& s1, OTC_SObject const& s2)
{
  OTC_String s2String = s2;
  OTC_RString tmpString(s1.rawString().length()+s2String.rawString().length());
  char* tmpBuffer = tmpString.string();
  OTC_Copy::copy(tmpBuffer,s1.string(),s1.rawString().length());
  OTC_Copy::copy(tmpBuffer+s1.rawString().length(),s2String.string(),
   s2String.rawString().length());
  return OTC_TString(tmpString);
}

/* ------------------------------------------------------------------------- */
OSE_EXPORT OTC_TString operator+(OTC_SObject const& s1, OTC_String const& s2)
{
  OTC_String s1String = s1;
  OTC_RString tmpString(s1String.rawString().length()+s2.rawString().length());
  char* tmpBuffer = tmpString.string();
  OTC_Copy::copy(tmpBuffer,s1String.string(),s1String.rawString().length());
  OTC_Copy::copy(tmpBuffer+s1String.rawString().length(),
   s2.string(),s2.rawString().length());
  return OTC_TString(tmpString);
}

/* ------------------------------------------------------------------------- */
OSE_EXPORT OTC_TString operator+(OTC_SObject const& s1, char const* s2)
{
  size_t theLength = (s2 ? ::strlen(s2) : 0);
  OTC_String s1String = s1;
  OTC_RString tmpString(s1String.rawString().length()+theLength);
  char* tmpBuffer = tmpString.string();
  OTC_Copy::copy(tmpBuffer,s1String.string(),s1String.rawString().length());
  OTC_Copy::copy(tmpBuffer+s1String.rawString().length(),s2,theLength);
  return OTC_TString(tmpString);
}

/* ------------------------------------------------------------------------- */
OSE_EXPORT OTC_TString operator+(OTC_String const& s1, char c1)
{
  OTC_RString tmpString(s1.rawString().length()+1);
  char* tmpBuffer = tmpString.string();
  OTC_Copy::copy(tmpBuffer,s1.string(),s1.rawString().length());
  tmpBuffer[s1.rawString().length()] = c1;
  return OTC_TString(tmpString);
}

/* ------------------------------------------------------------------------- */
OSE_EXPORT OTC_TString operator+(char c1, OTC_String const& s1)
{
  OTC_RString tmpString(1+s1.rawString().length());
  char* tmpBuffer = tmpString.string();
  tmpBuffer[0] = c1;
  OTC_Copy::copy(tmpBuffer+1,s1.string(),s1.rawString().length());
  return OTC_TString(tmpString);
}

/* ------------------------------------------------------------------------- */
OTC_String& OTC_String::truncate(size_t theIndex)
{
  if (data_.length() == 0 && theIndex == 0)
    return *this;

  long theNum = data_.length() - theIndex;

  if (theNum == 0)
    return *this;

  OTCLIB_ENSURE_FN((theNum > 0),
   "OTC_String::truncate(size_t)",
   "index out of range");

  replace_(theIndex,theNum,EOS,0);

  return *this;
}

/* ------------------------------------------------------------------------- */
OTC_String& OTC_String::rtrim()
{
  if (data_.length() == 0)
    return *this;

  size_t theIndex = data_.length();

  while (theIndex > 0 && isspace(data_[theIndex-1]))
    theIndex--;

  if (theIndex != data_.length())
    truncate(theIndex);

  return *this;
}

/* ------------------------------------------------------------------------- */
OTC_String& OTC_String::rtrim(char theChar)
{
  if (data_.length() == 0)
    return *this;

  size_t theIndex = data_.length();

  while (theIndex > 0 && data_[theIndex-1] == theChar)
    theIndex--;

  if (theIndex != data_.length())
    truncate(theIndex);

  return *this;
}

/* ------------------------------------------------------------------------- */
OTC_String& OTC_String::rtrim(OTC_CBitSet const& theCharSet)
{
  if (data_.length() == 0)
    return *this;

  size_t theIndex = data_.length();

  while (theIndex > 0 && theCharSet.test(data_[theIndex-1]))
    theIndex--;

  if (theIndex != data_.length())
    truncate(theIndex);

  return *this;
}

/* ------------------------------------------------------------------------- */
OTC_String& OTC_String::ltrim()
{
  if (data_.length() == 0)
    return *this;

  size_t theIndex = 0;
  size_t theLength = data_.length();

  while (theIndex < theLength && isspace(data_[theIndex]))
    theIndex++;

  if (theIndex != 0)
    remove(0,theIndex);

  return *this;
}

/* ------------------------------------------------------------------------- */
OTC_String& OTC_String::ltrim(char theChar)
{
  if (data_.length() == 0)
    return *this;

  size_t theIndex = 0;
  size_t theLength = data_.length();

  while (theIndex < theLength && data_[theIndex] == theChar)
    theIndex++;

  if (theIndex != 0)
    remove(0,theIndex);

  return *this;
}

/* ------------------------------------------------------------------------- */
OTC_String& OTC_String::ltrim(OTC_CBitSet const& theCharSet)
{
  if (data_.length() == 0)
    return *this;

  size_t theIndex = 0;
  size_t theLength = data_.length();

  while (theIndex < theLength && theCharSet.test(data_[theIndex]))
    theIndex++;

  if (theIndex != 0)
    remove(0,theIndex);

  return *this;
}

/* ------------------------------------------------------------------------- */
OTC_String& OTC_String::ljustify(size_t theWidth, char theChar)
{
  if (data_.length() >= theWidth)
    return *this;

  replace_(data_.length(),0,theChar,theWidth-data_.length());

  return *this;
}

/* ------------------------------------------------------------------------- */
OTC_String& OTC_String::rjustify(size_t theWidth, char theChar)
{
  if (data_.length() >= theWidth)
    return *this;

  replace_(0,0,theChar,theWidth-data_.length());

  return *this;
}

/* ------------------------------------------------------------------------- */
OTC_String& OTC_String::reverse()
{
  if (data_.length() <= 1)
    return *this;

  size_t theLength = data_.length();

  if (data_.isShared())
  {
    OTC_RString tmpData(theLength);
    for (size_t i=theLength; i>0; i--)
      tmpData[theLength-i] = data_[i-1];

    data_ = tmpData;
  }
  else
  {
    for (size_t i=(theLength/2); i>0; i--)
    {
      char c = data_[theLength-i];
      data_[theLength-i] = data_[i-1];
      data_[i-1] = c;
    }
  }

  return *this;
}

/* ------------------------------------------------------------------------- */
int OTC_String::index_(size_t theIndex, char theChar, size_t theNum) const
{
  OTCLIB_ENSURE_FN((theNum > 0),
   "OTC_String::index_(size_t, char, size_t)",
   "bad occurrence number");

  if (data_.length() == 0)
    return -1;

  OTCLIB_ENSURE_FN((theIndex < data_.length()),
   "OTC_String::index_(size_t, char, size_t)",
   "index out of range");

  char const* tmpString = data_.string()+theIndex;
  tmpString = (char const*)memchr(tmpString,theChar,data_.length()-theIndex);

  if (tmpString != 0)
  {
    long anIndex = tmpString - data_.string();
    if (theNum == 1)
    {
      return anIndex;
    }
    else
    {
      if (anIndex+1 == (long)data_.length())
        return -1;

      else
        return index_(anIndex+1,theChar,theNum-1);
    }
  }
  else
    return -1;
}

/* ------------------------------------------------------------------------- */
int OTC_String::rindex_(char theChar, size_t theNum) const
{
  OTCLIB_ENSURE_FN((theNum > 0),
   "OTC_String::rindex_(char, size_t)",
   "bad occurrence number");

  if (data_.length() == 0)
    return -1;

  long theIndex = data_.length() - 1;
  while (theIndex >= 0)
  {
    if (data_[theIndex] == theChar)
    {
      theNum--;
      if (theNum == 0)
        return theIndex;
    }
    theIndex--;
  }

  return -1;
}

/* ------------------------------------------------------------------------- */
int OTC_String::index_(
 size_t theIndex,
 OTC_CBitSet const& theCharSet,
 size_t theNum
) const
{
  OTCLIB_ENSURE_FN((theNum > 0),
   "OTC_String::index_(size_t, OTC_CBitSet const&, size_t)",
   "bad occurrence number");

  if (data_.length() == 0)
    return -1;

  OTCLIB_ENSURE_FN((theIndex < data_.length()),
   "OTC_String::index_(size_t, OTC_CBitSet const&, size_t)",
   "index out of range");

  char const* tmpString = data_.string() + theIndex;
  size_t theLength = data_.length() - theIndex;

  while (theLength != 0)
  {
    if (theCharSet.test(*tmpString) != 0)
      break;

    tmpString++;
    theLength--;
  }

  if (theLength != 0)
  {
    long anIndex = tmpString - data_.string();
    if (theNum == 1)
    {
      return anIndex;
    }
    else
    {
      if (anIndex+1 == (long)data_.length())
        return -1;

      else
        return index_(anIndex+1,theCharSet,theNum-1);
    }
  }
  else
    return -1;
}

/* ------------------------------------------------------------------------- */
int OTC_String::rindex_(OTC_CBitSet const& theCharSet, size_t theNum) const
{
  OTCLIB_ENSURE_FN((theNum > 0),
   "OTC_String::rindex_(OTC_CBitSet const&, size_t)",
   "bad occurrence number");

  if (data_.length() == 0)
    return -1;

  long theIndex = data_.length() - 1;
  while (theIndex >= 0)
  {
    if (theCharSet.test(data_[theIndex]) != 0)
    {
      theNum--;
      if (theNum == 0)
        return theIndex;
    }
    theIndex--;
  }

  return -1;
}

/* ------------------------------------------------------------------------- */
int OTC_String::rank(
 OTC_String const& theString,
 size_t theLength,
 OTC_SRankType theType
) const
{
  OTCLIB_ENSURE_FN((theLength <= theString.data_.length()),
   "OTC_String::rank(OTC_String const&, size_t, OTC_SRankType)",
   "invalid length");

  return OTC_Rank::rank(data_.string(),data_.length(),
   theString.data_.string(),theLength,theType);
}

/* ------------------------------------------------------------------------- */
bool OTC_String::compare(
 size_t theIndex,
 char const* theString,
 OTC_SCmpType theType
) const
{
  OTCLIB_ENSURE_FN((theIndex <= data_.length()),
   "OTC_String::compare(size_t, char const*, OTC_SCmpType)",
   "index out of range");

  OTC_SRankType theRankType = OTCLIB_ASCIIORDER;
  if (theType == OTCLIB_IGNORECASE)
    theRankType = OTCLIB_ASCIIORDER_IGNORECASE;

  return OTC_Rank::rank(data_.string()+theIndex,data_.length()-theIndex,
   theString,(theString ? ::strlen(theString) : 0),theRankType) == 0;
}

/* ------------------------------------------------------------------------- */
bool OTC_String::compare(
 size_t theIndex,
 char const* theString,
 size_t theNum,
 OTC_SCmpType theType
) const
{
  OTCLIB_ENSURE_FN((theIndex <= data_.length()),
   "OTC_String::compare(size_t, char const*, size_t, OTC_SCmpType)",
   "index out of range");

  if (data_.length()-theIndex < theNum)
    return false;

  OTC_SRankType theRankType = OTCLIB_ASCIIORDER;
  if (theType == OTCLIB_IGNORECASE)
    theRankType = OTCLIB_ASCIIORDER_IGNORECASE;

  return OTC_Rank::rank(data_.string()+theIndex,theNum,
   theString,theNum,theRankType) == 0;
}

/* ------------------------------------------------------------------------- */
bool OTC_String::compare(
 size_t theIndex,
 OTC_String const& theString,
 OTC_SCmpType theType
) const
{
  OTCLIB_ENSURE_FN((theIndex <= data_.length()),
   "OTC_String::compare(size_t, OTC_String const&, OTC_SCmpType)",
   "index out of range");

  OTC_SRankType theRankType = OTCLIB_ASCIIORDER;
  if (theType == OTCLIB_IGNORECASE)
    theRankType = OTCLIB_ASCIIORDER_IGNORECASE;

  return OTC_Rank::rank(data_.string()+theIndex,data_.length()-theIndex,
   theString.string(),theString.length(),theRankType) == 0;
}

/* ------------------------------------------------------------------------- */
bool OTC_String::compare(
 size_t theIndex,
 OTC_String const& theString,
 size_t theNum,
 OTC_SCmpType theType
) const
{
  OTCLIB_ENSURE_FN((theIndex <= data_.length()),
   "OTC_String::compare(size_t, OTC_String const&, size_t, OTC_SCmpType)",
   "index out of range");

  OTCLIB_ENSURE_FN((theNum <= theString.length()),
   "OTC_String::compare(size_t, OTC_String const&, size_t, OTC_SCmpType)",
   "invalid length");

  if (data_.length()-theIndex < theNum)
    return false;

  OTC_SRankType theRankType = OTCLIB_ASCIIORDER;
  if (theType == OTCLIB_IGNORECASE)
    theRankType = OTCLIB_ASCIIORDER_IGNORECASE;

  return OTC_Rank::rank(data_.string()+theIndex,theNum,
   theString.string(),theNum,theRankType) == 0;
}

/* ------------------------------------------------------------------------- */
OTC_String const& OTC_String::nullString()
{
  if (globInitialised_ == false)
  {
    mutex_.lock();

    OTC_MutexReaper<OTC_NRMutex> xxxMutex;
    xxxMutex.grab(mutex_);

    if (globNullString_ == 0)
    {
      OTC_CString tmpString;
      globNullString_ = new OTC_String(tmpString);
      OTCLIB_ASSERT_M(globNullString_ != 0);
    }

    if (globUndefinedString_ == 0)
    {
      OTC_CString tmpString;
      globUndefinedString_ = new OTC_String(tmpString);
      OTCLIB_ASSERT_M(globUndefinedString_ != 0);
    }

    globInitialised_ = true;

    xxxMutex.release();

    mutex_.unlock();
  }

  return *globNullString_;
}

/* ------------------------------------------------------------------------- */
OTC_String const& OTC_String::undefinedString()
{
  if (globInitialised_ == false)
  {
    mutex_.lock();

    OTC_MutexReaper<OTC_NRMutex> xxxMutex;
    xxxMutex.grab(mutex_);

    if (globNullString_ == 0)
    {
      OTC_CString tmpString;
      globNullString_ = new OTC_String(tmpString);
      OTCLIB_ASSERT_M(globNullString_ != 0);
    }

    if (globUndefinedString_ == 0)
    {
      OTC_CString tmpString;
      globUndefinedString_ = new OTC_String(tmpString);
      OTCLIB_ASSERT_M(globUndefinedString_ != 0);
    }

    globInitialised_ = true;

    xxxMutex.release();

    mutex_.unlock();
  }

  return *globUndefinedString_;
}

/* ------------------------------------------------------------------------- */
void OTC_String::replace_(
 long theStart,
 size_t theLength,
 char theChar,
 size_t theNum
)
{
  OTCLIB_ENSURE_FN((theStart >= 0 && theStart+theLength <= data_.length()),
   "OTC_String::replace_(long, size_t, char, size_t)",
   "invalid range");

  size_t oldLength = data_.length();

  if (data_.isShared())
  {
    // Check for special case whereby full length
    // of string being truncated and not replaced
    // with anything.

    if (theStart == 0 && (theLength == oldLength) && theNum == 0)
    {
      // Link it back to the null string.
      // Subsequent additions to string will
      // resize it as necessary.

      data_ = nullString().data_;
    }
    else
    {
      OTC_RString tmpData(data_.length()+theNum-theLength);
      OTC_Copy::copy(tmpData.string(),data_.string(),theStart);
      memset(tmpData.string()+theStart,theChar,theNum);
      OTC_Copy::copy(tmpData.string()+theStart+theNum,
       data_.string()+theStart+theLength,
       data_.length()-(theStart+theLength));
      data_ = tmpData;
    }
  }
  else
  {
    // Check for special case whereby full length
    // of string being truncated and not replaced
    // with anything.

    if (theStart == 0 && (theLength == oldLength) && theNum == 0)
    {
      // Simply adjust the length of the string
      // back down to zero length.

      data_.reallocate(0);
    }
    else if (theNum <= theLength)
    {
      memset(data_.string()+theStart,theChar,theNum);
      OTC_Copy::copy(data_.string()+theStart+theNum,
       data_.string()+theStart+theLength,oldLength-(theStart+theLength));
      data_.reallocate(data_.length()+theNum-theLength);
    }
    else
    {
      data_.reallocate(data_.length()+theNum-theLength);
      OTC_Copy::copy(data_.string()+theStart+theNum,
       data_.string()+theStart+theLength,oldLength-(theStart+theLength));
      memset(data_.string()+theStart,theChar,theNum);
    }
  }
}

/* ------------------------------------------------------------------------- */
void OTC_String::replace_(
 long theStart,
 size_t theLength,
 char const* theString,
 size_t theNum 
)
{
  OTCLIB_ENSURE_FN((theStart >= 0 && theStart+theLength <= data_.length()),
   "OTC_String::replace_(long, size_t, char const*, size_t)",
   "invalid range");
  OTCLIB_ENSURE_FN((theNum == 0 || theString != 0),
   "OTC_String::replace_(long, size_t, char const*, size_t)",
   "null string but non zero count");

  if (data_.isShared() || (theString >= data_.string() &&
   theString < data_.string()+data_.length()))
  {
    OTC_RString tmpData(data_.length()+theNum-theLength);
    OTC_Copy::copy(tmpData.string(),data_.string(),theStart);
    OTC_Copy::copy(tmpData.string()+theStart,theString,theNum);
    OTC_Copy::copy(tmpData.string()+theStart+theNum,
     data_.string()+theStart+theLength,data_.length()-(theStart+theLength));
    data_ = tmpData;
  }
  else
  {
    size_t oldLength = data_.length();
    if (theNum <= theLength)
    {
      OTC_Copy::copy(data_.string()+theStart,theString,theNum);
      OTC_Copy::copy(data_.string()+theStart+theNum,
       data_.string()+theStart+theLength,oldLength-(theStart+theLength));
      data_.reallocate(data_.length()+theNum-theLength);
    }
    else
    {
      data_.reallocate(data_.length()+theNum-theLength);
      OTC_Copy::copy(data_.string()+theStart+theNum,
       data_.string()+theStart+theLength,oldLength-(theStart+theLength));
      OTC_Copy::copy(data_.string()+theStart,theString,theNum);
    }
  }
}

/* ------------------------------------------------------------------------- */
void OTC_String::replace_(
 long theStart,
 size_t theLength,
 OTC_String const& theString,
 size_t theNum 
)
{
  OTCLIB_ENSURE_FN((theNum <= theString.data_.length()),
   "OTC_String::replace_(long, size_t, OTC_String const&, size_t)",
   "invalid range");

  replace_(theStart,theLength,theString.data_.string(),theNum);
}

/* ------------------------------------------------------------------------- */
OTC_String OTC_String::section_(long theStart, long theLength) const
{
  OTCLIB_ENSURE_FN((theLength >= 0),
   "OTC_String::section_(long, long)",
   "invalid range");
  OTCLIB_ENSURE_FN((theStart >= 0),
   "OTC_String::section_(long, long)",
   "invalid range");
  OTCLIB_ENSURE_FN((theStart+theLength <= int(data_.length())),
   "OTC_String::section_(long, long)",
   "invalid range");

  if (data_.length() == 0 && theLength == 0)
    return nullString();

  OTC_String tmpString(data_.string()+theStart,theLength);

  return tmpString;
}

/* ------------------------------------------------------------------------- */
OTC_String OTC_String::except_(long theStart, size_t theLength) const
{
  OTCLIB_ENSURE_FN((theStart >= 0 && theStart+theLength <= data_.length()),
   "OTC_String::except_(long, size_t)",
   "invalid range");

  if (data_.length() == 0)
    return nullString();

  else if (theLength == 0)
    return *this;

  OTC_RString tmpString(data_.length()-theLength);
  char* tmpBuffer1 = tmpString.string();
  char const* tmpBuffer2 = data_.string();
  OTC_Copy::copy(tmpBuffer1,tmpBuffer2,theStart);
  tmpBuffer1 += theStart;
  tmpBuffer2 += (theStart + theLength);
  OTC_Copy::copy(tmpBuffer1,tmpBuffer2,data_.length()-(theStart+theLength));

  return tmpString;
}

/* ------------------------------------------------------------------------- */
void OTC_String::upper_(long theStart, size_t theLength)
{
  OTCLIB_ENSURE_FN((theStart >= 0 && theStart+theLength <= data_.length()),
   "OTC_String::upper_(long, size_t)",
   "invalid range");

  if (theLength == 0)
    return;

  if (data_.isShared())
    data_.sync();

  for (size_t i=0; i<theLength; i++)
  {
    if (islower(data_[theStart+i]))
      data_[theStart+i] = char(toupper(data_[theStart+i]));
  }
}

/* ------------------------------------------------------------------------- */
void OTC_String::lower_(long theStart, size_t theLength)
{
  OTCLIB_ENSURE_FN((theStart >= 0 && theStart+theLength <= data_.length()),
   "OTC_String::lower_(long, size_t)",
   "invalid range");

  if (theLength == 0)
    return;

  if (data_.isShared())
    data_.sync();

  for (size_t i=0; i<theLength; i++)
  {
    if (isupper(data_[theStart+i]))
      data_[theStart+i] = char(tolower(data_[theStart+i]));
  }
}

/* ------------------------------------------------------------------------- */
int OTC_String::index_(
 size_t theIndex,
 char const* theString,
 size_t theLength
) const
{
  if (data_.length() == 0 || theLength == 0)
    return -1;

  OTCLIB_ENSURE_FN((theIndex < data_.length()),
   "OTC_String::index_(size_t, char const*, size_t)",
   "index out of range");

  size_t theLength1 = data_.length() - theIndex;

  if (theLength > theLength1)
    return -1;

  bool theEnd = false;
  char const* theStart = data_.string() + theIndex;

  char const* a;
  char const* b;

  b = theString;

  while (theEnd == false)
  {
    if (*theStart == *b)
    {
      a = theStart;

      size_t theLength2 = theLength;

      while (theLength2 != 0)
      {
	if (*a++ != *b++)
	  break;

	theLength2--;
      }

      if (theLength2 == 0)
	theEnd = true;

      b = theString;
    }

    if (theEnd == false)
    {
      theLength1--;

      // XXX
      //
      // if (theLength1 == 0)

      if (theLength > theLength1)
      {
	theStart = 0;
	theEnd = true;
      }
      else
	theStart++;
    }
  }

  if (theStart != 0)
  {
    size_t anIndex = theStart - data_.string();
    return anIndex;
  }

  return -1;
}

/* ------------------------------------------------------------------------- */
int OTC_String::rindex_(
 char const* theString,
 size_t theLength
) const
{
  if (data_.length() == 0 || theLength == 0)
    return -1;

  if (theLength > data_.length())
    return -1;

  bool theEnd = false;
  int theIndex = data_.length() - theLength;
  char const* theStart = data_.string() + theIndex;

  char const* a;
  char const* b;

  b = theString;

  while (theEnd == false)
  {
    if (*theStart == *b)
    {
      a = theStart;

      size_t theLength2 = theLength;

      while (theLength2 != 0)
      {
	if (*a++ != *b++)
	  break;

	theLength2--;
      }

      if (theLength2 == 0)
	theEnd = true;

      b = theString;
    }

    if (theEnd == false)
    {
      theIndex--;

      if (theIndex < 0)
      {
	theStart = 0;
	theEnd = true;
      }
      else
	theStart--;
    }
  }

  if (theStart != 0)
  {
    size_t anIndex = theStart - data_.string();
    return anIndex;
  }

  return -1;
}

/* ------------------------------------------------------------------------- */
