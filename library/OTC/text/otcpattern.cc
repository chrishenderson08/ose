/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     text/otcpattern.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/text/pattern.hh"
#endif
#endif

#include <OTC/text/pattern.hh>

#include <string.h>

#include "../regex/regex.h"

/* ------------------------------------------------------------------------- */
OTC_Pattern::OTC_Pattern(
 OTC_PatternType theType,
 int theFlags,
 char const* thePattern
)
  : regex_(0), match_(0), error_(0)
{
  OTCLIB_ENSURE_FN((thePattern != 0),
   "OTC_Pattern::OTC_Pattern()",
   "invalid argument");

  regex_ = new OTC_CompiledPattern;
  OTCLIB_ASSERT_M(regex_ != 0);

  theFlags |= int(theType);

  int theError = regcomp(regex_,thePattern,theFlags);

  if (theError != 0)
  {
    char buf1[128];
    char buf2[128];
    char buf3[512];

    regerror(REG_ITOA|theError,0,buf1,sizeof(buf1));

    regerror(theError,regex_,buf2,sizeof(buf2));
    sprintf(buf3,"error %s, '%s'",buf1,buf2);

    error_ = new char[strlen(buf3)+1];
    OTCLIB_ASSERT_M(error_ != 0);

    strcpy(error_,buf3);

    regfree(regex_);

    regex_ = 0;
  }
  else
  {
    match_ = new OTC_MatchedPattern[regex_->re_nsub+1];
    OTCLIB_ASSERT_M(match_ != 0);
  }
}

/* ------------------------------------------------------------------------- */
OTC_Pattern::OTC_Pattern(
 OTC_PatternType theType,
 int theFlags,
 char const* thePattern,
 size_t theLength
)
  : regex_(0), match_(0), error_(0)
{
  OTCLIB_ENSURE_FN((thePattern != 0),
   "OTC_Pattern::OTC_Pattern()",
   "invalid argument");

  regex_ = new OTC_CompiledPattern;
  OTCLIB_ASSERT_M(regex_ != 0);

  regex_->re_endp = thePattern + theLength;

  theFlags |= int(theType);
  theFlags |= REG_PEND;

  int theError = regcomp(regex_,thePattern,theFlags);

  if (theError != 0)
  {
    char buf1[128];
    char buf2[128];
    char buf3[512];

    regerror(REG_ITOA|theError,0,buf1,sizeof(buf1));

    regerror(theError,regex_,buf2,sizeof(buf2));
    sprintf(buf3,"error %s, '%s'",buf1,buf2);

    error_ = new char[strlen(buf3)+1];
    OTCLIB_ASSERT_M(error_ != 0);

    strcpy(error_,buf3);

    regfree(regex_);

    regex_ = 0;
  }
  else
  {
    match_ = new OTC_MatchedPattern[regex_->re_nsub+1];
    OTCLIB_ASSERT_M(match_ != 0);
  }
}

/* ------------------------------------------------------------------------- */
OTC_Pattern::OTC_Pattern(
 OTC_PatternType theType,
 int theFlags,
 OTC_String const& thePattern
)
  : regex_(0), match_(0), error_(0)
{
  regex_ = new OTC_CompiledPattern;
  OTCLIB_ASSERT_M(regex_ != 0);

  regex_->re_endp = thePattern.string() + thePattern.length();

  theFlags |= int(theType);
  theFlags |= REG_PEND;

  int theError = regcomp(regex_,thePattern.string(),theFlags);

  if (theError != 0)
  {
    char buf1[128];
    char buf2[128];
    char buf3[512];

    regerror(REG_ITOA|theError,0,buf1,sizeof(buf1));

    regerror(theError,regex_,buf2,sizeof(buf2));
    sprintf(buf3,"error %s, '%s'",buf1,buf2);

    error_ = new char[strlen(buf3)+1];
    OTCLIB_ASSERT_M(error_ != 0);

    strcpy(error_,buf3);

    regfree(regex_);

    regex_ = 0;
  }
  else
  {
    match_ = new OTC_MatchedPattern[regex_->re_nsub+1];
    OTCLIB_ASSERT_M(match_ != 0);
  }
}

/* ------------------------------------------------------------------------- */
OTC_Pattern::~OTC_Pattern()
{
  if (regex_ != 0)
  {
    regfree(regex_);

    delete regex_;
  }

  if (error_ != 0)
    delete [] error_;

  if (match_ != 0)
    delete [] match_;
}

/* ------------------------------------------------------------------------- */
bool OTC_Pattern::match(int theFlags, char const* theString, size_t theLength)
{
  if (regex_ == 0)
    return false;

  OTCLIB_ENSURE_FN((theString != 0),
   "OTC_Pattern::match(int,char const*,size_t)",
   "invalid input string");

  if (error_ != 0)
  {
    delete [] error_;

    error_ = 0;
  }

  theFlags |= REG_STARTEND;

  match_[0].rm_so = 0;
  match_[0].rm_eo = theLength;

  int theError = regexec(regex_,theString,regex_->re_nsub+1,match_,theFlags);

  if (theError != 0)
  {
    char buf1[128];
    char buf2[128];
    char buf3[512];

    regerror(REG_ITOA|theError,0,buf1,sizeof(buf1));

    regerror(theError,regex_,buf2,sizeof(buf2));
    sprintf(buf3,"error %s, '%s'",buf1,buf2);

    error_ = new char[strlen(buf3)+1];
    OTCLIB_ASSERT_M(error_ != 0);

    strcpy(error_,buf3);
  }
  else
    return true;

  return false;
}

/* ------------------------------------------------------------------------- */
bool OTC_Pattern::match(int theFlags, char const* theString)
{
  if (regex_ == 0)
    return false;

  OTCLIB_ENSURE_FN((theString != 0),
   "OTC_Pattern::match(int,char const*)",
   "invalid input string");

  if (error_ != 0)
  {
    delete [] error_;

    error_ = 0;
  }

  int theError = regexec(regex_,theString,regex_->re_nsub+1,match_,theFlags);

  if (theError != 0)
  {
    char buf1[128];
    char buf2[128];
    char buf3[512];

    regerror(REG_ITOA|theError,0,buf1,sizeof(buf1));

    regerror(theError,regex_,buf2,sizeof(buf2));
    sprintf(buf3,"error %s, '%s'",buf1,buf2);

    error_ = new char[strlen(buf3)+1];
    OTCLIB_ASSERT_M(error_ != 0);

    strcpy(error_,buf3);
  }
  else
    return true;

  return false;
}

/* ------------------------------------------------------------------------- */
u_int OTC_Pattern::count() const
{
  if (regex_ == 0)
    return 0;

  return regex_->re_nsub;
}

/* ------------------------------------------------------------------------- */
OTC_Range OTC_Pattern::range(u_int theIndex) const
{
  int theStart = start(theIndex);

  if (theStart == -1)
    return OTC_Range(0);

  return OTC_Range(theStart,length(theIndex));
}

/* ------------------------------------------------------------------------- */
int OTC_Pattern::start(u_int theIndex) const
{
  if (regex_ == 0 || error_ != 0)
    return -1;

  if (theIndex > regex_->re_nsub)
    return -1;

  return match_[theIndex].rm_so;
}

/* ------------------------------------------------------------------------- */
size_t OTC_Pattern::length(u_int theIndex) const
{
  if (regex_ == 0 || error_ != 0)
    return 0;

  if (theIndex > regex_->re_nsub)
    return 0;

  return match_[theIndex].rm_eo -  match_[theIndex].rm_so;
}

/* ------------------------------------------------------------------------- */
