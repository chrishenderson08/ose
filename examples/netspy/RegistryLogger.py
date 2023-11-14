import Tkinter
import Pmw
import netsvc
import string

from RegistryMonitor import RegistryMonitor

class RegistryLogger(Pmw.MegaWidget):

  def __init__(self,parent=None,**kw):
    optiondefs = (
      ("group",None,Pmw.INITOPT),
      ("service",None,Pmw.INITOPT),
    )

    self.defineoptions(kw,optiondefs)

    Pmw.MegaWidget.__init__(self,parent)

    self._logState = "stopped"
    self._eventLog = ""

    self._version = 0

    f4 = Tkinter.Frame(self.interior())
    f4.pack(side="top",fill="both",expand="true")

    self._eventView = self.createcomponent("events",(),None,
        Pmw.ScrolledText,(f4,),text_pady=4,text_wrap="char",
	text_state="disabled",borderframe=1)
    self._eventView.yview("moveto",1.0)
    self._eventView.pack(side="top",padx=4,pady=8,fill="both",expand="true")

    self._eventButtons = Pmw.ButtonBox(f4,orient="horizontal")
    self._eventButtons.pack(side="bottom",padx=8,pady=8)
    self._eventButtons.add("Start",width=7,command=self._startLog)
    self._eventButtons.add("Suspend",width=7,state="disabled",
        command=self._suspendLog)
    self._eventButtons.add("Clear",width=7,command=self._clearLog)

    self._startLog()

    self.bind("<Destroy>", self._widgetDestroyed)

    self._monitor = RegistryMonitor()
    self._monitor.scheduleStartup()

    self._timer = self.after(100,self._wakeup)

  def _wakeup(self):
    if self._monitor is not None:
      self._monitor.suspendEvents()
      version = self._monitor.snapshotVersion()
      self._monitor.resumeEvents()
      if version != self._version:
	self._version = version
	self._updateView()

    self._timer = self.after(100,self._wakeup)

  def _clearLog(self):
    self._eventView.configure(text_state="normal")
    self._eventView.clear()
    self._eventView.configure(text_state="disabled")
    self._eventView.yview("moveto",1.0)

  def _appendLog(self,message):
    if self._logState == "running":
      p1,p2 = self._eventView.yview()
      self._eventView.configure(text_state="normal")
      self._eventView.insert("end",message)
      self._eventView.configure(text_state="disabled")
      if (p1 == 0.0 and p2 == 0.0) or p2 == 1.0:
	self._eventView.yview("moveto",1.0)
    elif self._logState == "paused":
      self._eventLog = self._eventLog + message

  def _startLog(self):
    if self._logState == "stopped":
      self._logState = "running"
      self._eventButtons.button(0).configure(text="Stop")
      self._eventButtons.button(1).configure(state="normal")
    elif self._logState == "running":
      self._logState = "stopped"
      self._eventButtons.button(0).configure(text="Start")
      self._eventButtons.button(1).configure(state="disabled")
    elif self._logState == "paused":
      self._logState = "stopped"
      self._eventButtons.button(0).configure(text="Start")
      self._eventButtons.button(1).configure(text="Suspend")
      self._eventButtons.button(1).configure(state="disabled")

  def _suspendLog(self):
    if self._logState == "running":
      self._logState = "paused"
      self._eventButtons.button(1).configure(text="Resume")
    elif self._logState == "paused":
      self._logState = "running"
      self._eventButtons.button(1).configure(text="Suspend")
      if self._eventLog != "":
	p1,p2 = self._eventView.yview()
	self._eventView.configure(text_state="normal")
	self._eventView.insert("end",self._eventLog)
	self._eventView.configure(text_state="disabled")
	if (p1 == 0.0 and p2 == 0.0) or p2 == 1.0:
	  self._eventView.yview("moveto",1.0)
	self._eventLog = ""

  def _updateView(self):
    self._monitor.suspendEvents()

    for event in self._monitor.queuedEvents():
      if (self["group"] is None and self["service"] is None) \
          or (self["service"] is None and self["group"] == event["GROUP"]) \
	  or (self["group"] is None and self["service"] == event["BINDING"].serviceName()) \
	  or (self["group"] == event["GROUP"] and self["service"] == \
	  event["BINDING"].serviceName()):
	message = []
	message.append("TIMESTAMP = %s" % `str(event["TIMESTAMP"])`)
	message.append("SERVICE   = %s" % `event["BINDING"].serviceName()`)
	message.append("IDENTITY  = %s" % `event["BINDING"].agentIdentity()`)
	if event["GROUP"] is not None:
	  message.append("GROUP     = %s" % `event["GROUP"]`)
	message.append("STATUS    = %s" % `event["STATUS"]`)
	message.append("")
	message.append("")
	message = string.joinfields(message,"\n")
	self._appendLog(message)

    self._monitor.resumeEvents()

    self.update_idletasks()

  def _widgetDestroyed(self,*args):    
    if self._timer:
      self.after_cancel(self._timer)
      self._timer = None
    if self._monitor is not None:
      job = netsvc.Job(self._monitor.destroyAgent)
      netsvc.Dispatcher().schedule(job,netsvc.STANDARD_JOB)
      self._monitor = None

Pmw.forwardmethods(RegistryLogger,Pmw.ScrolledListBox,"_eventView")
