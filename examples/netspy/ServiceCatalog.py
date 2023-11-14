import Tkinter
import Pmw
import fnmatch
import netsvc

from ServiceMonitor import ServiceMonitor

class ServiceCatalog(Pmw.MegaWidget):

  def __init__(self,parent=None,**kw):
    optiondefs = (
      ("namefilter","*",Pmw.INITOPT),
      ("groupfilter","*",Pmw.INITOPT),
    )

    self.defineoptions(kw,optiondefs)

    Pmw.MegaWidget.__init__(self,parent)

    self._version = 0

    self._nameFilter = self["namefilter"]
    self._groupFilter = self["groupfilter"]

    f1 = Tkinter.Frame(self.interior())
    f1.pack(side="top")

    f2 = Tkinter.Frame(f1)
    f2.pack(side="left",padx=4,pady=4)

    self._groupField = self.createcomponent("group",(),None,
        Pmw.EntryField,(f2,),labelpos="w",entry_width=25,
	label_text="Group:",validate=None,value=self["groupfilter"],
	command=self._applyFilters)
    self._groupField.pack(side="top",padx=4,pady=4)

    self._serviceField = self.createcomponent("service",(),None,
        Pmw.EntryField,(f2,),labelpos="w",entry_width=25,
        label_text="Name:",validate=None,value=self["namefilter"],
        command=self._applyFilters)
    self._serviceField.pack(side="top",padx=4,pady=4)

    Pmw.alignlabels((self._groupField,self._serviceField))

    self._buttonBox = self.createcomponent("buttons",(),None,
        Pmw.ButtonBox,(f1,),orient="vertical")
    self._buttonBox.pack(side="left",padx=8,pady=8)
    self._buttonBox.add("Filter",command=self._applyFilters)   
    self._buttonBox.add("Reset",command=self._resetFilters)

    self._servicesView = self.createcomponent("catalog",(),None,
        Pmw.ScrolledListBox,(self.interior(),))
    self._servicesView.pack(side="top",fill="both",expand="true")

    self.bind("<Destroy>", self._widgetDestroyed)

    self._monitor = ServiceMonitor()
    self._monitor.scheduleStartup()

    self._timer = self.after(100,self._wakeup)

  def defineGroup(self,name):
    self._groupField.setvalue(name)
    self._groupField.invoke()

  def _wakeup(self):
    self._monitor.suspendEvents()
    version = self._monitor.snapshotVersion()
    self._monitor.resumeEvents()
    if version != self._version:
      self._version = version
      self._updateView()
    self._timer = self.after(100,self._wakeup)

  def _widgetDestroyed(self,*args):    
    if self._timer:
      self.after_cancel(self._timer)
      self._timer = None
    if self._monitor is not None:
      job = netsvc.Job(self._monitor.destroyAgent)
      netsvc.Dispatcher().schedule(job,netsvc.STANDARD_JOB)
      self._monitor = None

  def _applyFilters(self):
    self._nameFilter = self._serviceField.get()
    self._groupFilter = self._groupField.get()              
    self._updateView()     

  def _resetFilters(self):
    self._serviceField.setentry("*")
    self._groupField.setentry("*")
    self._applyFilters()

  def _updateView(self):
    if self._groupFilter == "*":      
      services = []
      self._monitor.suspendEvents()
      for service in self._monitor.registeredServices().keys():
        if fnmatch.fnmatch(service,self._nameFilter):      
          services.append(service)  
      self._monitor.resumeEvents()
      services.sort()
      self._servicesView.setlist(services)
    else:
      services = []
      self._monitor.suspendEvents()
      for group in self._monitor.registeredGroups():
        if fnmatch.fnmatch(group,self._groupFilter):
          for service in self._monitor.registeredServices(group).keys():
            if fnmatch.fnmatch(service,self._nameFilter):
              services.append(service)
      self._monitor.resumeEvents()
      services.sort()
      self._servicesView.setlist(services)

    self.update_idletasks()

Pmw.forwardmethods(ServiceCatalog,Pmw.ScrolledListBox,"_servicesView")
