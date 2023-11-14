import Tkinter
import Pmw
import netsvc

from AgentMonitor import AgentMonitor

class AgentCatalog(Pmw.MegaWidget):

  def __init__(self,parent=None,**kw):
    optiondefs = (
    )

    self.defineoptions(kw,optiondefs)

    Pmw.MegaWidget.__init__(self,parent)

    self._service = ""

    self._version = 0

    f1a = Tkinter.Frame(self.interior())
    f1a.pack(side="top",fill="both",expand="true")

    f1 = Tkinter.Frame(f1a)
    f1.pack(side="top")

    self._serviceField = self.createcomponent("service",(),None,
        Pmw.EntryField,(f1,),labelpos="w",entry_width=25,
	label_text="Service:",validate=None,value="",
	command=self._subscribeService)
    self._serviceField.pack(side="left",padx=8,pady=8)

    b1 = Pmw.ButtonBox(f1,orient="horizontal")
    b1.pack(side="left",padx=8,pady=8)
    b1.add("Monitor",command=self._subscribeService)

    self._agentsView = self.createcomponent("catalog",(),None,
        Pmw.ScrolledListBox,(self.interior(),))
    self._agentsView.pack(side="top",fill="both",expand="true")

    self.bind("<Destroy>", self._widgetDestroyed)

    self._monitor = None

    self._timer = self.after(100,self._wakeup)

  def defineService(self,name):
    self._serviceField.setvalue(name)
    self._serviceField.invoke()

  def _wakeup(self):
    if self._monitor is not None:
      self._monitor.suspendEvents()
      version = self._monitor.snapshotVersion()
      self._monitor.resumeEvents()
      if version != self._version:
	self._version = version
	self._updateView()

    self._timer = self.after(100,self._wakeup)

  def _subscribeService(self):
    if self._monitor is not None:
      job = netsvc.Job(self._monitor.destroyAgent)
      netsvc.Dispatcher().schedule(job,netsvc.STANDARD_JOB)
      self._monitor = None

    self._service = self._serviceField.get()

    self._monitor = AgentMonitor(self._service)
    self._monitor.scheduleStartup()
    self._version = 0

  def _widgetDestroyed(self,*args):    
    if self._timer:
      self.after_cancel(self._timer)
      self._timer = None
    if self._monitor is not None:
      job = netsvc.Job(self._monitor.destroyAgent)
      netsvc.Dispatcher().schedule(job,netsvc.STANDARD_JOB)
      self._monitor = None

  def _updateView(self):
    self._monitor.suspendEvents()
    agents = list(self._monitor.registeredAgents().keys())
    self._monitor.resumeEvents()
    agents.sort()
    self._agentsView.setlist(agents)

    self.update_idletasks()

Pmw.forwardmethods(AgentCatalog,Pmw.ScrolledListBox,"_agentsView")
