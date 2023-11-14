/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     text/otcvrecord.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1997 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/text/vrecord.hh>

#include <OTC/collctn/copyactn.hh>

/* ------------------------------------------------------------------------- */
OTC_VRecord::~OTC_VRecord()
{
  refcnt_->dec();
  if (refcnt_->count() == 0)
  {
    delete refcnt_;
    delete [] data_;
    delete [] index_;
  }
}

/* ------------------------------------------------------------------------- */
OTC_VRecord::OTC_VRecord()
  : refcnt_(0), data_(0), index_(0), offset_(0), numFields_(0)
{
  refcnt_ = new OTC_Count;
  OTCLIB_ASSERT_M(refcnt_ != 0);

  refcnt_->set(0);
  refcnt_->inc();
}

/* ------------------------------------------------------------------------- */
OTC_VRecord::OTC_VRecord(OTC_String const& theString, char theDelimiter)
  : refcnt_(0), data_(0), index_(0), offset_(0), numFields_(0)
{
  refcnt_ = new OTC_Count;
  OTCLIB_ASSERT_M(refcnt_ != 0);

  refcnt_->set(0);
  refcnt_->inc();

  OTC_CBitSet theBitSet(&theDelimiter,1);;

  split_(theString.string(),theString.length(),theBitSet);
}

/* ------------------------------------------------------------------------- */
OTC_VRecord::OTC_VRecord(
 char const* theString,
 char theDelimiter
)
  : refcnt_(0), data_(0), index_(0), offset_(0), numFields_(0)
{
  OTCLIB_ENSURE_FN((theString != 0),
   "OTC_VRecord::OTC_VRecord(char const*,char)",
   "invalid string");

  refcnt_ = new OTC_Count;
  OTCLIB_ASSERT_M(refcnt_ != 0);

  refcnt_->set(0);
  refcnt_->inc();

  OTC_CBitSet theBitSet(&theDelimiter,1);;

  split_(theString,strlen(theString),theBitSet);
}

/* ------------------------------------------------------------------------- */
OTC_VRecord::OTC_VRecord(
 char const* theString,
 u_int theLength,
 char theDelimiter
)
  : refcnt_(0), data_(0), index_(0), offset_(0), numFields_(0)
{
  OTCLIB_ENSURE_FN((theString != 0 || theLength == 0),
   "OTC_VRecord::OTC_VRecord(char const*,u_int,char)",
   "invalid length or string");

  refcnt_ = new OTC_Count;
  OTCLIB_ASSERT_M(refcnt_ != 0);

  refcnt_->set(0);
  refcnt_->inc();

  OTC_CBitSet theBitSet(&theDelimiter,1);

  split_(theString,theLength,theBitSet);
}

/* ------------------------------------------------------------------------- */
OTC_VRecord::OTC_VRecord(
 OTC_String const& theString,
 OTC_String const& theDelimiters
)
  : refcnt_(0), data_(0), index_(0), offset_(0), numFields_(0)
{
  refcnt_ = new OTC_Count;
  OTCLIB_ASSERT_M(refcnt_ != 0);

  refcnt_->set(0);
  refcnt_->inc();

  OTC_CBitSet theBitSet(theDelimiters.string(),theDelimiters.length());

  split_(theString.string(),theString.length(),theBitSet);
}

/* ------------------------------------------------------------------------- */
OTC_VRecord::OTC_VRecord(
 char const* theString,
 OTC_String const& theDelimiters
)
  : refcnt_(0), data_(0), index_(0), offset_(0), numFields_(0)
{
  OTCLIB_ENSURE_FN((theString != 0),
   "OTC_VRecord::OTC_VRecord(char const*,OTC_CBitSet const&)",
   "invalid string");

  refcnt_ = new OTC_Count;
  OTCLIB_ASSERT_M(refcnt_ != 0);

  refcnt_->set(0);
  refcnt_->inc();

  OTC_CBitSet theBitSet(theDelimiters.string(),theDelimiters.length());

  split_(theString,strlen(theString),theBitSet);
}

/* ------------------------------------------------------------------------- */
OTC_VRecord::OTC_VRecord(
 char const* theString,
 u_int theLength,
 OTC_String const& theDelimiters
)
  : refcnt_(0), data_(0), index_(0), offset_(0), numFields_(0)
{
  OTCLIB_ENSURE_FN((theString != 0 || theLength == 0),
   "OTC_VRecord::OTC_VRecord(char const*,u_int,OTC_CBitSet const&)",
   "invalid length or string");

  refcnt_ = new OTC_Count;
  OTCLIB_ASSERT_M(refcnt_ != 0);

  refcnt_->set(0);
  refcnt_->inc();

  OTC_CBitSet theBitSet(theDelimiters.string(),theDelimiters.length());

  split_(theString,theLength,theBitSet);
}

/* ------------------------------------------------------------------------- */
OTC_VRecord::OTC_VRecord(OTC_VRecord const& theRecord)
  : refcnt_(theRecord.refcnt_),
    data_(theRecord.data_),
    index_(theRecord.index_),
    offset_(theRecord.offset_),
    numFields_(theRecord.numFields_)
{
  refcnt_->inc();
}

/* ------------------------------------------------------------------------- */
OTC_VRecord::OTC_VRecord(
 OTC_VRecord const& theRecord,
 u_int theStart,
 u_int theLength
)
  : refcnt_(theRecord.refcnt_),
    data_(theRecord.data_),
    index_(theRecord.index_),
    offset_(theRecord.offset_),
    numFields_(theRecord.numFields_)
{
  OTCLIB_ENSURE_FN((theStart != 0),
   "OTC_VRecord::OTC_VRecord(OTC_VRecord const&,u_int,u_int)",
   "invalid start index");
  OTCLIB_ENSURE_FN(((theStart+theLength) <= (theRecord.numFields_+1)),
   "OTC_VRecord::OTC_VRecord(OTC_VRecord const&,u_int,u_int)",
   "invalid range");

  refcnt_->inc();

  offset_ += (theStart-1);
  numFields_ = theLength;
}

/* ------------------------------------------------------------------------- */
OTC_VRecord& OTC_VRecord::operator=(OTC_VRecord const& theRecord)
{
  if (&theRecord == this)
    return *this;

  if (theRecord.refcnt_ != refcnt_)
  {
    refcnt_->dec();
    if (refcnt_->count() == 0)
    {
      delete refcnt_;
      delete [] data_;
      delete [] index_;
    }
    refcnt_ = theRecord.refcnt_;
    refcnt_->inc();

    data_ = theRecord.data_;
    index_ = theRecord.index_;
  }

  offset_ = theRecord.offset_;
  numFields_ = theRecord.numFields_;

  return *this;
}

/* ------------------------------------------------------------------------- */
char const* OTC_VRecord::field(u_int theIndex) const
{
  OTCLIB_ENSURE_FN((theIndex != 0 && theIndex <= numFields_),
   "OTC_VRecord::field(u_int)",
   "invalid index");

  return index_[offset_+theIndex-1];
}

/* ------------------------------------------------------------------------- */
u_int OTC_VRecord::length(u_int theIndex) const
{
  OTCLIB_ENSURE_FN((theIndex != 0 && theIndex <= numFields_),
   "OTC_VRecord::field(u_int)",
   "invalid index");

  return (index_[offset_+theIndex] - index_[offset_+theIndex-1]) - 1;
}

/* ------------------------------------------------------------------------- */
OTC_VRecord OTC_VRecord::slice(u_int theStart, u_int theLength) const
{
  return OTC_VRecord(*this,theStart,theLength);
}

/* ------------------------------------------------------------------------- */
void OTC_VRecord::split_(
 char const* theString,
 u_int theLength,
 OTC_CBitSet theDelimiters
)
{
  data_ = new char[theLength+1];
  data_[theLength] = EOS;

  // If there is nothing in the string, we say there are no
  // fields.

  if (theLength == 0)
    return;

  // As we copy the string, remember how many times we saw
  // the delimiter so we now how many fields there are and
  // thus how big to make the index vector. As the string
  // is not empty, we know there will be at least one.

  numFields_ = 1;

  u_int tmpLength = theLength;

  while (tmpLength-- != 0)
  {
    if (theDelimiters.test(data_[tmpLength] = theString[tmpLength]))
      numFields_++;
  }

  // Create index vector.

  index_ = new char*[numFields_+1];
  OTCLIB_ASSERT_M(index_ != 0);

  // Now go look for the delimiter again, replacing them with
  // the EOS character and pointing the index at the start of
  // each field.

  tmpLength = theLength;

  u_int tmpIndex = numFields_;

  while (tmpLength-- != 0)
  {
    if (theDelimiters.test(data_[tmpLength]))
    {
      data_[tmpLength] = EOS;
      index_[--tmpIndex] = &data_[tmpLength+1];
    }
  }

  // Don't forget the first field, which starts at the start
  // of the record.

  index_[0] = &data_[0];

  // Also keep pointer to one past terminating null for last
  // field. This is so we can work out lengths of fields.
  // This is potentially an invalid pointer but is not
  // accessed and is actually allowed by C/C++ standard.

  index_[numFields_] = &data_[theLength+1];
}

/* ------------------------------------------------------------------------- */
