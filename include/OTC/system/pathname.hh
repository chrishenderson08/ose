#ifndef OTC_SYSTEM_PATHNAME_HH
#define OTC_SYSTEM_PATHNAME_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     system/pathname.hh
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
#include <OTC/text/record.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/system/pathname.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_Pathname
    // = TITLE
    //	   Class for accessing information about a pathname.
    //
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //	   The <OTC_Pathname> class is intended to hold the pathname
    //	   of a file and allow interogation of whether or not the file
    //	   exists and what access the user is allowed.
    //
    // = NOTES
    //	   Trailing and leading whitespace is not removed from the pathname
    //	   by this class. If you need to remove the whitespace then it must
    //	   be done before initialisation of the class. One way of doing
    //	   this would be to use <OTC_String::trim()>.
    //	   
    //	   The functions <isAccessable()>, <isReadable()>, <isWritable()> and
    //	   <isExecutable()> make use of <access(1)>. Thus the results
    //	   generated are based on the real user ID and supplementary
    //	   group ID (including the real group ID) of the executing
    //	   program.
    //
    //     On non UNIX platforms, a <\\> is not interpreted as a directory
    //     separator. A leading drive name will be recognised though.
    //
    //	   An <OTC_BaseActions> and <OTC_HashActions> class are defined for
    //	   <OTC_Pathname>.
    //	   
    // = SEE ALSO
    //	   <access(2)>, <dirname(1)>, <basename(1)>
{
  public:

			~OTC_Pathname();

    // = INITIALISATION

			OTC_Pathname();
				// Creates an empty pathname.

			OTC_Pathname(char const* thePath);
				// Creates a pathname using <thePath>.

			OTC_Pathname(OTC_Pathname const& thePath);
				// Creates a pathname using <thePath>.

    OTC_Pathname&	operator=(char const* thePath)
				{ path_ = thePath; return *this; }
				// Initialises this path to <thePath>.

    OTC_Pathname&	operator=(OTC_Pathname const& thePath)
				{ path_ = thePath.path_; return *this; }
				// Initialises this path to <thePath>.

    // = QUERY

    OTC_String const&	path() const
				{ return path_; }
				// Returns the pathname.

			operator char const*() const
				{ return path(); }
				// Returns the pathname.

    bool		isAccessable() const;
				// Returns <true> if the file exists and all
				// directories in the path are searchable. In
				// the case of <false> being returned, if
				// <errno> is <ENOENT> then the file truly
				// doesn't exist. If <errno> has some other
				// value then a problem was encountered in
				// accessing the file.

    bool		isWritable() const;
				// Returns <true> if the file is writable.

    bool		isReadable() const;
				// Returns <true> if the file is readable.

    bool		isExecutable() const;
				// Returns <true> if the file is executable.

    // Following functions are currently only available under UNIX
    // like systems.

    OTC_Pathname	absolute() const;
				// Returns the pathname as an absolute path.
				// If the path was already absolute then
				// it is returned unchanged. If it was
				// relative, the current working directory is
				// prepended to it and the result returned.
				// In the case of the pathname being ".", the
				// current working directory is returned
				// without a trailing "/.". If the pathname
				// is empty, an empty pathname is returned.

    OTC_Pathname	dirname() const;
				// Behaves like <dirname(1)>, returning
				// the directory portion of the pathname.

    OTC_Pathname	basename() const;
				// Behaves like <basename(1)>, returning
				// the filename portion of the pathname.

    OTC_String		basename(char const* theSuffix) const;
				// Behaves like <basename(1)>, returning the
				// filename portion of the pathname less
				// <theSuffix> if the file did in fact end in
				// that suffix.

    // Note that if the pathname is empty both <isAbsolute()> and
    // <isRelative()> will return <false>.

    bool		isAbsolute() const;
				// Returns <true> if the pathname commences
				// with a </>.

    bool		isRelative() const;
				// Returns <true> if the pathname doesn't
				// commence with a </>.

    // = EXTENSION

    OTC_Pathname&	operator+=(char const* theString)
				{ path_ += theString; return *this; }
				// Extend the pathname with <theString>.

    OTC_Pathname&	operator+=(OTC_Pathname const& thePath)
				{ path_ += thePath.path_; return *this; }
				// Extend the pathname with <thePath>.

  private:

    // = ADDITION
    //     These have been removed from public use. Assign the pathname
    //     to a string if you want to manipulate it.

    OTC_TString		operator+(char theChar)
				{ return path_ + theChar; }
				// Returns the path with <theChar> appended.
				// This object is not modified.

    OTC_TString		operator+(char const* theString)
				{ return path_ + theString; }
				// Returns the path with <theString> appended.
				// This object is not modified.

    OTC_TString		operator+(OTC_String const& theString)
				{ return path_ + theString; }
				// Returns the path with <theString> appended.
				// This object is not modified.

    OTC_TString		operator+(OTC_Pathname const& thePath)
				{ return path_ + thePath; }
				// Returns the path with <thePath> appended.
				// This object is not modified.

    // = STREAMS
    //     Input stream operator removed from public use as it can allow
    //     embedded nulls into the name. Also, using this prevents use
    //     of spaces in names, not that that is desirable though.

    friend OSE_EXPORT istream&	operator>>(istream& ins, OTC_Pathname& thePath)
				{ ins >> thePath.path_; return ins; }

  private:

#if 0
			OTC_Pathname(OTC_String const& thePath)
			  : path_(thePath) {}
				// Creates a pathname using <thePath>.
#endif

    OTC_String		path_;
				// The path.
};

/* ------------------------------------------------------------------------- */

OSE_TEMPLATE_SPECIALISATION
class OSE_EXPORT OTC_RankActions<OTC_Pathname>
{
  public:
    static int		rank(OTC_Pathname const& p1, OTC_Pathname const& p2)
				{
				  return OTC_RankActions<OTC_String>::rank(
				   p1.path(),p2.path());
				}
};

OSE_TEMPLATE_SPECIALISATION
class OSE_EXPORT OTC_HashActions<OTC_Pathname>
{
  public:
    static int		hash(OTC_Pathname const& p)
				{
				  return OTC_HashActions<OTC_String>::hash(
				   p.path());
				}
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_SYSTEM_PATHNAME_HH */
