/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     debug/otcexcption.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992 OTC LIMITED
//     Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/debug/excption.hh"
#endif
#endif

#include <OTC/debug/excption.hh>
#include <OTC/debug/logstrm.hh>
#include <OTC/debug/logger.hh>

#include <OTC/stdlib/iostream.h>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTC_Exception::mutex_;
OTC_ThrowFunction OTC_Exception::globThrowFunction_ = 0;
bool OTC_Exception::globInitialised_ = false;
bool OTC_Exception::globLogException_ = false;

/* ------------------------------------------------------------------------- */
OTC_Exception::OTC_Exception(char const* theError)
  : error_(theError), file_(0), line_(0)
{
  if (error_ == 0)
    error_ = "Unknown error";
}

/* ------------------------------------------------------------------------- */
OTC_Exception::OTC_Exception(
 char const* theError,
 char const* theFile,
 u_int theLine
)
  : error_(theError), file_(theFile), line_(theLine)
{
  if (error_ == 0)
    error_ = "Unknown error";
}

/* ------------------------------------------------------------------------- */
OTC_Exception::OTC_Exception(OTC_Exception const& theException)
  : error_(theException.error_),
    file_(theException.file_),
    line_(theException.line_)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_Exception::~OTC_Exception()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void OTC_Exception::display(ostream& outs) const
{
  mutex_.lock();

  initialise();

  mutex_.unlock();

  outs << error_ << EOL;

  if (file_ != 0)
    outs << "Location: " << file_ << ", line " << line_ << EOL;
}

/* ------------------------------------------------------------------------- */
void OTC_Exception::throwException(
 OTC_Exception& theException,
 char const* theFile,
 u_int theLine
)
{
  if (theException.file_ == 0)
  {
    theException.file_ = theFile;
    theException.line_ = theLine;
  }

  OTC_ThrowFunction theFunction;
  bool theLogException;

  mutex_.lock();

  initialise();

  theFunction = globThrowFunction_;

  theLogException = globLogException_;

  mutex_.unlock();

  if (theFunction)
    theFunction(theException);

  if (theLogException != false)
  {
    char buf[8192];
    OTC_LogStream outs(buf,sizeof(buf));
    outs.setLevel(OTCLIB_LOG_ERROR);
    theException.display(outs);
    outs.flush();
  }

  theException.throwIt();
}

/* ------------------------------------------------------------------------- */
OTC_ThrowFunction OTC_Exception::setThrow(OTC_ThrowFunction theFunction)
{
  OTC_ThrowFunction oldFunction;

  mutex_.lock();

  oldFunction = globThrowFunction_;
  globThrowFunction_ = theFunction;

  mutex_.unlock();

  return oldFunction;
}

/* ------------------------------------------------------------------------- */
void OTC_Exception::throwIt() const
{
  throw *this;
}

/* ------------------------------------------------------------------------- */
bool OTC_Exception::loggingEnabled()
{
  bool theResult;

  mutex_.lock();

  initialise();

  theResult = globLogException_;

  mutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
void OTC_Exception::enableLogging()
{
  mutex_.lock();

  initialise();

  globLogException_ = true;

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_Exception::disableLogging()
{
  mutex_.lock();

  initialise();

  globLogException_ = false;

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_Exception::initialise()
{
  if (globInitialised_ == false)
  {
    globInitialised_ = true;

    char const* theVariable;
    theVariable = getenv("OTCLIB_LOGEXCEPTION");
    if (theVariable != 0)
      globLogException_ = true;
  }
}

/* ------------------------------------------------------------------------- */
