#ifndef OTC_MESSAGE_INETADDR_HH
#define OTC_MESSAGE_INETADDR_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     message/inetaddr.hh
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

#include <sys/types.h>

#if defined(OSE_SYS_UNIX)
#include <sys/socket.h>
#include <netinet/in.h>
#if defined(shutdown)
#undef shutdown
#endif
#endif

#if defined(OSE_SYS_WIN32)
#include <winsock2.h>
#include <windows.h>
#endif

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/message/inetaddr.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_InetAddress
    // = TITLE
    //     Class for holding an internet style address structure.
{
  public:

#if !defined(socklen_t) && defined(NEED_SOCKLEN_T)
    typedef int		socklen_t;
#endif

    // = DEFAULT ADDRESSES

    static char const* const	ANY;
				// Address for selecting any host.
				// ie. "0.0.0.0".

    static char const* const	BROADCAST;
				// Address for broadcasting.
				// ie. "255.255.255.255".

    // = INITIALISATION

			OTC_InetAddress();
				// Initialise an empty internet address
				// structure.

			OTC_InetAddress(char const* theHostName, int thePort);
				// Initialises the internet address structure
				// for <thePort> on <theHostName>.

			OTC_InetAddress(int thePort);
				// Initialises the internet address structure
				// for <thePort> on any host.

			OTC_InetAddress(
			 char const* theHostName,
			 char const* theService,
			 char const* theProtocol
			);
				// Initialises the internet address structure
				// for <theService> on <theHostName> with
				// <theProtocol>.

			OTC_InetAddress(
			 char const* theService,
			 char const* theProtocol
			);
				// Initialises the internet address structure
				// for <theService> on any host with
				// <theProtocol>.

			OTC_InetAddress(OTC_InetAddress const& theAddress);
				// Initialises the internet address structure
				// to be a copy of <theAddress>.

    // = ASSIGNMENT

    OTC_InetAddress&	operator=(OTC_InetAddress const& theAddress);
				// Initialises the internet address structure
				// to be a copy of <theAddress>.

    // = HOST/PORT

    OTC_String		hostname() const;
				// Returns the name of the host which is
				// being addressed. If the ascii name for the
				// host couldn't be determined then it is
				// returned in numeric form.

    OTC_String		numericHostname() const;
				// Returns the name of the host which
				// is being addressed in numeric form.

    int			port() const;
				// Returns the port to which the address
				// refers.

    // = ADDRESS

    OTC_String		address() const;
    				// Returns the address as a string which
				// consists of the name of the host followed
				// by a colon and the port number. If the
				// ascii name for the host couldn't be
				// determined then it is returned in numeric
				// form.

    OTC_String		numericAddress() const;
    				// Returns the address as a string which
				// consists of the name of the host followed
				// by a colon and the port number. The
				// name of the host will be in numeric form.

    // = SOCKADDR

    socklen_t&		addrlen()
				{ return length_; }
				// Returns the length of the internet address
				// structure being used.

    socklen_t		addrlen() const
				{ return length_; }
				// Returns the length of the internet address
				// structure being used.

    sockaddr*		addr();
				// Returns the underlying internet address
				// structure as a standard socket address.

    sockaddr const*	addr() const;
				// Returns the underlying internet address
				// structure as a standard socket address.

			operator sockaddr*()
				{ return addr(); }
				// Returns the underlying internet address
				// structure as a standard socket address.

			operator sockaddr const*() const
				{ return addr(); }
				// Returns the underlying internet address
				// structure as a standard socket address.

  protected:

    void		resetAddress();
				// Clears the address structure to zeros
				// and resets the length.

    void		initAddress(char const* theHostname, int thePort);
				// Initialises the internet address structure
				// for <thePort> on <theHostname>.

    void		initAddress(int thePort)
				{ initAddress(ANY,thePort); }
				// Initialises the internet address structure
				// for <thePort> on any host.

    void		initAddress(
			 char const* theHostName,
			 char const* theService,
			 char const* theProtocol
			);
				// Initialises the internet address structure
				// for <theService> on <theHostName> with
				// <theProtocol>.

    void		initAddress(
			 char const* theService,
			 char const* theProtocol
			)
				{ initAddress(ANY,theService,theProtocol); }
				// Initialises the internet address structure
				// for <theService> on any host with
				// <theProtocol>.

  private:

    sockaddr_in		address_;
				// Internet address structure.

    socklen_t		length_;
				// Length of address.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MESSAGE_INETADDR_HH */
