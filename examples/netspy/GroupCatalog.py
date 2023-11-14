import Tkinter
import Pmw
import netsvc

from ServiceMonitor import ServiceMonitor

class GroupCatalog(Pmw.MegaWidget):

  def __init__(self,parent=None,**kw):
    optiondefs = (
    )

    self.defineoptions(kw,optiondefs)

    Pmw.MegaWidget.__init__(self,parent)

    self._version = 0

    self._groupsView = self.createcomponent("catalog",(),None,
        Pmw.ScrolledListBox,(self.interior(),))
    self._groupsView.pack(side="top",fill="both",expand="true")

    self.bind("<Destroy>", self._widgetDestroyed)

    self._monitor = ServiceMonitor()
    self._monitor.scheduleStartup()

    self._timer = self.after(100,self._wakeup)

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

  def _updateView(self):
    self._monitor.suspendEvents()
    groups = self._monitor.registeredGroups()
    self._monitor.resumeEvents()
    groups.sort()
    self._groupsView.setlist(groups)

    self.update_idletasks()

Pmw.forwardmethods(GroupCatalog,Pmw.ScrolledListBox,"_groupsView")
