/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcsvrgstry.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/dispatch/svrgstry.hh"
#endif
#endif

#include <OTC/dispatch/svrgstry.hh>
#include <OTC/dispatch/servannc.hh>
#include <OTC/dispatch/grpannc.hh>
#include <OTC/dispatch/envelope.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/debug/tracetag.hh>
#include <OTC/thread/mxreaper.hh>

/* ------------------------------------------------------------------------- */
static OTC_NRMutex MODULE_TRACETAG_MUTEX;
static OTC_TraceTag& MODULE_TRACETAG()
{
  static OTC_TraceTag* TAG = 0;
  MODULE_TRACETAG_MUTEX.lock();
  if (TAG == 0)
    TAG = new OTC_TraceTag("OTC_SVRegistry");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}

#define MODULE MODULE_TRACETAG()

/* ------------------------------------------------------------------------- */
OTC_SVRegistry::~OTC_SVRegistry()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_SVRegistry::OTC_SVRegistry()
{
  serviceInBox_.bind(this,"$SERVICE");
  localServiceInBox_.bind(this,"$LOCAL-SERVICE");
  remoteServiceInBox_.bind(this,"$REMOTE-SERVICE");
}

/* ------------------------------------------------------------------------- */
OTC_Iterator<OTC_ServiceBinding*> OTC_SVRegistry::serviceAgents() const
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_SVRegistry::serviceAgents()");

  OTC_Iterator<OTC_ServiceBinding*> theResult;

  mutex_.readLock();

  OTC_MutexReaper<OTC_RWMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_Iterator<OTC_ServiceBinding*> theServices;
  theServices = services_.items(OTCLIB_FORWARD,OTCLIB_UNSAFE);

  if (theServices.isValid())
  {
    OTC_Deque<OTC_ServiceBinding*> theList;

    while (theServices.isValid())
    {
      theList.addLast(theServices.item());

      theServices.next();
    }

    theResult = theList.items();
  }

  theServices = 0;

  xxxMutex.release();

  mutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
OTC_Iterator<OTC_String> OTC_SVRegistry::serviceGroups(
 char const* theServiceAddress
) const
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_SVRegistry::serviceGroups(char const*)");

  OTC_Iterator<OTC_String> theResult;

  mutex_.readLock();

  OTC_MutexReaper<OTC_RWMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (theServiceAddress == 0 || *theServiceAddress == EOS)
  {
    OTC_PairIterator<OTC_String,OTC_String> theMembers;
    theMembers = mapServiceGroup_.pairs(OTCLIB_FORWARD,OTCLIB_UNSAFE);

    if (theMembers.isValid())
    {
      OTC_HSet<OTC_String> theSet;

      while (theMembers.isValid())
      {
	theSet.add(theMembers.key(),OTCLIB_REPLACE);

	theMembers.next();
      }

      theResult = theSet.items();
    }
  }
  else
  {
    OTC_Iterator<OTC_String> theGroups;
    theGroups = mapServiceGroup_.keys(theServiceAddress,
     OTCLIB_FORWARD,OTCLIB_UNSAFE);

    OTC_Deque<OTC_String> theList;

    while (theGroups.isValid())
    {
      theList.addLast(theGroups.item());

      theGroups.next();
    }

    theResult = theList.items();
  }

  xxxMutex.release();

  mutex_.unlock();

  return theResult;
}


/* ------------------------------------------------------------------------- */
OTC_Iterator<OTC_ServiceBinding*> OTC_SVRegistry::lookupServiceName(
 char const* theServiceName
) const
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_SVRegistry::lookupServiceName(char const*)");

  if (theServiceName == 0)
    theServiceName = "";

  OTCLIB_TRACER(MODULE) << "name = " << theServiceName << endl;

  OTC_Iterator<OTC_ServiceBinding*> theResult;

  mutex_.readLock();

  OTC_MutexReaper<OTC_RWMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_Iterator<OTC_ServiceBinding*> theServices;
  theServices = mapServiceName_.items(theServiceName,
   OTCLIB_FORWARD,OTCLIB_UNSAFE);

  if (theServices.isValid())
  {
    OTC_Deque<OTC_ServiceBinding*> theList;

    while (theServices.isValid())
    {
      theList.addLast(theServices.item());

      theServices.next();
    }

    theResult = theList.items();
  }

  theServices = 0;

  xxxMutex.release();

  mutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
OTC_Iterator<OTC_ServiceBinding*> OTC_SVRegistry::lookupServiceAddress(
 char const* theServiceAddress
) const
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_SVRegistry::lookupServiceAddress(char const*)");

  if (theServiceAddress == 0)
    theServiceAddress = "";

  OTCLIB_TRACER(MODULE) << "address = " << theServiceAddress << endl;

  OTC_Iterator<OTC_ServiceBinding*> theResult;

  mutex_.readLock();

  OTC_MutexReaper<OTC_RWMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_Deque<OTC_ServiceBinding*> theList;

  if (services_.contains(theServiceAddress))
    theList.addLast(services_.item(theServiceAddress));

  theResult = theList.items();

  xxxMutex.release();

  mutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
OTC_Iterator<OTC_ServiceBinding*> OTC_SVRegistry::lookupServiceGroup(
 char const* theServiceGroup
) const
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_SVRegistry::lookupServiceGroup(char const*)");

  if (theServiceGroup == 0)
    theServiceGroup = "";

  OTCLIB_TRACER(MODULE) << "group = " << theServiceGroup << endl;

  OTC_Iterator<OTC_ServiceBinding*> theResult;

  mutex_.readLock();

  OTC_MutexReaper<OTC_RWMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_Deque<OTC_ServiceBinding*> theList;

  OTC_Iterator<OTC_String> theAddresses;
  theAddresses = mapServiceGroup_.items(theServiceGroup,
   OTCLIB_FORWARD,OTCLIB_UNSAFE);

  while (theAddresses.isValid())
  {
    theList.addLast(services_.item(theAddresses.item()));

    theAddresses.next();
  }

  theAddresses = 0;

  theResult = theList.items();

  xxxMutex.release();

  mutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
void OTC_SVRegistry::subscribeRegistry(int theAgentId, int theLocation)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_SVRegistry::subscribeRegistry()");

  if (theLocation & OTCLIB_SERVICE_LOCAL)
    observersLocal_.addObserver(theAgentId);

  if (theLocation & OTCLIB_SERVICE_REMOTE)
    observersRemote_.addObserver(theAgentId);

  if ((theLocation & OTCLIB_SERVICE_LOCAL) ||
   (theLocation & OTCLIB_SERVICE_REMOTE))
  {
    mutex_.readLock();

    OTC_MutexReaper<OTC_RWMutex> xxxMutex;
    xxxMutex.grab(mutex_);

    if (!services_.isEmpty())
    {
      OTC_Iterator<OTC_ServiceBinding*> theServices;
      theServices = services_.items(OTCLIB_FORWARD,OTCLIB_UNSAFE);

      // For each service we need to send out a
      // service announcement indicating it is
      // available and then a group announcement
      // for any groups it is a member of.

      while (theServices.isValid())
      {
	OTC_ServiceBinding* theBinding;
	theBinding = theServices.item();

	// Only send the announcements if subscriber
	// is interested in services in that location.

	if (((theLocation & OTCLIB_SERVICE_LOCAL) &&
	 theBinding->serviceLocation() == OTCLIB_SERVICE_LOCAL) ||
	 ((theLocation & OTCLIB_SERVICE_REMOTE) &&
	 theBinding->serviceLocation() == OTCLIB_SERVICE_REMOTE))
	{
	  OTC_Event* theEvent;

	  // Send the service announcement.

	  theEvent = new OTCEV_ServiceAnnouncement(theBinding,
	   OTCLIB_SERVICE_AVAILABLE,OTCLIB_SERVICE_REGISTRY);
	  OTCLIB_ASSERT_M(theEvent != 0);

	  theEvent->queue(theAgentId,OTCLIB_STANDARD_JOB);

	  // Send the group announcements.

	  OTC_Iterator<OTC_String> theGroups;
	  theGroups = mapServiceGroup_.keys(
	   theBinding->serviceAddress(),
	   OTCLIB_FORWARD,OTCLIB_UNSAFE);

	  while (theGroups.isValid())
	  {
	    theEvent = new OTCEV_GroupAnnouncement(theBinding,
	     theGroups.item(),OTCLIB_SERVICE_AVAILABLE,
	     OTCLIB_SERVICE_REGISTRY);
	    OTCLIB_ASSERT_M(theEvent != 0);

	    theEvent->queue(theAgentId,OTCLIB_STANDARD_JOB);

	    theGroups.next();
	  }
	}

	theServices.next();
      }
    }

    xxxMutex.release();

    mutex_.unlock();
  }
}

/* ------------------------------------------------------------------------- */
void OTC_SVRegistry::unsubscribeRegistry(int theAgentId, int theLocation)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_SVRegistry::unsubscribeRegistry()");

  if (theLocation & OTCLIB_SERVICE_LOCAL)
    observersLocal_.removeObserver(theAgentId);

  if (theLocation & OTCLIB_SERVICE_REMOTE)
    observersRemote_.removeObserver(theAgentId);
}

/* ------------------------------------------------------------------------- */
void OTC_SVRegistry::subscribeServiceName(
 int theAgentId,
 OTC_String const& theServiceName
)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_SVRegistry::subscribeServiceName(int,OTC_String const&)");

  OTCLIB_TRACER(MODULE) << "agent = " << theAgentId << endl;
  OTCLIB_TRACER(MODULE) << "name = " << theServiceName << endl;

  mutex_.writeLock();

  OTC_MutexReaper<OTC_RWMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (!subServiceName_.contains(theServiceName,theAgentId))
    subServiceName_.add(theServiceName,theAgentId);

  OTC_Iterator<OTC_ServiceBinding*> theServices;
  theServices = mapServiceName_.items(theServiceName);

  // For each service with the specified name
  // that exists, send out a service announcement
  // indicating it is available. No group
  // announcements are sent even if service is
  // a member of a group.

  while (theServices.isValid())
  {
    OTCEV_ServiceAnnouncement* theEvent;
    theEvent = new OTCEV_ServiceAnnouncement(theServices.item(),
     OTCLIB_SERVICE_AVAILABLE,OTCLIB_SERVICE_NAME);
    OTCLIB_ASSERT_M(theEvent != 0);

    theEvent->queue(theAgentId,OTCLIB_STANDARD_JOB);

    theServices.next();
  }

  theServices = 0;

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_SVRegistry::unsubscribeServiceName(
 int theAgentId,
 OTC_String const& theServiceName
)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_SVRegistry::unsubscribeServiceName(int,OTC_String const&)");


  OTCLIB_TRACER(MODULE) << "agent = " << theAgentId << endl;
  OTCLIB_TRACER(MODULE) << "name = " << theServiceName << endl;

  mutex_.writeLock();

  OTC_MutexReaper<OTC_RWMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  subServiceName_.remove(theServiceName,theAgentId,OTCLIB_UNSAFE);

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_SVRegistry::unsubscribeServiceName(int theAgentId)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_SVRegistry::unsubscribeServiceName(int)");

  OTCLIB_TRACER(MODULE) << "agent = " << theAgentId << endl;

  mutex_.writeLock();

  OTC_MutexReaper<OTC_RWMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  subServiceName_.removeItem(theAgentId,OTCLIB_UNSAFE);

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
OTC_ServiceStatus OTC_SVRegistry::subscribeServiceAddress(
 int theAgentId,
 OTC_String const& theServiceAddress,
 OTC_String const& theSubscription
)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_SVRegistry::subscribeServiceAddress()");

  OTCLIB_TRACER(MODULE) << "agent = " << theAgentId << endl;
  OTCLIB_TRACER(MODULE) << "address = " << theServiceAddress << endl;

  OTC_ServiceStatus theResult = OTCLIB_SERVICE_WITHDRAWN;

  mutex_.writeLock();

  OTC_MutexReaper<OTC_RWMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (!mapServiceAddress_.contains(theServiceAddress,theAgentId))
    mapServiceAddress_.add(theServiceAddress,theAgentId);

  OTC_SVRegistrySubscription* theSubscribers = 0;

  if (!subServiceAddress_.contains(theServiceAddress))
  {
    theSubscribers = new OTC_SVRegistrySubscription;
    OTCLIB_ASSERT_M(theSubscribers != 0);

    subServiceAddress_.add(theServiceAddress,theSubscribers);
  }
  else
    theSubscribers = subServiceAddress_.item(theServiceAddress);

  theSubscribers->addSubscription(theAgentId,theSubscription);

  // Note that announcements are only sent if
  // a service with corresponding address
  // exists. No initial service announcement
  // indicating that a service may have been
  // withdrawn is sent.

  if (services_.contains(theServiceAddress))
  {
    theResult = OTCLIB_SERVICE_AVAILABLE;

    OTC_ServiceBinding* tmpBinding;
    tmpBinding = services_.item(theServiceAddress);

    OTC_Event* theEvent;

    // Send out a service announcement indicating
    // that a service with the corresponding
    // address is available.

    theEvent = new OTCEV_ServiceAnnouncement(
     tmpBinding,OTCLIB_SERVICE_AVAILABLE,
     OTCLIB_SERVICE_ADDRESS,theSubscription);
    OTCLIB_ASSERT_M(theEvent != 0);

    theEvent->queue(theAgentId,OTCLIB_STANDARD_JOB);

    // Send out a group announcement for each
    // group the service is a member of.

    OTC_Iterator<OTC_String> theGroups;
    theGroups = mapServiceGroup_.keys(theServiceAddress);

    while (theGroups.isValid())
    {
      theEvent = new OTCEV_GroupAnnouncement(tmpBinding,
       theGroups.item(),OTCLIB_SERVICE_AVAILABLE,
       OTCLIB_SERVICE_ADDRESS,theSubscription);
      OTCLIB_ASSERT_M(theEvent != 0);

      theEvent->queue(theAgentId,OTCLIB_STANDARD_JOB);

      theGroups.next();
    }
  }

  xxxMutex.release();

  mutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
void OTC_SVRegistry::unsubscribeServiceAddress(
 int theAgentId,
 OTC_String const& theServiceAddress,
 OTC_String const& theSubscription
)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_SVRegistry::unsubscribeServiceAddress()");

  OTCLIB_TRACER(MODULE) << "agent = " << theAgentId << endl;
  OTCLIB_TRACER(MODULE) << "address = " << theServiceAddress << endl;

  mutex_.writeLock();

  OTC_MutexReaper<OTC_RWMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (subServiceAddress_.contains(theServiceAddress))
  {
    OTC_SVRegistrySubscription* theSubscribers;
    theSubscribers = subServiceAddress_.item(theServiceAddress);

    theSubscribers->removeSubscription(theAgentId,theSubscription);

    if (theSubscribers->numSubscriptions(theAgentId) == 0)
      mapServiceAddress_.remove(theServiceAddress,theAgentId,OTCLIB_UNSAFE);

    if (theSubscribers->numSubscriptions() == 0)
      subServiceAddress_.remove(theServiceAddress);
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_SVRegistry::unsubscribeServiceAddress(int theAgentId)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_SVRegistry::unsubscribeServiceAddress(int)");

  OTCLIB_TRACER(MODULE) << "agent = " << theAgentId << endl;

  mutex_.writeLock();

  OTC_MutexReaper<OTC_RWMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_Iterator<OTC_String> theAddresses;
  theAddresses = mapServiceAddress_.keys(theAgentId);

  while (theAddresses.isValid())
  {
    OTC_SVRegistrySubscription* theSubscribers;
    theSubscribers = subServiceAddress_.item(theAddresses.item());

    theSubscribers->removeSubscription(theAgentId);

    if (theSubscribers->numSubscriptions() == 0)
      subServiceAddress_.remove(theAddresses.item());

    theAddresses.next();
  }

  theAddresses = 0;

  mapServiceAddress_.removeItem(theAgentId,OTCLIB_UNSAFE);

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_SVRegistry::subscribeServiceGroup(
 int theAgentId,
 OTC_String const& theServiceGroup
)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_SVRegistry::subscribeServiceGroup(int,char const*)");

  OTCLIB_TRACER(MODULE) << "agent = " << theAgentId << endl;
  OTCLIB_TRACER(MODULE) << "group = " << theServiceGroup << endl;

  mutex_.writeLock();

  OTC_MutexReaper<OTC_RWMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (!subServiceGroup_.contains(theServiceGroup,theAgentId))
    subServiceGroup_.add(theServiceGroup,theAgentId);

  OTC_Iterator<OTC_String> theAddresses;
  theAddresses = mapServiceGroup_.items(theServiceGroup);

  // For each service that is a member of the
  // specified group, send out a group announcement.
  // No service announcements are sent.

  while (theAddresses.isValid())
  {
    OTC_ServiceBinding* tmpBinding;
    tmpBinding = services_.item(theAddresses.item());

    OTC_Event* theEvent;

    theEvent = new OTCEV_GroupAnnouncement(tmpBinding,
     theServiceGroup,OTCLIB_SERVICE_AVAILABLE,
     OTCLIB_SERVICE_GROUP);
    OTCLIB_ASSERT_M(theEvent != 0);

    theEvent->queue(theAgentId,OTCLIB_STANDARD_JOB);

    theAddresses.next();
  }

  theAddresses = 0;

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_SVRegistry::unsubscribeServiceGroup(
 int theAgentId,
 OTC_String const& theServiceGroup
)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_SVRegistry::unsubscribeServiceGroup(int,char const*)");

  OTCLIB_TRACER(MODULE) << "agent = " << theAgentId << endl;
  OTCLIB_TRACER(MODULE) << "group = " << theServiceGroup << endl;

  mutex_.writeLock();

  OTC_MutexReaper<OTC_RWMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  subServiceGroup_.remove(theServiceGroup,theAgentId,OTCLIB_UNSAFE);

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_SVRegistry::unsubscribeServiceGroup(int theAgentId)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_SVRegistry::unsubscribeServiceGroup(int)");

  OTCLIB_TRACER(MODULE) << "agent = " << theAgentId << endl;

  mutex_.writeLock();

  OTC_MutexReaper<OTC_RWMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  subServiceGroup_.removeItem(theAgentId,OTCLIB_UNSAFE);

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_SVRegistry::unsubscribeAgent(int theAgentId)
{
  unsubscribeRegistry(theAgentId);
  unsubscribeServiceName(theAgentId);
  unsubscribeServiceAddress(theAgentId);
  unsubscribeServiceGroup(theAgentId);
}

/* ------------------------------------------------------------------------- */
OTC_ServiceBinding* OTC_SVRegistry::addServiceBinding(
 OTC_ServiceBinding* theBinding
)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_SVRegistry::addServiceBinding(...)");

  OTCLIB_TRACER(MODULE) << "name = "
   << theBinding->serviceName() << endl;
  OTCLIB_TRACER(MODULE) << "audience = "
   << theBinding->serviceAudience() << endl;
  OTCLIB_TRACER(MODULE) << "receiver = "
   << theBinding->receiverName() << endl;
  OTCLIB_TRACER(MODULE) << "process = "
   << theBinding->processAddress() << endl;
  OTCLIB_TRACER(MODULE) << "address = "
   << theBinding->serviceAddress() << endl;

  mutex_.writeLock();

  OTC_MutexReaper<OTC_RWMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTCLIB_ENSURE_FN((!services_.contains(theBinding->serviceAddress())),
   "OTC_SVRegistry::addServiceBinding(...)",
   "service address already exists");

  // Add service name and address mappings.

  services_.add(theBinding->serviceAddress(),theBinding);
  mapServiceName_.add(theBinding->serviceName(),theBinding);

  // We need to send out announcements for
  // name, address and registry subscriptions.
  // We don't send out any group announcements
  // as at this point the service cannot be a
  // member of a group.

  OTC_Event* theEvent;
  OTC_Iterator<int> theAgents;

  // Send out service announcements corresponding
  // to service registry subscriptions.

  theEvent = new OTCEV_ServiceAnnouncement(theBinding,
   OTCLIB_SERVICE_AVAILABLE,OTCLIB_SERVICE_REGISTRY);
  OTCLIB_ASSERT_M(theEvent != 0);

  if (theBinding->serviceLocation() == OTCLIB_SERVICE_LOCAL)
    observersLocal_.notifyObservers(theEvent,OTCLIB_STANDARD_JOB);
  else
    observersRemote_.notifyObservers(theEvent,OTCLIB_STANDARD_JOB);

  // Send out service announcements corresponding
  // to service address subscriptions.

#if 0
  theAgents = mapServiceAddress_.items(theBinding->serviceAddress());

  if (theAgents.isValid())
  {
    theEvent = new OTCEV_ServiceAnnouncement(theBinding,
     OTCLIB_SERVICE_AVAILABLE,OTCLIB_SERVICE_ADDRESS);
    OTCLIB_ASSERT_M(theEvent != 0);

    theEvent->queue(theAgents,OTCLIB_STANDARD_JOB);
  }
#endif

  if (subServiceAddress_.contains(theBinding->serviceAddress()))
  {
    OTC_SVRegistrySubscription* theSubscribers;
    theSubscribers = subServiceAddress_.item(theBinding->serviceAddress());

    OTC_PairIterator<int,OTC_String> theSubscriptions;
    theSubscriptions = theSubscribers->subscriptions();

    while (theSubscriptions.isValid())
    {
      theEvent = new OTCEV_ServiceAnnouncement(theBinding,
       OTCLIB_SERVICE_AVAILABLE,OTCLIB_SERVICE_ADDRESS,
       theSubscriptions.item());
      OTCLIB_ASSERT_M(theEvent != 0);

      theEvent->queue(theSubscriptions.key(),OTCLIB_STANDARD_JOB);

      theSubscriptions.next();
    }
  }

  // Send out service announcements corresponding
  // to service name subscriptions.

  theAgents = subServiceName_.items(theBinding->serviceName());

  if (theAgents.isValid())
  {
    theEvent = new OTCEV_ServiceAnnouncement(theBinding,
     OTCLIB_SERVICE_AVAILABLE,OTCLIB_SERVICE_NAME);
    OTCLIB_ASSERT_M(theEvent != 0);

    theEvent->queue(theAgents,OTCLIB_STANDARD_JOB);
  }

  theAgents = 0;

  xxxMutex.release();

  mutex_.unlock();

  return theBinding;
}

/* ------------------------------------------------------------------------- */
OTC_ServiceBinding* OTC_SVRegistry::addServiceBinding(
 char const* theServiceName,
 char const* theServiceAudience,
 char const* theReceiverName,
 char const* theProcessAddress,
 char const* theNetworkGroup
)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_SVRegistry::addServiceBinding(...)");

  OTC_ServiceBinding* theBinding;
  theBinding = new OTC_ServiceBinding(theServiceName,
   theServiceAudience,theReceiverName,theProcessAddress,theNetworkGroup);
  OTCLIB_ASSERT_M(theBinding != 0);

  addServiceBinding(theBinding);

  // Okay to destroy this here, as it is kept in
  // the various collections. Caller can call
  // clone() if it wants to keep it around after
  // service has been removed from the registry.

  theBinding->destroy();

  return theBinding;
}

/* ------------------------------------------------------------------------- */
void OTC_SVRegistry::removeServiceBinding(char const* theServiceAddress)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_SVRegistry::removeServiceBinding(char const*)");

  if (theServiceAddress == 0)
    theServiceAddress = "";

  OTCLIB_TRACER(MODULE) << "address = " << theServiceAddress << endl;

  mutex_.writeLock();

  OTC_MutexReaper<OTC_RWMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  // Have to send out service announcements
  // indicating withdrawal of the service.
  // Where appropriate need to first send out
  // group announcements where service was a
  // member of a group.

  OTC_Event* theEvent;
  OTC_Iterator<int> theAgents;

  if (services_.contains(theServiceAddress))
  {
    OTC_ServiceBinding* tmpBinding;
    tmpBinding = services_.item(theServiceAddress);

    // If service is a member of any group it
    // has to be removed from the groups and
    // group announcements sent out.

    OTC_String tmpServiceAddress(tmpBinding->serviceAddress());

    if (mapServiceGroup_.containsItem(tmpServiceAddress))
    {
      OTC_Iterator<OTC_String> theGroups;
      theGroups = mapServiceGroup_.keys(tmpServiceAddress);

      // For each group service is a member of,
      // send out group announcements indicating
      // it has withdrawn from that group.

      while (theGroups.isValid())
      {
	// Send out group announcements to those
	// with service group subscription.

	theAgents = subServiceGroup_.items(theGroups.item());

	if (theAgents.isValid())
	{
	  theEvent = new OTCEV_GroupAnnouncement(tmpBinding,
	   theGroups.item(),OTCLIB_SERVICE_WITHDRAWN,
	   OTCLIB_SERVICE_GROUP);
	  OTCLIB_ASSERT_M(theEvent != 0);

	  theEvent->queue(theAgents,OTCLIB_STANDARD_JOB);
	}

	// Send out group announcements to those
	// with service address subscription.

#if 0
	theAgents = mapServiceAddress_.items(tmpServiceAddress);

	if (theAgents.isValid())
	{
	  theEvent = new OTCEV_GroupAnnouncement(tmpBinding,
	   theGroups.item(),OTCLIB_SERVICE_WITHDRAWN,
	   OTCLIB_SERVICE_ADDRESS);
	  OTCLIB_ASSERT_M(theEvent != 0);

	  theEvent->queue(theAgents,OTCLIB_STANDARD_JOB);
        }
#endif

	if (subServiceAddress_.contains(tmpServiceAddress))
	{
	  OTC_SVRegistrySubscription* theSubscribers;
	  theSubscribers = subServiceAddress_.item(tmpServiceAddress);

	  OTC_PairIterator<int,OTC_String> theSubscriptions;
	  theSubscriptions = theSubscribers->subscriptions();

	  while (theSubscriptions.isValid())
	  {
	    theEvent = new OTCEV_GroupAnnouncement(tmpBinding,
	     theGroups.item(),OTCLIB_SERVICE_WITHDRAWN,
	     OTCLIB_SERVICE_ADDRESS,theSubscriptions.item());
	    OTCLIB_ASSERT_M(theEvent != 0);

	    theEvent->queue(theSubscriptions.key(),OTCLIB_STANDARD_JOB);

	    theSubscriptions.next();
	  }
	}

	// Send out group announcements to those
	// with service registry subscription.

	theEvent = new OTCEV_GroupAnnouncement(tmpBinding,
	 theGroups.item(),OTCLIB_SERVICE_WITHDRAWN,
	 OTCLIB_SERVICE_REGISTRY);
	OTCLIB_ASSERT_M(theEvent != 0);

	if (tmpBinding->serviceLocation() == OTCLIB_SERVICE_LOCAL)
	  observersLocal_.notifyObservers(theEvent,OTCLIB_STANDARD_JOB);
	else
	  observersRemote_.notifyObservers(theEvent,OTCLIB_STANDARD_JOB);

	theGroups.next();
      }

      // Remove service group memberships.

      mapServiceGroup_.removeItem(tmpServiceAddress);
    }

    // Send out service announcements to
    // those with service name subscriptions.

    theAgents = subServiceName_.items(tmpBinding->serviceName());

    if (theAgents.isValid())
    {
      theEvent = new OTCEV_ServiceAnnouncement(tmpBinding,
       OTCLIB_SERVICE_WITHDRAWN,OTCLIB_SERVICE_NAME);
      OTCLIB_ASSERT_M(theEvent != 0);

      theEvent->queue(theAgents,OTCLIB_STANDARD_JOB);
    }

    // Send out service announcements to
    // those with service address subscriptions.

#if 0
    theAgents = mapServiceAddress_.items(tmpBinding->serviceAddress());

    if (theAgents.isValid())
    {
      theEvent = new OTCEV_ServiceAnnouncement(tmpBinding,
       OTCLIB_SERVICE_WITHDRAWN,OTCLIB_SERVICE_ADDRESS);
      OTCLIB_ASSERT_M(theEvent != 0);

      theEvent->queue(theAgents,OTCLIB_STANDARD_JOB);
    }
#endif

    if (subServiceAddress_.contains(tmpBinding->serviceAddress()))
    {
      OTC_SVRegistrySubscription* theSubscribers;
      theSubscribers = subServiceAddress_.item(tmpBinding->serviceAddress());

      OTC_PairIterator<int,OTC_String> theSubscriptions;
      theSubscriptions = theSubscribers->subscriptions();

      while (theSubscriptions.isValid())
      {
	theEvent = new OTCEV_ServiceAnnouncement(tmpBinding,
	 OTCLIB_SERVICE_WITHDRAWN,OTCLIB_SERVICE_ADDRESS,
	 theSubscriptions.item());
	OTCLIB_ASSERT_M(theEvent != 0);

	theEvent->queue(theSubscriptions.key(),OTCLIB_STANDARD_JOB);

	theSubscriptions.next();
      }
    }

    // Send out service announcements to
    // those with service registry subscriptions.

    theEvent = new OTCEV_ServiceAnnouncement(tmpBinding,
     OTCLIB_SERVICE_WITHDRAWN,OTCLIB_SERVICE_REGISTRY);
    OTCLIB_ASSERT_M(theEvent != 0);

    if (tmpBinding->serviceLocation() == OTCLIB_SERVICE_LOCAL)
      observersLocal_.notifyObservers(theEvent,OTCLIB_STANDARD_JOB);
    else
      observersRemote_.notifyObservers(theEvent,OTCLIB_STANDARD_JOB);

    // Remove service name and address mappings.

    mapServiceName_.remove(tmpBinding->serviceName(),tmpBinding);
    services_.remove(tmpBinding->serviceAddress());
  }

  theAgents = 0;

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_SVRegistry::addToServiceGroup(
 char const* theServiceGroup,
 char const* theServiceAddress
)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_SVRegistry::addToServiceGroup(char const*,char const*)");

  OTCLIB_ENSURE_FN((theServiceAddress != 0),
   "OTC_SVRegistry::addToServiceGroup(char const*,char const*)",
   "invalid service address");

  if (theServiceGroup == 0 || *theServiceGroup == EOS)
    return;

  OTC_String tmpServiceGroup(theServiceGroup);
  OTC_String tmpServiceAddress(theServiceAddress);

  OTCLIB_TRACER(MODULE) << "group = " << tmpServiceGroup << endl;
  OTCLIB_TRACER(MODULE) << "address = " << tmpServiceAddress << endl;

  mutex_.writeLock();

  OTC_MutexReaper<OTC_RWMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (!mapServiceGroup_.contains(tmpServiceGroup,tmpServiceAddress))
  {
    // Add service group mapping.

    mapServiceGroup_.add(tmpServiceGroup,tmpServiceAddress);

    OTC_Event* theEvent;

    OTC_ServiceBinding* tmpBinding;
    tmpBinding = services_.item(tmpServiceAddress);

    OTC_Iterator<int> theAgents;

    // Send group announcements to those with
    // service registry subscriptions.

    theEvent = new OTCEV_GroupAnnouncement(tmpBinding,
     tmpServiceGroup,OTCLIB_SERVICE_AVAILABLE,
     OTCLIB_SERVICE_REGISTRY);
    OTCLIB_ASSERT_M(theEvent != 0);

    if (tmpBinding->serviceLocation() == OTCLIB_SERVICE_LOCAL)
      observersLocal_.notifyObservers(theEvent,OTCLIB_STANDARD_JOB);
    else
      observersRemote_.notifyObservers(theEvent,OTCLIB_STANDARD_JOB);

    // Send group announcements to those with
    // service address subscription.

#if 0
    theAgents = mapServiceAddress_.items(tmpServiceAddress);

    if (theAgents.isValid())
    {
      theEvent = new OTCEV_GroupAnnouncement(tmpBinding,
       tmpServiceGroup,OTCLIB_SERVICE_AVAILABLE,
       OTCLIB_SERVICE_ADDRESS);
      OTCLIB_ASSERT_M(theEvent != 0);

      theEvent->queue(theAgents,OTCLIB_STANDARD_JOB);
    }
#endif

    if (subServiceAddress_.contains(tmpServiceAddress))
    {
      OTC_SVRegistrySubscription* theSubscribers;
      theSubscribers = subServiceAddress_.item(tmpServiceAddress);

      OTC_PairIterator<int,OTC_String> theSubscriptions;
      theSubscriptions = theSubscribers->subscriptions();

      while (theSubscriptions.isValid())
      {
	theEvent = new OTCEV_GroupAnnouncement(tmpBinding,
	 tmpServiceGroup,OTCLIB_SERVICE_AVAILABLE,
	 OTCLIB_SERVICE_ADDRESS,theSubscriptions.item());
	OTCLIB_ASSERT_M(theEvent != 0);

	theEvent->queue(theSubscriptions.key(),OTCLIB_STANDARD_JOB);

	theSubscriptions.next();
      }
    }

    // Send group announcements to those with
    // service group subscriptions.

    theAgents = subServiceGroup_.items(tmpServiceGroup);

    if (theAgents.isValid())
    {
      theEvent = new OTCEV_GroupAnnouncement(tmpBinding,
       tmpServiceGroup,OTCLIB_SERVICE_AVAILABLE,
       OTCLIB_SERVICE_GROUP);
      OTCLIB_ASSERT_M(theEvent != 0);

      theEvent->queue(theAgents,OTCLIB_STANDARD_JOB);
    }
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_SVRegistry::removeFromServiceGroup(
 char const* theServiceGroup,
 char const* theServiceAddress
)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_SVRegistry::removeFromServiceGroup(char const*,char const*)");

  OTCLIB_ENSURE_FN((theServiceAddress != 0),
   "OTC_SVRegistry::removeFromServiceGroup(char const*,char const*)",
   "invalid service address");

  OTC_String tmpServiceGroup(theServiceGroup);
  OTC_String tmpServiceAddress(theServiceAddress);

  OTCLIB_TRACER(MODULE) << "group = " << tmpServiceGroup << endl;
  OTCLIB_TRACER(MODULE) << "address = " << tmpServiceAddress << endl;

  mutex_.writeLock();

  OTC_MutexReaper<OTC_RWMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (mapServiceGroup_.contains(tmpServiceGroup,tmpServiceAddress))
  {
    OTC_Event* theEvent;

    OTC_ServiceBinding* tmpBinding;
    tmpBinding = services_.item(tmpServiceAddress);

    OTC_Iterator<int> theAgents;

    // Send group announcements to those with
    // service group subscriptions.

    theAgents = subServiceGroup_.items(tmpServiceGroup);

    if (theAgents.isValid())
    {
      theEvent = new OTCEV_GroupAnnouncement(tmpBinding,
       tmpServiceGroup,OTCLIB_SERVICE_WITHDRAWN,
       OTCLIB_SERVICE_GROUP);
      OTCLIB_ASSERT_M(theEvent != 0);

      theEvent->queue(theAgents,OTCLIB_STANDARD_JOB);
    }

    // Send group announcements to those with
    // service address subscriptions.

#if 0
    theAgents = mapServiceAddress_.items(tmpServiceAddress);

    if (theAgents.isValid())
    {
      theEvent = new OTCEV_GroupAnnouncement(tmpBinding,
       tmpServiceGroup,OTCLIB_SERVICE_WITHDRAWN,
       OTCLIB_SERVICE_ADDRESS);
      OTCLIB_ASSERT_M(theEvent != 0);

      theEvent->queue(theAgents,OTCLIB_STANDARD_JOB);
    }
#endif

    if (subServiceAddress_.contains(tmpServiceAddress))
    {
      OTC_SVRegistrySubscription* theSubscribers;
      theSubscribers = subServiceAddress_.item(tmpServiceAddress);

      OTC_PairIterator<int,OTC_String> theSubscriptions;
      theSubscriptions = theSubscribers->subscriptions();

      while (theSubscriptions.isValid())
      {
	theEvent = new OTCEV_GroupAnnouncement(tmpBinding,
	 tmpServiceGroup,OTCLIB_SERVICE_WITHDRAWN,
	 OTCLIB_SERVICE_ADDRESS,theSubscriptions.item());
	OTCLIB_ASSERT_M(theEvent != 0);

	theEvent->queue(theSubscriptions.key(),OTCLIB_STANDARD_JOB);

	theSubscriptions.next();
      }
    }

    // Send group announcements to those with
    // service registry subscriptions.

    theEvent = new OTCEV_GroupAnnouncement(tmpBinding,
     tmpServiceGroup,OTCLIB_SERVICE_WITHDRAWN,
     OTCLIB_SERVICE_REGISTRY);
    OTCLIB_ASSERT_M(theEvent != 0);

    if (tmpBinding->serviceLocation() == OTCLIB_SERVICE_LOCAL)
      observersLocal_.notifyObservers(theEvent,OTCLIB_STANDARD_JOB);
    else
      observersRemote_.notifyObservers(theEvent,OTCLIB_STANDARD_JOB);

    // Remove service group mapping.

    mapServiceGroup_.remove(tmpServiceGroup,tmpServiceAddress);
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_SVRegistry::handle(OTC_Event* theEvent)
{
  if (theEvent == 0)
    return;

  if (theEvent->type() == OTCEV_Envelope::typeId())
  {
    OTCEV_Envelope* theEnvelope;
    theEnvelope = (OTCEV_Envelope*)theEvent;

    if (theEnvelope->currentReceiver() == "$SERVICE" ||
     theEnvelope->currentReceiver() == "$LOCAL-SERVICE" ||
     theEnvelope->currentReceiver() == "$REMOTE-SERVICE")
    {
      mutex_.writeLock();

      OTC_MutexReaper<OTC_RWMutex> xxxMutex;
      xxxMutex.grab(mutex_);

      OTC_Iterator<OTC_ServiceBinding*> theServices;
      theServices = mapServiceName_.items(theEnvelope->nextReceiver());

      while (theServices.isValid())
      {
	OTC_ServiceBinding* theBinding;
	theBinding = theServices.item();

	if (theEnvelope->currentReceiver() == "$SERVICE" ||
	 (theEnvelope->currentReceiver() == "$LOCAL-SERVICE" &&
	 theBinding->serviceLocation() == OTCLIB_SERVICE_LOCAL) ||
	 (theEnvelope->currentReceiver() == "$REMOTE-SERVICE" &&
	 theBinding->serviceLocation() == OTCLIB_SERVICE_REMOTE))
	{
	  OTCEV_Envelope* tmpEnvelope;
	  tmpEnvelope = new OTCEV_Envelope(theEnvelope,
	   theBinding->serviceAddress(),theEnvelope->from());
	  OTCLIB_ASSERT_M(tmpEnvelope);

	  tmpEnvelope->deliver();
	}

	theServices.next();
      }

      xxxMutex.release();

      mutex_.unlock();
    }
  }

  theEvent->destroy();
}

/* ------------------------------------------------------------------------- */
