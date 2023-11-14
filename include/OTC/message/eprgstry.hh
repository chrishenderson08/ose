#ifndef OTC_MESSAGE_EPRGSTRY_HH
#define OTC_MESSAGE_EPRGSTRY_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/eprgstry.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1997-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/servbind.hh>
#include <OTC/message/endpoint.hh>
#include <OTC/collctn/baseactn.hh>
#include <OTC/collctn/hmap.hh>
#include <OTC/collctn/tdualmap.hh>
#include <OTC/refcnt/resource.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/message/eprgstry.hh"
#endif
#endif

#ifdef _MSC_VER
#pragma warning(disable: 4251)
#endif

/* ------------------------------------------------------------------------- */

enum OTC_ExchangeType
{
  OTCLIB_EXCHANGE_CLIENT,
  OTCLIB_EXCHANGE_SERVER,
  OTCLIB_EXCHANGE_SHUTDOWN,
  OTCLIB_EXCHANGE_UNKNOWN
};

class OSE_EXPORT OTC_EPRegistry : public OTC_Resource
    // = TITLE
    //     Tracks endpoints available for use by an exchange.
    //
    // = DESCRIPTION
    //     The <OTC_EPRegistry> keeps track of endpoints available for use by
    //     an instance of <OTC_Exchange>. Information maintained includes
    //     last known state, type of exchange at other end of connection,
    //     remote process name and a summary of registration information for
    //     services in remote process for the corresponding exchange group.
{
  public:

			~OTC_EPRegistry();
				// Decrements reference count.

    // = INITIALISATION

			OTC_EPRegistry(
			 OTC_EndPoint* theEndPoint,
			 char const* theExchangeGroup
			);
				// Increments reference count on
				// <theEndPoint>. Sets the exchange type
				// to the value <OTCLIB_EXCHANGE_UNKNOWN>.
				// <theExchangeGroup> should be the group of
				// the exchange.

    // = QUERY

    OTC_EndPoint*	endPoint()
				{ return endPoint_; }
				// Returns the endpoint object.

    OTC_String const&	exchangeName() const
				{ return exchangeName_; }
				// Returns the full name of the exchange
				// at the remote end. Will return empty
				// if the type of the remote exchange is
				// not known.

    OTC_String const&	exchangeGroup() const
				{ return exchangeGroup_; }
				// Returns the name of the group the remote
				// exchange is in. If this registry is for
				// a client endpoint, this should be valid
				// from the point of creation of the class.
				// For a server endpoint, it will only have
				// a valid value when contact has been made
				// with the remote exchange.

    OTC_ExchangeType	exchangeType() const
				{ return exchangeType_; }
				// Returns the type of the exchange at
				// the remote end of the connection, or
				// unknown or shutdown as appropriate.

    OTC_String const&	remoteProcess() const
      				{ return remoteProcess_; }
				// Returns the name given for the remote
				// process.

    OTC_String const&	localAddress() const
      				{ return endPoint_->localAddress(); }
    				// Returns the local address of the endpoint.

    OTC_String const&	remoteAddress() const
      				{ return remoteAddress_; }
				// Returns a cached version of the remote
				// address for the endpoint. This will be
				// valid even after the connection has been
				// lost.

    // = EXCHANGE

    void		setup(
			 OTC_ExchangeType theType,
			 OTC_String const& theRemoteProcess
			);
				// Sets the exchange type for the remote end
				// to <theType> and initialises registrations
				// ready for new service agent information.
				// <theRemoteProcess> is the name provided by
				// the remote end describing the process.
				// Called when contact with the remote
				// exchange has been established.

    void		reset();
				// Resets remote exchange type to being
				// unknown and cleans up any registration
				// information which is no longer required.
				// Ie., this is called when a connection
				// is lost. Exception is that if the
				// endpoint had been explicitly shutdown
				// in which case the exchange type will
				// be left as being shutdown.

    // = SHUTDOWN

    void		shutdown();
				// Stops any active connection and marks that
				// this whole registry entry can be removed
				// thus dropping the endpoint from the
				// exchange. If the connection wasn't active,
				// the caller should also actually remove the
				// registry after calling this function.
				// Before calling this function, the exchange
				// type can be checked to determine if there
				// is an active connection. That is, if the
				// exchange type is unknown, registry entry
				// can be removed after having called this
				// function. If there is an active connection
				// call this function and the exchange will
				// drop it after it has cleaned up after the
				// connection.

    // = SERVICES

    bool		containsService(OTC_String const& theServiceAddress)
				{
				  return services_.contains(
				   theServiceAddress);
				}
				// Returns <true> if service with
				// <theServiceAddress> is accessable via this
				// endpoint.

    OTC_ServiceBinding*	service(OTC_String const& theServiceAddress) const
      				{ return services_.item(theServiceAddress); }
    				// Returns the service binding for the
				// service with <theServiceAddress> which
				// is accessable via this endpoint.

    void		addService(OTC_ServiceBinding* theBinding);
				// Adds the service described by
				// <theBinding> to the set of services
				// accessable via this endpoint. If a service
				// with the same agent address already
				// exists, it will be replaced.

    void		removeService(OTC_String const& theServiceAddress);
				// Removes service with <theServiceAddress>
				// from set of services accesable via this
				// endpoint. If no such service is known
				// of, no action is taken.

    void		removeAllServices()
				{
				  serviceGroups_.removeAll();
				  services_.removeAll();
				}
				// Removes all information about services
				// accessable via this endpoint.

    OTC_Iterator<OTC_ServiceBinding*>	services(
				 OTC_Direction theDirection=OTCLIB_FORWARD
				) const
				{ return services_.items(theDirection); }
				// Returns an iterator over services known
				// to be accessable via this endpoint.
				// By default returns services in order
				// they were added. Supply <OTCLIB_BACKWARD>
				// as argument for <theDirection> if you
				// want to visit them in reverse order.

    // = SERVICE GROUPS

    bool		isServiceGroupMember(
			 OTC_String const& theServiceGroup,
			 OTC_String const& theServiceAddress
			)
				{
				  return serviceGroups_.contains(
				   theServiceGroup,theServiceAddress);
				}
				// Returns <true> if the service with
				// <theServiceAddress> is in <theServiceGroup>.

    void		addToServiceGroup(
			 OTC_String const& theServiceGroup,
			 OTC_String const& theServiceAddress
			)
      				{
				  serviceGroups_.add(theServiceGroup,
				   theServiceAddress);
				}
				// Adds the service group mapping described
				// by <theServiceAddress> to <theServiceGroup>.

    void		removeFromServiceGroup(
			 OTC_String const& theServiceGroup,
			 OTC_String const& theServiceAddress
			)
      				{
				  serviceGroups_.remove(theServiceGroup,
				   theServiceAddress);
				}
				// Removes the service group mapping
				// described by <theServiceAddress> from
				// <theServiceGroup>.

    OTC_Iterator<OTC_String>	serviceGroupsByAddress(
				 OTC_String const& theServiceAddress
				) const
				{
				  return serviceGroups_.keys(
				   theServiceAddress);
				}
				// Returns an iterator over service groups
				// which <theServiceAddress> is in.

  private:

			OTC_EPRegistry(OTC_EPRegistry const&);
				// Do not define an implementation for this.

    OTC_EPRegistry&	operator=(OTC_EPRegistry const&);
				// Do not define an implementation for this.

    OTC_EndPoint*	endPoint_;
				// The endpoint object. A reference count
				// is held against the object so it we
				// can still use it even if connection
				// has died.

    OTC_String		exchangeName_;
				// The full name of the exchange at the
				// remote end of the connection.

    OTC_String		exchangeGroup_;
				// The group in which the remote exchange
				// is in.

    OTC_ExchangeType	exchangeType_;
				// The type of the exchange at the other
				// end of the connection.

    OTC_String		remoteProcess_;
    				// The name of the remote process.

    OTC_String		remoteAddress_;
    				// The cached remote address.

    OTC_HMap<OTC_String,OTC_ServiceBinding*>	services_;
				// Index onto services accessed via this
				// endpoint.

    OTC_TDualMap<OTC_String,OTC_String>	serviceGroups_;
    				// Mapping between service group names and
				// service addresses for services accessed
				// via this endpoint which are in a service
				// group.
};

/* ------------------------------------------------------------------------- */

OSE_TEMPLATE_SPECIALISATION
class OTC_BaseActions<OTC_EPRegistry*>
{
  public:

    static OTC_EPRegistry*      add(OTC_EPRegistry* theObject)
                                { theObject->reference(); return theObject; }

    static void         remove(OTC_EPRegistry* theObject)
                                { theObject->unReference(); }
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MESSAGE_EPRGSTRY_HH */
