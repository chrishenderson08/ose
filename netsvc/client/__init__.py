# COPYRIGHT 2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

import netsvc
import Queue
import types


class _RegistryMonitor(netsvc.Monitor):

  def __init__(self,queue,service,group,filter,timeout):
    netsvc.Monitor.__init__(self)
    self._queue = queue
    self._service = service
    self._group = group
    self._filter = filter
    self._timeout = timeout
    self.subscribeStartup(self.activateMonitor)

  def destroyAgent(self):
    netsvc.Monitor.destroyAgent(self)
    self._queue = None

  def activateMonitor(self):
    if self._service is not None and self._group is None:
      self.subscribeServiceName(self.serviceAnnouncement,self._service)
    elif self._group is not None:
      self.subscribeServiceGroup(self.serviceAnnouncement,self._group)
    else:
      self.subscribeRegistry(self.serviceAnnouncement)
    self.startTimer(self.timeoutExpired,self._timeout)

  def serviceAnnouncement(self,binding,group,status):
    if status == netsvc.SERVICE_AVAILABLE:
      if self._group == group:
	if self._service is not None:
	  if self._service == binding.serviceName():
	    if self._filter is not None:
	      if self._filter(binding):
		self._queue.put(binding)
	    else:
	      self._queue.put(binding)
	else:
	  if self._filter is not None:
	    if self._filter(binding):
	      self._queue.put(binding)
	  else:
	    self._queue.put(binding)

  def timeoutExpired(self):
    self._queue.put(None)


class Registry:

  def __init__(self,timeout=10):
    self._timeout = timeout

  def locateService(self,service=None,group=None,filter=None):
    if service == None and group == None and filter == None:
      return []

    registry = netsvc.Registry()

    bindings = []

    if service is not None and group == None:
      bindings = registry.lookupServiceName(service)
      if filter != None:
        filtered = []
	for binding in bindings:
	  if filter(binding):
	    filtered.append(binding)
	bindings = filtered
      if bindings != []:
	return bindings
      if self._timeout == 0:
	return []
    elif group is not None:
      bindings = registry.lookupServiceName(group)
      filtered = []
      for binding in bindings:
	if service is not None:
	  if binding.serviceName() == service:
	    if filter != None:
	      if filter(binding):
		filtered.append(binding)
	else:
	  if filter != None:
	    if filter(binding):
	      filtered.append(binding)
      bindings = filtered

    if group is not None or service is not None:
      if bindings != []:
	return bindings
      if self._timeout == 0:
	return []

    queue = Queue.Queue(0)
    monitor = _RegistryMonitor(queue,service,group,filter,self._timeout)
    monitor.scheduleStartup()
    result = queue.get()
    if result is None:
      return []
    bindings = [result]
    monitor.suspendEvents()
    monitor.destroyAgent()
    monitor.resumeEvents()
    return bindings


class _RequestService(netsvc.Service):

  def __init__(self,queue,binding,methodName,args,timeout):
    netsvc.Service.__init__(self,"","",netsvc.SERVICE_HIDDEN)
    self._queue = queue
    self._binding = binding
    self._methodName = methodName
    self._args = args
    self._timeout = timeout
    self.subscribeStartup(self.activateService)

  def destroyAgent(self):
    netsvc.Service.destroyAgent(self)
    self._queue = None

  def activateService(self):
    id = self.sendRequest(self._binding,self._methodName,*self._args)
    self.processResponse(self.resultReceived,id)
    self.processFailure(self.failureOccurred,id,self._timeout)

  def resultReceived(self,id,result):
    self._queue.put(result)

  def failureOccurred(self,id,error,description,origin,details):
    exception = netsvc.ServiceFailure(error,description,origin,details)
    self._queue.put(exception)


class _ServiceMethod:

  def __init__(self,binding,methodName,timeout):
    self._binding = binding
    self._methodName = methodName
    self._timeout = timeout
    
  def __getattr__(self,methodName):
    return _ServiceMethod(self._binding,
        "%s.%s"%(self._methodName,methodName),self._timeout)
    
  def __call__(self,*args):
    queue = Queue.Queue(0)
    service = _RequestService(queue,self._binding,
        self._methodName,args,self._timeout)
    service.scheduleStartup()
    result = queue.get()
    service.suspendEvents()
    service.destroyAgent()
    service.resumeEvents()
    if type(result) == types.InstanceType and \
        result.__class__ is netsvc.ServiceFailure:
      raise result
    return result


class RemoteService:

  def __init__(self,binding,timeout=30):
    self._binding = binding
    self._timeout = timeout

  def __getattr__(self,methodName):
    return _ServiceMethod(self._binding,methodName,self._timeout)
