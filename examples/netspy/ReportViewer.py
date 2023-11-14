import Tkinter
import Pmw
import netsvc
import string

from ReportService import ReportService

class ReportViewer(Pmw.MegaWidget):

  def __init__(self,parent=None,**kw):
    optiondefs = (
      ("service",None,Pmw.INITOPT),
      ("identity",None,Pmw.INITOPT),
      ("subject","*",Pmw.INITOPT)
    )

    self.defineoptions(kw,optiondefs)

    Pmw.MegaWidget.__init__(self,parent)

    self._service = ""
    self._subject = self["subject"]

    self._subjects = {}

    self._logState = "stopped"
    self._eventLog = ""

    self._version = 0

    f1 = Tkinter.Frame(self.interior())
    f1.pack(side="top",fill="both",expand="true")

    p3 = Pmw.PanedWidget(f1,orient="vertical",
        hull_width=400,hull_height=550)
    p3.pack(side="top",padx=4,pady=4,fill="both",expand="true")

    p3.add("top",size=300,min=200)
    p3.add("bottom",min=150)

    f3a = Tkinter.Frame(p3.pane("top"))
    f3a.pack(side="top",fill="both",expand="true")

    f3b = Tkinter.Frame(f3a)
    f3b.pack(side="top",fill="both",expand="true")

    f3c = Tkinter.Frame(f3b)
    f3c.pack(side="top")

    f3 = Tkinter.Frame(f3c)
    f3.pack(side="left",fill="both",expand="true")

    self._serviceField = self.createcomponent("group",(),None,
        Pmw.EntryField,(f3,),labelpos="w",entry_width=25,
	label_text="Service:",validate=None)
    self._serviceField.pack(side="top",padx=8,pady=8)

    self._identityField = self.createcomponent("identity",(),None,
        Pmw.EntryField,(f3,),labelpos="w",entry_width=25,
        label_text="Identity:",validate=None)
    self._identityField.pack(side="top",padx=8,pady=8)

    self._subjectField = self.createcomponent("service",(),None,
        Pmw.EntryField,(f3,),labelpos="w",entry_width=25,
        label_text="Subject:",validate=None,value="*")
    self._subjectField.pack(side="top",padx=8,pady=8)

    Pmw.alignlabels((self._serviceField,self._subjectField))

    b2 = Pmw.ButtonBox(f3c,orient="horizontal")
    b2.pack(side="left",padx=4,pady=4)
    b2.add("Subscribe",command=self._subscribeService)

    self._subjectView = Pmw.ScrolledListBox(f3b,listbox_height=15)
    self._subjectView.pack(side="top",padx=4,pady=8,fill="both",expand="true")

    f4 = Tkinter.Frame(p3.pane("bottom"))
    f4.pack(side="top",fill="both",expand="true")

    self._eventView = Pmw.ScrolledText(f4,text_padx=4,
        text_pady=4,text_wrap="char",text_state="disabled",
	text_height=5,borderframe=1)
    self._eventView.yview("moveto",1.0)
    self._eventView.pack(side="top",padx=4,pady=8,fill="both",expand="true")

    self._eventButtons = Pmw.ButtonBox(f4,orient="horizontal")
    self._eventButtons.pack(side="bottom",padx=8,pady=8)
    self._eventButtons.add("Start",width=7,command=self._startLog)
    self._eventButtons.add("Suspend",width=7,state="disabled",
        command=self._suspendLog)
    self._eventButtons.add("Clear",width=7,command=self._clearLog)

    self.bind("<Destroy>", self._widgetDestroyed)

    if self["service"] is not None:
      self._startLog()

    self._monitor = None

    if self["service"] is not None:
      self._serviceField.setvalue(self["service"])
      if self["identity"] is not None:
	self._identityField.setvalue(self["identity"])
      self._subjectField.setvalue(self["subject"])
      self._subscribeService()
      self._eventView.yview("moveto",1.0)

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

  def _subscribeService(self):
    if self._monitor is not None:
      job = netsvc.Job(self._monitor.destroyAgent)
      netsvc.Dispatcher().schedule(job,netsvc.STANDARD_JOB)
      self._monitor = None

    if self._service != "":
      self._subjectView.clear()
      self._subjects = {}

      message = []
      message.append("TIMESTAMP = %s" % `str(netsvc.DateTime())`)
      message.append("EVENT     = 'UNSUBSCRIBE'")
      message.append("SERVICE   = %s" % `self._service`)
      if self._identity != "":
	message.append("IDENTITY  = %s" % `self._identity`)
      message.append("SUBJECT   = %s" % `self._subject`)
      message.append("")
      message.append("")
      message = string.joinfields(message,"\n")
      self._appendLog(message)

      self._service = self._serviceField.get()
      self._identity = self._identityField.get()
      self._subject = self._subjectField.get()

      if self._service != "":
	message = []
	message.append("TIMESTAMP = %s" % `str(netsvc.DateTime())`)
	message.append("EVENT     = 'SUBSCRIBE'")
	message.append("SERVICE   = %s" % `self._service`)
	if self._identity != "":
	  message.append("IDENTITY  = %s" % `self._identity`)
	message.append("SUBJECT   = %s" % `self._subject`)
	message.append("")
	message.append("")
	message = string.joinfields(message,"\n")
	self._appendLog(message)

	self._monitor = ReportService(self._service,
	    self._identity,self._subject)
	self._monitor.scheduleStartup()
	self._version = 0
    else:
      self._service = self._serviceField.get()
      self._identity = self._identityField.get()
      self._subject = self._subjectField.get()
      if self._service != "":
	message = []
	message.append("TIMESTAMP = %s" % `str(netsvc.DateTime())`)
	message.append("EVENT     = 'SUBSCRIBE'")
	message.append("SERVICE   = %s" % `self._service`)
	if self._identity != "":
	  message.append("IDENTITY  = %s" % `self._identity`)
	message.append("SUBJECT   = %s" % `self._subject`)
	message.append("")
	message.append("")
	message = string.joinfields(message,"\n")
	self._appendLog(message)

	self._monitor = ReportService(self._service,
	    self._identity,self._subject)
	self._monitor.scheduleStartup()
	self._version = 0

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
      if event["EVENT"] == "REPORT":
	subject = event["SUBJECT"]
	content = str(repr(event["CONTENT"]))
	self._subjects[subject] = content

        message = []
	message.append("TIMESTAMP = %s" % `str(event["TIMESTAMP"])`)
	message.append("EVENT     = %s" % `event["EVENT"]`)
	message.append("SERVICE   = %s" % `event["SERVICE"]`)
	message.append("AGENT     = %s" % `event["AGENT"]`)
	message.append("SUBJECT   = %s" % `event["SUBJECT"]`)
	message.append("CONTENT   = %s" % str(repr(event["CONTENT"])))
	message.append("")
	message.append("")
	message = string.joinfields(message,"\n")
	self._appendLog(message)

      elif event["EVENT"] == "ANNOUNCEMENT":
        message = []
	message.append("TIMESTAMP = %s" % `str(event["TIMESTAMP"])`)
	message.append("EVENT     = %s" % `event["EVENT"]`)
	message.append("SERVICE   = %s" % `event["SERVICE"]`)
	message.append("AGENT     = %s" % `event["AGENT"]`)
	message.append("STATUS    = %s" % `event["STATUS"]`)
	message.append("")
	message.append("")
	message = string.joinfields(message,"\n")
	self._appendLog(message)

    self._monitor.resumeEvents()

    subjects = self._subjects.keys()
    subjects.sort()

    view = []
    for subject in subjects:
      view.append(subject + " = " + self._subjects[subject])
    self._subjectView.setlist(view)

    self.update_idletasks()

  def _widgetDestroyed(self,*args):    
    if self._timer:
      self.after_cancel(self._timer)
      self._timer = None
    if self._monitor is not None:
      job = netsvc.Job(self._monitor.destroyAgent)
      netsvc.Dispatcher().schedule(job,netsvc.STANDARD_JOB)
      self._monitor = None

Pmw.forwardmethods(ReportViewer,Pmw.ScrolledListBox,"_subjectView")
