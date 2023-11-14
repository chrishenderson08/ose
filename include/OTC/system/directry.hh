#ifndef OTC_SYSTEM_DIRECTRY_HH
#define OTC_SYSTEM_DIRECTRY_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     system/directry.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1991-1992 OTC LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/text/string.hh>
#include <OTC/collctn/deque.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/system/directry.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

typedef OTC_Deque<OTC_String> otc_files_directry_hh_typedef1;

class OSE_EXPORT OTC_Directory
    // = TITLE
    //	   Iterator interface to system directory routines.
    //
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //	   The <OTC_Directory> class is a wrapper around the <opendir(3)>,
    //	   <readdir(3)> system calls, or in the case of WIN32 around the
    //     <FindFirstFile()> and <FindNextFile()> system calls.
    //
    // = NOTES
    //     The results provided by this class under WIN32 are undefined,
    //     if the directory name contains '*' or '?'.
{
  public:

    // = INITIALISATION

			OTC_Directory(char const* theName);
				// Creates an instance of this class for the
				// directory <theName>. Note that <theName>
				// is used as is, ie. leading and trailing
				// whitespace is not removed. If this is
				// required it should be done before hand.

			OTC_Directory(OTC_Directory const& theDir);
				// Creates another class for the directory
				// <theDir>.

    OTC_Directory&	operator=(char const* theName)
				{ name_ = theName; return *this; }
				// Initialises this directory to now
				// reference the directory given by
				// <theName>.

    OTC_Directory&	operator=(OTC_Directory const& theDir)
				{ name_ = theDir.name_; return *this; }
				// Initialises this directory to now
				// reference the directory given by
				// <theDir>.

    // = QUERY

    OTC_String const&	name() const
				{ return name_; }
				// Returns the name of the directory.

    bool		isSearchable() const;
				// Returns <true> if the file named by the
				// string to the constructor is actually a
				// directory and that the directory exists
				// and is searchable. This is determined by
				// trying to open it as a directory.

    // = FILES

    OTC_Iterator<OTC_String>	files() const;
				// Returns an iterator over a snapshot of the
				// names of the files in this directory. Any
				// changes made to the directory while this
				// iterator is active will not be reflected
				// in the iterator. If the directory is not
				// searchable then a null iterator is
				// returned.

    OTC_Iterator<OTC_String>	files(char const* thePattern) const;
				// Returns an iterator over a snapshot of the
				// names of the files in this directory which
				// match the glob pattern <thePattern>. Any
				// changes made to the directory while this
				// iterator is active will not be reflected
				// in the iterator. If the directory is not
				// searchable then a null iterator is
				// returned.

    bool		contains(char const* theName) const;
				// Returns <true> if this directory contains
				// a file with name <theName>.

  private:

    OTC_String		name_;
				// The name of the directory.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_SYSTEM_DIRECTRY_HH */
