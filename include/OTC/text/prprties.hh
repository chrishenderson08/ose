#ifndef OTC_TEXT_PRPRTIES_HH
#define OTC_TEXT_PRPRTIES_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     text/prprties.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1998-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/text/string.hh>
#include <OTC/collctn/tmap.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/text/prprties.hh"
#endif
#endif

#ifdef _MSC_VER
#pragma warning(disable: 4251)
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_Properties
    // = TITLE
    //     Class to encapsulate a properties database.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     This class is able to read in and manage a name/value properties
    //     database which might be used to control the behaviour of an
    //     application. The format of the file is name/value pairs where
    //     the name and value are separated by a colon, ie. ':'. A comment
    //     in the file can be represented by beginning the line with an
    //     exclamation mark.
    //
    //     Each name can optionally be composed of separate components
    //     where each component is separated by a period, ie., '.'. If
    //     a component of a name is a wildcard, ie., '*', if a lookup against
    //     the properties database doesn't result in an exact match, a
    //     wildcard match will be performed. That is, the wildcard will
    //     match any component of the name being looked up which is in
    //     the corresponding position. Ie., "a.*.c" will match "a.b.c".
    //
    // = NOTES
    //     Any error messages describing problems when trying to load in and
    //     merge new entries into the database are sent to the logger on the
    //     default channel and will be displayed. If you do not want such
    //     messages displayed, you will need to define an alternate channel
    //     with the channel name starting with a character other than a letter
    //     or a number, so the default logger will ignore it. The messages
    //     can still be captured with a user defined logger such as
    //     <OTC_SLogger>.
{
  public:

			~OTC_Properties();

    // = INITIALISATION

			OTC_Properties();
				// Creates an empty database.

			OTC_Properties(
			 OTC_Properties const& theDatabase
			);
				// Makes a duplicate of <theDatabase>.

    // = ASSIGNMENT

    OTC_Properties&	operator=(OTC_Properties const& theDatabase)
				{ load(theDatabase); return *this; }
				// Replaces the contents of this database
				// with that of <theDatabase>.

    // = REMOVAL

    void		removeAll();
				// Removes all database entries.

    void		remove(
			 char const* theKeyPrefix,
			 char const* theKey
			);
				// Removes the database entry with <theKey>.
				// If <theKeyPrefix> is not <0>, the key
				// will first be prefixed with <theKeyPrefix>
				// with a "." seperating the two key
				// components.

    void		remove(char const* theKey)
      				{ remove(0,theKey); }
				// Removes the database entry with <theKey>.

    // = LOADING
    //     Note that when defining a key prefix or a key, if you include
    //     a colon, ie., ":", and the property database is then written
    //     out to a file and reread, your intent in having a colon as
    //     part of the key will not be preserved with only that before
    //     the colon being taken as the key and anything after the colon
    //     being taken as part of the value.

    void		load(
			 char const* theKeyPrefix,
			 OTC_Properties const& theDatabase
			)
      				{
				  removeAll();
				  merge(theKeyPrefix,theDatabase);
				}
				// Removes all existing entries in the
				// database and then loads the contents
				// of <theDatabase> into this database. If
				// <theKeyPrefix> is not <0>, all the keys
				// will be prefixed with <theKeyPrefix> with
				// a "." seperating the two key components.

    void		load(OTC_Properties const& theDatabase);
				// Removes all existing entries in the
				// database and then loads the contents
				// of <theDatabase> into this database.

    void		load(
			 char const* theKeyPrefix,
			 istream& theStream,
			 char const* theChannel=0
			)
				{
				  removeAll();
				  merge(theKeyPrefix,theStream,theChannel);
				}
				// Removes all existing entries in the
				// database and then reads in <theStream>,
				// parsing it and loading the entries into
				// the database. If <theKeyPrefix> is not
				// <0>, all the keys will be prefixed with
				// <theKeyPrefix> with a "." seperating the
				// two key components. <theChannel> is the
				// channel used if error messages need to be
				// sent to the logger.

    void		load(
			 istream& theStream,
			 char const* theChannel=0
			)
				{
				  removeAll();
				  merge(0,theStream,theChannel);
				}
				// Removes all existing entries in the
				// database and then reads in <theStream>,
				// parsing it and loading the entries into
				// the database. <theChannel> is the channel
				// used if error messages need to be sent to
				// the logger.

    // = MERGING

    void		merge(
			 char const* theKeyPrefix,
			 OTC_Properties const& theDatabase
			);
				// Merges the contents of <theDatabase> into
				// this database. If <theKeyPrefix> is not
				// <0>, all the keys will be prefixed with
				// <theKeyPrefix> with a "." seperating the
				// two key components. The values of any
				// existing entries which have a matching
				// key, will be replaced.

    void		merge(OTC_Properties const& theDatabase)
      				{ merge(0,theDatabase); }
				// Merges the contents of <theDatabase> into
				// this database. The values of any existing
				// entries which have a matching key, will be
				// replaced.

    void		merge(
			 char const* theKeyPrefix,
			 istream& theStream,
			 char const* theChannel=0
			);
				// Reads in <theStream>, parsing it and
				// merging the entries into the database.
				// If <theKeyPrefix> is not <0>, all the
				// keys will be prefixed with <theKeyPrefix>
				// with a "." seperating the two key
				// components. The values of any existing
				// entries which have a matching key, will be
				// replaced. <theChannel> is the channel used
				// if error messages need to be sent to the
				// logger.

    void		merge(
			 istream& theStream,
			 char const* theChannel=0
			)
      				{ merge(0,theStream,theChannel); }
				// Reads in <theStream>, parsing it and
				// merging the entries into the database. The
				// values of any existing entries which have
				// a matching key, will be replaced.
				// <theChannel> is the channel used if error
				// messages need to be sent to the logger.

    void		merge(
			 char const* theKeyPrefix,
			 char const* theKey,
			 char const* theValue
			);
				// Adds the single entry into the database
				// with <theKey> and corresponding value of
				// <theValue>. If <theKeyPrefix> is not <0>
				// <theKey> will be prefixed with
				// <theKeyPrefix> first, with a "."
				// seperating the two key components. If an
				// existing value already exists for
				// <theKey>, it will be replaced.

    void		merge(
			 char const* theKey,
			 char const* theValue
			)
      				{ merge(0,theKey,theValue); }
				// Adds the single entry into the database
				// with <theKey> and corresponding value
				// of <theValue>. If an existing value
				// already exists for <theKey>, it will
				// be replaced.

    // = LOOKUP

    OTC_String const&	lookup(
			 char const* theKeyPrefix,
			 char const* theKey
			) const;
				// Will look for a value corresponding to
				// <theKey>. If <theKeyPrefix> is not <0>
				// <theKey> will be prefixed with
				// <theKeyPrefix> first, with a "."
				// seperating the two key components.
				// If a corresponding key wasn't
				// found, an undefined string will be
				// returned. Ie., an empty string for which
				// the member <isUndefined()> will return
				// <true>. You can also compare the
				// string against the string returned by
				// <OTC_String::undefinedString()>.

    OTC_String const&	lookup(char const* theKey) const
      				{ return lookup(0,theKey); }
				// Will look for a value corresponding to
				// <theKey>. If a corresponding key wasn't
				// found, an undefined string will be
				// returned. Ie., an empty string for which
				// the member <isUndefined()> will return
				// <true>. You can also compare the
				// string against the string returned by
				// <OTC_String::undefinedString()>.

    // = OUTPUT

    void		save(ostream& theStream) const;
				// Will save the contents of the database
				// onto <theStream>.

    OTC_String		encode() const;
    				// Formats the contents of the database
				// and returns the result as a string.
				// The returned value is suitable for
				// feeding back into the <load()> routine
				// via a stream.

  private:

    void		output_(
			 ostream& theStream,
			 OTC_String const& theString
			) const;
				// Outputs <theString> onto <theStream>.
				// Any instance of a newline will be
				// output with a preceeding backslash
				// character.

    bool		matchPattern_(
			 char const* thePattern,
			 char const* theString
			) const;
				// Returns <true> if <thePattern> matches
				// <theString> using a wildcard match.

    OTC_TMap<OTC_String,OTC_String>	standard_;
				// Standard entries. Ie., where there are
				// no wildcard patterns in the key.

    OTC_TMap<OTC_String,OTC_String>	wildcard_;
				// Wildcard entries. Ie., those which it
				// is necessary to iterate over and do
				// glob style pattern matching to deduce
				// equivalence.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_TEXT_PRPRTIES_HH */
