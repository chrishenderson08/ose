#ifndef OTC_DISPATCH_SERVBIND_HH
#define OTC_DISPATCH_SERVBIND_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/servbind.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/text/string.hh>
#include <OTC/collctn/baseactn.hh>
#include <OTC/collctn/rankactn.hh>
#include <OTC/collctn/hashactn.hh>
#include <OTC/refcnt/resource.hh>
#include <OTC/thread/nrmutex.hh>
#include <OTC/thread/atomic.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/dispatch/servbind.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

enum OTC_ServiceLocation
{
  OTCLIB_SERVICE_LOCAL=0x01,
  OTCLIB_SERVICE_REMOTE=0x02
};

class OSE_EXPORT OTC_ServiceBinding
    // = TITLE
    //     Holds a description of a service, including its properties.
    //
    // = DESCRIPTION
    //     The <OTC_ServiceBinding> class holds information about an
    //     available service, including where it resides and its properties.
    //
    // = NOTES
    //     This class uses clone/destroy paradigm for handling reference
    //     counting. There is an <OTC_BaseActions> class defined which calls
    //     a <clone()> in the <add()> method and <destroy()> in the
    //     <remove()> method. This means that if you have responsibility for
    //     destroying the instance of this class passed into a collections
    //     add function, you must call <destroy()> on your instance.
{
    friend		class OTC_SVRegistry;
    friend		class OTC_SVBroker;

  private:

    // = INITIALISATION

			OTC_ServiceBinding(
                         char const* theServiceName,
                         char const* theServiceAudience,
                         char const* theReceiverName,
                         char const* theProcessAddress=0,
			 char const* theNetworkGroup=0
			);
				// Creates a binding for a service having
				// <theServiceName> and where how widely
				// the service is known is dicated by
				// <theServiceAudience>. <theReceiverName> is
				// the address within the process for sending
				// messages to the service, with
				// <theProcessAddress> being the address
				// prefix for the process in which the
				// service is located. If <theProcessAddress>
				// is <0> or empty, the service is taken to
				// be local to the process. If not local to a
				// process, <theNetworkGroup> identifies the
				// interprocess network connection through
				// which knowledge of the service arrived in
				// the process.

  public:

    // = QUERY

    OTC_String const&   serviceName() const
                                { return serviceName_; }
                                // Returns the instance name of the service.

    OTC_String const&   serviceAudience() const
                                { return serviceAudience_; }
                                // Returns the scope of the service.

    OTC_String const&   receiverName() const
                                { return receiverName_; }
				// Returns the receiver name within the
				// process for the agent hosting the service.

    OTC_String const&   agentIdentity() const
                                { return agentIdentity_; }
				// Returns a human understandable version
				// of the receiver name which identifies
				// the agent.

    OTC_String const&   processAddress() const
                                { return processAddress_; }
                                // Returns the address prefix for the
				// process in which the service is located.

    OTC_String const&   processIdentity() const
                                { return processIdentity_; }
				// Returns a human understandable string
				// identify the process and host where the
				// service is located.

    OTC_String const&   networkGroup() const
                                { return networkGroup_; }
				// Returns the identifier for the
				// interprocess network connection knowledge
				// of the service arrived through. Only
				// relevant when the service is in a remote
				// process.

    OTC_String const&   agentAddress() const
                                { return agentAddress_; }
				// Returns the full unique address of the
				// agent hosting the service.

    OTC_String const&   serviceAddress() const
                                { return serviceAddress_; }
				// Returns the full unique address of the
				// service.

    OTC_ServiceLocation serviceLocation() const
                                { return serviceLocation_; }
                                // Returns where the service is located.
				// This will be <OTCLIB_SERVICE_LOCAL> if
				// the process address is empty and the
				// value <OTCLIB_SERVICE_REMOTE> otherwise.

    // = CLONING

    OTC_ServiceBinding*	clone();
				// Makes a clone of this object. Actually
				// just increments a reference count and
				// returns the same object, but the paradigm
				// of it returning a clone is what we want.

    // = DESTRUCTION

    void		destroy();
				// Destroys this object. Actually decrements
				// a reference count and only deletes it
				// if that reference count reaches <0>.

  protected:

			~OTC_ServiceBinding();
				// Protected so that <destroy()> method
				// must be used.

  private:

			OTC_ServiceBinding(OTC_ServiceBinding const&);
				// Do not define an implementation for this.

    OTC_ServiceBinding&	operator=(OTC_ServiceBinding const&);
				// Do not define an implementation for this.

    static OTC_NRMutex	mutex_;
				// Lock for threads, used to protect
				// reference counting code and access
				// to the group list.

    OTC_Atomic		refCount_;
				// The number of references to this
				// description.

    OTC_String          serviceName_;
                                // The service instance name.

    OTC_String          serviceAudience_;
                                // The scope of the service.

    OTC_String          receiverName_;
                                // The receiver name for agent.

    OTC_String		agentIdentity_;
    				// Human understandable version of the
				// receiver name.

    OTC_String          processAddress_;
                                // The address prefix for process agent
				// is located in.

    OTC_String		processIdentity_;
    				// Human understandable version of the
				// process ID and hostname.

    OTC_String          networkGroup_;
				// The identifier for the interprocess
				// network connection which knowledge of the
				// service came through.

    OTC_String          agentAddress_;
				// The full unique address of the agent.
				// Includes process address and receiver
				// name.

    OTC_String          serviceAddress_;
				// The full unique address of the service.
				// Includes process address, receiver name,
				// service type, service group, service name
				// and service object.

    OTC_ServiceLocation serviceLocation_;
                                // Where the service is located. Is
				// derived from whether process address
				// was empty.
};

/* ------------------------------------------------------------------------- */

OSE_TEMPLATE_SPECIALISATION
class OTC_BaseActions<OTC_ServiceBinding*>
{
  public:

    static OTC_ServiceBinding*	add(OTC_ServiceBinding* theItem)
				{ return theItem->clone(); }

    static void		remove(OTC_ServiceBinding* theItem)
				{ theItem->destroy(); }
};

OSE_TEMPLATE_SPECIALISATION
class OTC_HashActions<OTC_ServiceBinding*>
{
  public:

    static int		hash(OTC_ServiceBinding* theItem)
				{ return theItem->serviceAddress().hash(); }
};

OSE_TEMPLATE_SPECIALISATION
class OTC_RankActions<OTC_ServiceBinding*>
{
  public:

    static int		rank(
			 OTC_ServiceBinding* theItem1,
			 OTC_ServiceBinding* theItem2
			)
				{
				  return theItem1->serviceAddress().rank(
				   theItem2->serviceAddress());
				}
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_SERVBIND_HH */
