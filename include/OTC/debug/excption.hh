#ifndef OTC_DEBUG_EXCPTION_HH
#define OTC_DEBUG_EXCPTION_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     debug/excption.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992-1993 OTC LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifndef OTC_OTC_H
#include <OTC/OTC.h>
#endif
#ifndef OTC_THREAD_NRMUTEX_HH
#include <OTC/thread/nrmutex.hh>
#endif

#include <OTC/stdlib/iostream.h>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/debug/excption.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OTC_Exception;
typedef void (*OTC_ThrowFunction)(OTC_Exception const&);

class OSE_EXPORT OTC_Exception
    // = TITLE
    //     Base class for exceptions.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     This class is intended to be the ultimate base class for all
    //     exceptions. The class saves away information about the error which
    //     has occurred. This information can be queried directly or a
    //     message composing all the information can be dumped to an
    //     <ostream>.
    //     
    //     The class does not make copies of the character strings it is
    //     given, nor does it delete them. Thus, character strings passed to
    //     the constructor of the class should be literal strings, or
    //     otherwise be guaranteed not to be deleted.
    //     
    // = EXAMPLE
    // = BEGIN<CODE>
    //     OTC_Exception exception("Underflow");
    //     throw exception;
    //
    //     ...
    //
    //     catch (OTC_Exception const& exception)
    //     {
    //       exception.display(cerr);
    //       terminate();
    //     }
    // = END<CODE>
{
  public:

    virtual		~OTC_Exception();
				// Does nothing. Defined only to make
				// destructor virtual so that derived class
				// destructors will be correctly called if
				// class destroyed through base class
				// pointer.

    // = INITIALISATION

			OTC_Exception(char const* theError=0);
				// Saves <theError> as the description of the
				// type of error which has occurred. If
				// <theError> is <0>, a description of
				// <"Unknown error"> is saved.

			OTC_Exception(
			 char const* theError,
			 char const* theFile,
			 u_int theLine
			);
				// Saves <theError> as the description of the
				// type of error which has occurred. If
				// <theError> is <0>, a description of
				// <"Unknown error"> is saved. <theFile>
				// and <theLine> should be the name of the
				// file containing, and the line number in
				// the file, at which the code raising
				// the exception is located. These can be
				// generated using the preprocessor symbols
				// <__FILE__> and <__LINE__>.

			OTC_Exception(OTC_Exception const& theException);
				// Uses the description of the exception from
				// <theException>.

    // = QUERY

    char const*		error() const
				{ return error_; }
				// Returns a description of the type of error
				// which has occurred.

    char const*		file() const
				{ return file_; }
				// Returns the name of file in which the code
				// raising the exception is located.
				// Will return <0> if the name of the file
				// is not known.

    u_int		line() const
				{ return line_; }
				// Returns the line number in the file at
				// which the code raising the exception is
				// located. Will return <0> if the line
				// in the file is not known.

    // = DISPLAY

    virtual void	display(ostream& outs) const;
				// Dumps a message which composes together
				// all the information about the error on the
				// stream <outs>. This should be redefined
				// in derived class to first call the
				// base class version of the function, ie.,
				// <OTC_Exception::display()>, and then
				// dump out any additional information which
				// is kept in the derived class. The derived
				// class must terminate each line of
				// information with an <EOL> and should
				// not use <endl> or <flush>.

    // = THROW

    static void		throwException(
			 OTC_Exception& theException,
			 char const* theFile=0,
			 u_int theLine=0
			);
				// Throws <theException>. If exceptions
				// are not supported by the compiler,
				// details of the exception are displayed
				// via the logger and <terminate()> called.
				// If <theFile> and <theLine> are defined
				// and the file and line number are not
				// already set in the exception class,
				// those in the exception class will be
				// set using <theFile> and <theLine>.

    static OTC_ThrowFunction	setThrow(OTC_ThrowFunction theFunction);
				// Set the function to be called when an
				// exception is thrown. This allows
				// something over than the default
				// action to be performed. For example,
				// a home grown exception could be
				// generated. The previously defined
				// throw function is returned.

    // = LOGGING
    //     If C++ exceptions are supported by the compiler, the default
    //     behaviour is that the details of an exception are not output to
    //     the logger. This behaviour for the case when C++ exceptions are
    //     available can be overridden by setting the environment variable
    //     <OTCLIB_LOGEXCEPTION>. You can also change this behaviour using
    //     the following functions. It is not recommended that you disable
    //     the display of the details of an exceptions when the compiler
    //     doesn't support C++ exceptions as an exception in that case causes
    //     the process to abort. If the display of exception details was
    //     disabled you wouldn't know why your program exited. For compilers
    //     where C++ exceptions are supported, they generally output some
    //     message when a C++ exception occurs which isn't caught.

    static bool		loggingEnabled();
				// Returns <true> if output of the details of
				// an exception when raised are displayed
				// using the logger.

    static void		enableLogging();
				// Enables the output of the details of an
				// exception to the logger.

    static void		disableLogging();
				// Disables the output of the details of an
				// exception to the logger.

  protected:

    // = DERIVED

    virtual void	throwIt() const;
				// Must be redefined in a derived class to
				// throw this object as an exception. If not
				// done, derived part of class will be
				// truncated. Ie., derived version must
				// contain "<throw *this>". This function
				// will be called by the <throwException()>
				// static member function. This function
				// should not be called directly. This odd
				// arrangement of functions derives from when
				// the library tried to emulate some
				// exception functionality where compilers
				// did not support exceptions. It is now
				// expected that compilers support
				// exceptions.

  private:

    OTC_Exception&	operator=(OTC_Exception const&);
				// Do not define an implementation for this.

    static void		initialise();
    				// Initialises exception configuration.

    static OTC_NRMutex	mutex_;
				// Lock for threads. Can use a non recursive
				// mutex as there is no chance of an attempt
				// to acquire lock from inside scope of lock.

    static OTC_ThrowFunction	globThrowFunction_;
				// Function to be called to throw an
				// exception.

    static bool		globInitialised_;
				// Flag indicating whether global data
				// for class has been initialised.

    static bool		globLogException_;
				// Flag indicating whether details of the
				// exception should be logged even when
				// real exceptions are available.

    char const*		error_;
				// The description of the type of
				// error which has occurred.

    char const*		file_;
				// File in which the exception was
				// raised.

    u_int		line_;
				// Line in the file at which the exception
				// as raised.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DEBUG_EXCPTION_HH */
