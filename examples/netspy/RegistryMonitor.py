import netsvc

class RegistryMonitor(netsvc.Monitor):

  count = 0

  def __init__(self):
    RegistryMonitor.count = RegistryMonitor.count + 1
    netsvc.Monitor.__init__(self)
    self._events = []
    self._version = 0
    self.subscribeStartup(self.activateMonitor)

  def activateMonitor(self):
    self.subscribeRegistry(self._processAnnouncements)

  def snapshotVersion(self):
    return self._version

  def queuedEvents(self):
    result = self._events
    self._events = []
    return result

  def _processAnnouncements(self,binding,group,status):
    details = {}

    details["TIMESTAMP"] = netsvc.DateTime()
    details["BINDING"] = binding
    details["GROUP"] = group

    if status == netsvc.SERVICE_AVAILABLE:
      details["STATUS"] = "AVAILABLE"
    else:
      details["STATUS"] = "WITHDRAWN"

    self._events.append(details)

    self._version = self._version + 1
