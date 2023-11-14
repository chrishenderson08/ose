#ifndef OTC_MESSAGE_UNIXADDR_HH
#define OTC_MESSAGE_UNIXADDR_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//    message/unixaddr.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/text/string.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/message/unixaddr.hh"
#endif
#endif

#if defined(OSE_SYS_UNIX) && !defined(OSE_SYS_CYGWIN32)

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#if defined(shutdown)
#undef shutdown
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_UnixAddress
    // = TITLE
    //     Class for holding a UNIX style address structure.
{
  public:

#if !defined(socklen_t) && defined(NEED_SOCKLEN_T)
    typedef int		socklen_t;
#endif

    // = DEFAULT ADDRESSES

    static char const* const	ANY;
				// Used to get a pathname in /tmp
				// automatically selected.

    // = INITIALISATION

			OTC_UnixAddress();
				// Initialise an empty internet address
				// structure.

			OTC_UnixAddress(char const* pathname);
				// Initialises the UNIX address structure
				// for <pathname>.

			OTC_UnixAddress(OTC_UnixAddress const& theAddress);
				// Initialises the UNIX address structure
				// to be a copy of <theAddress>.

    // = ASSIGNMENT

    OTC_UnixAddress&	operator=(OTC_UnixAddress const& theAddress);
				// Initialises the UNIX address structure
				// to be a copy of <theAddress>.

    // = PATHNAME

    OTC_String		pathname() const;
				// Returns the pathname of the address.

    // = ADDRESS

    OTC_String		address() const
      				{ return pathname(); }
				// Returns the pathname of the address.

    // = SOCKADDR

    socklen_t&	addrlen()
				{ return length_; }
				// Returns the length of the UNIX address
				// structure which is being used.

    socklen_t		addrlen() const
				{ return length_; }
				// Returns the length of the UNIX address
				// structure which is being used.

    sockaddr* 		addr();
				// Returns the underlying UNIX address
				// structure as a standard socket address.

    sockaddr const*	addr() const;
				// Returns the underlying UNIX address
				// structure as a standard socket address.

			operator sockaddr*()
				{ return addr(); }
				// Returns the underlying UNIX address
				// structure as a standard socket address.

			operator sockaddr const*() const
				{ return addr(); }
				// Returns the underlying UNIX address
				// structure as a standard socket address.

  protected:

    void		resetAddress();
				// Clears the address structure to zeros
				// and resets the length.

    void		initAddress(char const* pathname);
				// Initialises the UNIX address structure for
				// <pathname>.

  private:

    sockaddr_un		address_;
				// UNIX address structure.

    socklen_t		length_;
				// The length of the UNIX address structure
				// being used.
};

/* ------------------------------------------------------------------------- */

#endif

#endif /* OTC_MESSAGE_UNIXADDR_HH */
