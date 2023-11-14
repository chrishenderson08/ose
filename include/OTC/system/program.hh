#ifndef OTC_SYSTEM_PROGRAM_HH
#define OTC_SYSTEM_PROGRAM_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     system/program.hh
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

#include <OTC/text/options.hh>
#include <OTC/text/prprties.hh>
#include <OTC/collctn/hmap.hh>
#include <OTC/thread/nrmutex.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/system/program.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

enum OTC_UniqueIdType
{
  OTCLIB_ID_LONG_FORMAT=0,
  OTCLIB_ID_SHORT_FORMAT=1
};

class OSE_EXPORT OTC_Program
    // = TITLE
    //	   Class to encapsulate information about a program.
    //
    // = CLASS TYPE
    //	   Static
    //
    // = DESCRIPTION
    //	   The <OTC_Program> class encapsulates information about a program.
    //	   This class should be initialised once using <initialise()> at the
    //	   start of <main()> using the programs options. Upon doing this it
    //	   will maintain a copy of the name of the program and the options to
    //	   the program. As well as command line options, the class also
    //	   provides a limited range of other information regarding the
    //	   executing process. The class also provides a central point
    //	   for generating system wide unique ids which encapsulate
    //	   information about the host, initialisation time, process id and
    //	   process unique time/counter id. Finally, the class holds a
    //	   central configuration database. This should be loaded from
    //	   in <main()>. It can then be queried from anywhere in the
    //	   program.
    //	   
    // = NOTES
    //	   It is safe to use the iterators which can be obtained from
    //	   this class in different threads of a multi threaded application
    //	   as they are created in unsafe mode and therefore do not try to
    //	   do reference counting on the internals of the data structure
    //	   holding the options. It not however safe to use the <shift()> and
    //	   <restore()> member functions from different threads at the same
    //	   time.
    //
    // = SEE ALSO
    //	   <OTC_Properties>
{
  public:

    // = INITIALISATION

    static void		initialise(int argc, char* argv[]);
				// Initialise the name of the program and its
				// options.

    // = PROGRAM

    static OTC_String const&	name();
				// Returns the name of the program.

    // Following functions currently only handle UNIX style pathnames.

    static OTC_String const&	basename();
				// Returns the basename part of the
				// program name. ie. it removes the
				// directory information.

    static OTC_String const&	dirname();
				// Returns the directory in which the
				// program was located.

    // = OPTIONS

    static int		numOptions();
				// Returns the number of options supplied to
				// the program. Unlike <argc> this does not
				// include the name of the program in the
				// count. If this class has not been
				// initialised, then <-1> will be returned.

    static OTC_String const&	option(u_int theNum=1);
				// Return the option at position <theNum>.
				// Generates an exception if <theNum> is
				// greater than the number of options.
				// If <theNum> is <0> then the name of the
				// program is returned.

    static bool		optionMatches(u_int theNum, char const* thePattern);
				// Performs a glob style match against the
				// option at position <theNum> and returns
				// <true> if sucessful. Generates an
				// exception if <theNum> is greater than the
				// number of options. If <theNum> is <0> then
				// the name of the program will be matched
				// against.

    static bool		optionMatches(char const* thePattern)
				{ return optionMatches(1,thePattern); }
				// Performs a glob style match against the
				// the first option and returns <true> if
				// sucessful. Generates an exception if
				// there are no options. If <theNum> is <0>
				// then the name of the program will be
				// matched against.

    static bool		match(char const* thePattern, u_int theNum=1)
				{ return optionMatches(theNum,thePattern); }
				// Performs a glob style match against the
				// option at position <theNum> and returns
				// <true> if sucessful. Generates an
				// exception if <theNum> is greater than the
				// number of options. If <theNum> is <0> then
				// the name of the program will be matched
				// against. This function is obsolete, use
				// <optionMatches() instead.

    static OTC_Iterator<OTC_String>	options(
				 OTC_Direction theDirection=OTCLIB_FORWARD
				);
				// Returns an iterator over just the
				// options to the program.

    static OTC_Iterator<OTC_String>	options(
				 u_int theStart,
				 u_int theLength,
				 OTC_Direction theDirection=OTCLIB_FORWARD
				);
				// Returns an iterator over the options
				// in the range commencing at <theStart>
				// and with length <theLength>.

    static OTC_Iterator<OTC_String>	options(
				 OTC_Range const& theRange,
				 OTC_Direction theDirection=OTCLIB_FORWARD
				);
				// Returns an iterator over the options in
				// <theRange>.

    static void		shift(u_int theNum=1);
				// Discard the <theNum> options and shift all
				// other options down one spot. This is
				// analagous to the <shift> operator in shell
				// scripts.

    static void		restore();
				// Restores the set of options after
				// any were discarded using <shift()>.

    // = CONFIGURATION

    static void		removeAllConfig();
				// Removes all configuration database
				// entries.

    static void		removeConfig(char const* theKey);
				// Removes the configuration database entry
				// with <theKey>.

    static void		loadConfig(OTC_Properties const& theDatabase);
				// Removes all existing entries in the
				// configuration database and then loads the
				// contents of <theDatabase> into this
				// database.

    static void		loadConfig(
			 istream& theStream,
			 char const* theChannel=0
			);
				// Removes all existing entries in the
				// configuration database and then reads in
				// <theStream>, parsing it and loading the
				// entries into the database. <theChannel>
				// will be used to display an error messages
				// if problems occur.

    static void		mergeConfig(OTC_Properties const& theDatabase);
				// Merges the contents of <theDatabase> into
				// the configuration database. The values of
				// any existing entries which have a matching
				// key, will be replaced.

    static void		mergeConfig(
			 istream& theStream,
			 char const* theChannel=0
			);
				// Reads in <theStream>, parsing it and
				// merging the entries into the configuration
				// database. The values of any existing
				// entries which have a matching key, will be
				// replaced. <theChannel> will be used to
				// display an error messages if problems
				// occur.

    static void		mergeConfig(
			 char const* theKey,
			 char const* theValue
			);
				// Adds the single entry into the configuration
				// database with <theKey> and corresponding
				// value of <theValue>. If an existing value
				// already exists for <theKey>, it will be
				// replaced.

    static OTC_String const&	lookupConfig(char const* theKey);
				// Will look for a value corresponding to
				// <theKey> in the configuration database. If
				// a corresponding key wasn't found, an
				// undefined string will be returned. Ie., an
				// empty string for which the member
				// <isUndefined()> will return <true>. You
				// can also compare the string against
				// the string returned by
				// <OTC_String::undefinedString()>.

    static OTC_String const&	lookupConfig(
			 char const* theKeyPrefix,
			 char const* theKey
			);
				// Will look for a value corresponding to
				// <theKey> with <theKeyPrefix> prepended. A
				// "." character will be placed between the
				// two values automatically. If a
				// corresponding key wasn't found, an
				// undefined string will be returned. Ie., an
				// empty string for which the member
				// <isUndefined()> will return <true>. You
				// can also compare the string against
				// the string returned by the function
				// <OTC_String::undefinedString()>.

    static void		saveConfig(ostream& theStream);
				// Will save the contents of the
				// configuration database onto <theStream>.

    // = ENVIRONMENT
    //     Following functions provide an interface on top of the system
    //     <getenv()> and <putenv()> functions. One difference is that when
    //     <mergeEnviron()> is used, heap memory is used with a local copy of
    //     memory supplied to <putenv()> being kept. If the system doesn't
    //     actually use this memory because there is sufficient space within
    //     existing variable memory, or when a subsequent change is made
    //     through this interface, or it is noticed that a change had been
    //     made direct through <putenv()>, the memory is released. This
    //     avoids a problem with <putenv()> on some platforms whereby you can
    //     only safely provide static memory as <putenv()> doesn't take a
    //     copy. On such systems, if you provide heap memory and keep making
    //     changes to the environment variable, you can end up with memory
    //     leaks as you can't easily detect that you can delete the existing
    //     memory being used to hold the details of the environment variable.

    static OTC_String	lookupEnviron(char const* theName);
				// Returns the value associated with the
				// environment variable with <theName>. If no
				// such environment variable exists, an
				// undefined string will be returned. Ie., an
				// empty string for which the member function
				// <isUndefined()> will return <true>.
				// You can also compare the returned string
				// against the string returned by the function
				// <OTC_String::undefinedString()>.

    static void		mergeEnviron(
			 char const* theName,
			 char const* theValue
			);
				// Sets the environment variable with
				// <theName> to <theValue>.

    static OTC_String	expandEnviron(char const* theString);
				// Parses <theString> and returns a result
				// which has had any Bourne Shell style
				// references to environment variables
				// replaced with the actual values associated
				// with those environment variables. Ie.,
				// if <theString> contains "$TZ" or "${TZ}",
				// that part of the string will be replaced
				// with the actual value of the environment
				// variable called "TZ". If an environment
				// variable referred to doesn't exist, that
				// part of the string will be replaced with
				// an empty string.

    // = PROCESS

    static OTC_String const&	hostname();
				// The name of the host on which the program
				// is executing.

    static long		processId();
				// The id of the process executing the
				// program.

    static OTC_String	processIdentity();
    				// A string comprising the name of the host
				// and the process ID.

    // = UNIQUE ID
    //     The following functions, return a string which in most cases can
    //     be used as a system wide unique id although there isn't a
    //     gaurantee on that as it requires all process to cooperate. Two
    //     types of id string can be obtained by setting the argument
    //     <theType>. The first is defined by <OTCLIB_ID_LONG_FORMAT>. The
    //     long format is such that it will result in the host IP number as a
    //     single number, process id, initialisation time for id system in
    //     this process, an incrementing counter and the current time being
    //     encoded into the id string. Alternatively, to obtain the short
    //     format id use the value <OTCLIB_ID_SHORT_FORMAT>. This format
    //     contains only the host IP number, process ID and incrementing
    //     counter value. Each value separated by a colon. Each call will
    //     return a different and unique value. Note that with the short
    //     form, two problems can occur. First is that the counter could
    //     feasibly eventually wrap around and secondly, the process ID could
    //     be reused. Only use the short format if the process would result
    //     in IDs being discarded anyway and the likely hood of IDs being
    //     kept until a point that the counter wrapped around is highly
    //     unlikely. The default type is the long format.

    static OTC_String	uniqueId(
			 char const* thePrefix,
			 OTC_UniqueIdType theType=OTCLIB_ID_LONG_FORMAT
			);
				// Returns a string which in most cases can
				// be used as a psuedo system wide unique id.
				// <thePrefix> is a string which will be
				// prepended to the id string returned. If
				// <thePrefix> is <0> no prefix is added.
				// <theType> can be used to specify a short
				// or long format.

    static OTC_String	uniqueId(
			 OTC_UniqueIdType theType=OTCLIB_ID_LONG_FORMAT
			)
      				{ return uniqueId(0,theType); }
				// Returns a string which in most cases can
				// be used as a psuedo system wide unique id.
				// <theType> can be used to specify a short
				// or long format.

    // = THREADING

    static bool		threadingEnabled();
				// Returns <true> if threading support has
				// been compiled into this library.

  private:

    static OTC_NRMutex	mutex_;
				// Lock for threads.

    static OTC_NRMutex	idmutex_;
				// Lock for threads. Special to unique
				// id code.

    static OTC_String*	gName_;
				// The name of the program.

    static OTC_Options* gOptions_;
				// The options to the program.

    static OTC_String*	gBasename_;
				// The basename part of the program name.

    static OTC_String*	gDirname_;
				// The directory part of the program name.

    static OTC_String*	gHostname_;
				// The name of the host the program is
				// executing on.

    static OTC_String*	gHostnameIP_;
				// The encoded IP of the host the program is
				// executing on.

    static u_long	gInitTime_;
				// Time id system was initialised.

    static u_long	gProcessId_;
				// Process id when id system was initialised.

    static u_int	gCounter_;
				// Counter of integer ids. The value <0>
				// will never be used.

    static OTC_Properties*	gConfig_;
				// Config database.

    static OTC_HMap<OTC_String,OTC_String>*	gEnviron_;
				// Environment cache for changed variables
				// so memory is deleted properly.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_SYSTEM_PROGRAM_HH */
