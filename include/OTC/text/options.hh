#ifndef OTC_TEXT_OPTIONS_HH
#define OTC_TEXT_OPTIONS_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     text/options.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992 OTC LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/text/string.hh>
#include <OTC/collctn/list.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/text/options.hh"
#endif
#endif

#ifdef _MSC_VER
#pragma warning(disable: 4251)
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_Options
    // = TITLE
    //     Class to encapsulate a list of options.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     This class helps to encapsulate and break options coming
    //     from a string. The set can be created from an <argv>,
    //     <argc> style argument list or from a string. In the case
    //     of a string arguments will be split with whitespace
    //     being the delimiter. If whitespace is required within
    //     an option then it should be delineated with double
    //     quotes.
{
  public:

    // = INITIALISATION

			OTC_Options();
				// Creates and empty list of options.

			OTC_Options(int argc, char* argv[]);
				// Initialise the list of options using
				// <argv> where <argc> is the number of
				// options in <argv>.

			OTC_Options(char const* theArgs);
				// Initialise the list of options by
				// splitting <theArgs>. If spaces are
				// required in a particular options it should
				// be delineated with double quotes. These
				// double quotes will subsequently be
				// dropped. If a real double quote is
				// required then it should be prefixed with a
				// backslash.

			OTC_Options(OTC_Options const& theOptions);
				// Makes a duplicate of <theOptions>.

    OTC_Options&	operator=(OTC_Options const& theOptions);
				// Replaces this set of options
				// with <theOptions>.

    // If a list of options is already held then it will first be deleted
    // when calling either of the following functions.

    void		initialise(int argc, char* argv[]);
				// Initialise the list of options using
				// <argv> where <argc> is the number of
				// options in <argv>.

    void		initialise(char const* theArgs);
				// Initialise the list of options by
				// splitting <theArgs>. If spaces are
				// required in a particular options it should
				// be delineated with double quotes. These
				// double quotes will subsequently be
				// dropped. If a real double quote is
				// required then it should be prefixed with a
				// backslash.

    // = QUERY

    u_int		numOptions() const;
				// Returns the number of options.

    OTC_String const&	option(u_int theNum) const;
				// Return the option at position <theNum>.
				// Generates an exception if <theNum> is
				// greater than the number of options.
				// Note that the first option has an index
				// of <1> and not <0>. If <0> is supplied
				// then an exception will be generated.

    OTC_Iterator<OTC_String>	options(
				 OTC_Direction theDirection=OTCLIB_FORWARD,
				 OTC_Protection theProtection=OTCLIB_SAFE
				) const;
				// Returns an iterator over all the options.

    OTC_Iterator<OTC_String>	options(
				 u_int theStart,
				 u_int theLength,
				 OTC_Direction theDirection=OTCLIB_FORWARD,
				 OTC_Protection theProtection=OTCLIB_SAFE
				) const;
				// Returns an iterator over options in
				// the range commencing at <theStart> and with
				// length <theLength>.

    OTC_Iterator<OTC_String>	options(
				 OTC_Range const& theRange,
				 OTC_Direction theDirection=OTCLIB_FORWARD,
				 OTC_Protection theProtection=OTCLIB_SAFE
				) const;
				// Returns an iterator over options in
				// <theRange>.

    // = OPERATIONS

    void		shift(u_int theNum=1)
				{ offset_ += theNum; }
				// Discard the <theNum> options and shift all
				// other options down one spot. This is
				// analagous to the <shift> operator in shell
				// scripts.

    void		restore()
				{ offset_ = 0; }
				// Restores the set of options after
				// any were discarded using <shift()>.

  private:

    OTC_List<OTC_String>	options_;
				// The list of options.

    u_int		offset_;
				// Index of the first option in the list.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_TEXT_OPTIONS_HH */
