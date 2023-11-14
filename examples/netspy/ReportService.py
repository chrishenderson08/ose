import netsvc

class ReportService(netsvc.Service):

  count = 0

  def __init__(self,service,identity,subject="*"):
    #name = "netspy@%s" % netsvc.processIdentity()
    ReportService.count = ReportService.count + 1
    name = "netspy@%s/%d" % (netsvc.processIdentity(),ReportService.count)
    #name = "netspy (%s)" % netsvc.uniqueId(netsvc.ID_SHORT_FORMAT)
    netsvc.Service.__init__(self,name)

    self._service = service
    self._identity = identity
    self._subject = subject

    self._events = []

    self._version = 0

    self.joinGroup("netspy")

    self.subscribeStartup(self.activateService)

  def activateService(self):
    if self._identity != "":
      agents = self.lookupServiceName(self._service)
      for agent in agents:
        if agent.agentIdentity() == self._identity:
	  self.subscribeReports(agent,self._subject)
    else:
      self.subscribeReports(self._service,self._subject)

  def snapshotVersion(self):
    return self._version

  def queuedEvents(self):
    result = self._events
    self._events = []
    return result

  def handlePublisherNotification(self,notification):
    details = {}

    details["TIMESTAMP"] = netsvc.DateTime()
    details["EVENT"] = "ANNOUNCEMENT"
    details["SERVICE"] = notification.publisher().serviceName()
    details["AGENT"] = notification.publisher().agentIdentity()

    if notification.status() == netsvc.SERVICE_AVAILABLE:
      details["STATUS"] = "AVAILABLE"
    else:
      details["STATUS"] = "WITHDRAWN"

    self._events.append(details)

    self._version = self._version + 1

  def handleReport(self,report):
    details = {}

    details["TIMESTAMP"] = netsvc.DateTime()
    details["EVENT"] = "REPORT"
    details["SERVICE"] = report.publisher().serviceName()
    details["AGENT"] = report.publisher().agentIdentity()
    details["SUBJECT"] = report.subjectName()
    details["CONTENT"] = report.content()

    self._events.append(details)

    self._version = self._version + 1
