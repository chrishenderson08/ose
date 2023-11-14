import netsvc

class ServiceMonitor(netsvc.Monitor):

  def __init__(self):
    netsvc.Monitor.__init__(self)
    self._services = {}
    self._groups = {}
    self._version = 0
    self.subscribeStartup(self.activateMonitor)

  def activateMonitor(self):
    self.subscribeRegistry(self._processAnnouncements)

  def snapshotVersion(self):
    return self._version

  def registeredServices(self,group="*"):
    if group == "*":
      return self._services
    elif self._groups.has_key(group):
      return self._groups[group]
    else:
      return {}

  def registeredGroups(self):
    return self._groups.keys()

  def _processAnnouncements(self,binding,group,status):
    service = binding.serviceName()

    update = 0
    if group == None:
      if status == netsvc.SERVICE_AVAILABLE:
        if self._services.has_key(service):
	  self._services[service] = self._services[service] + 1
	else:
	  self._services[service] = 1
	  update = 1
      else:
        if self._services.has_key(service):
	  count = self._services[service] - 1
	  if count == 0:
	    del self._services[service]
	    update = 1
	  else:
	    self._services[service] = count
    else:
      if status == netsvc.SERVICE_AVAILABLE:
        if self._groups.has_key(group):
	  services = self._groups[group]
	  if services.has_key(service):
	    self._groups[group][service] = services[service] + 1
	  else:
	    self._groups[group][service] = 1
	    update = 1
	else:
	  self._groups[group] = { service: 1 }
	  update = 1
      else:
        if self._groups.has_key(group):
	  services = self._groups[group]
	  if services.has_key(service):
	    count = services[service] - 1
	    if count == 0:
	      del self._groups[group][service]
	      if len(self._groups[group].keys()) == 0:
	        del self._groups[group]
	      update = 1
	    else:
	      self._groups[group][service] = count

    if update != 0:
      self._version = self._version + 1
