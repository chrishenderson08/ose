#! /usr/bin/env python

import Tkinter
import Pmw
import netsvc

from AgentCatalog import AgentCatalog
from ServiceCatalog import ServiceCatalog
from GroupCatalog import GroupCatalog
from RegistryLogger import RegistryLogger
from ReportViewer import ReportViewer


class ReportWindow(Pmw.MegaToplevel):

  def __init__(self,**kw):
    optiondefs = (
      ("service",None,Pmw.INITOPT),
      ("identity",None,Pmw.INITOPT),
      ("subject","*",Pmw.INITOPT)
    )

    self.defineoptions(kw,optiondefs)

    Pmw.MegaToplevel.__init__(self)
    self.withdraw()

    self.title("NETSPY:REPORTS")

    g1 = Pmw.Group(self.interior(),tag_text="Service")
    g1.pack(side="top",padx=8,pady=8,fill="both",expand="true")

    self._reportView = ReportViewer(g1.interior(),service=self["service"],
     identity=self["identity"],subject=self["subject"])
    self._reportView.pack(side="top",padx=8,pady=8,fill="both",expand="true")

    self.update()
    self.deiconify()
    self.update_idletasks()


class RegistryWindow(Pmw.MegaToplevel):

  def __init__(self,**kw):
    optiondefs = (
      ("group",None,Pmw.INITOPT),
      ("service",None,Pmw.INITOPT),
    )

    self.defineoptions(kw,optiondefs)

    Pmw.MegaToplevel.__init__(self)
    self.withdraw()

    title = "NETSPY:REGISTRY"
    if self["group"] is not None:
      title = title + " - GROUP(%s)" % `self["group"]`
    if self["service"] is not None:
      title = title + " - SERVICE(%s)" % `self["service"]`
    self.title(title)

    g1 = Pmw.Group(self.interior(),tag_text="Events")
    g1.pack(side="top",padx=8,pady=8,fill="both",expand="true")

    self._agentView = RegistryLogger(g1.interior(),
        events_text_height=40,events_text_width=50,
	group=self["group"],service=self["service"])
    self._agentView.pack(side="top",padx=8,pady=8,fill="both",expand="true")

    self.update()
    self.deiconify()
    self.update_idletasks()


class MainWindow:

  def __init__(self,root):
    self._root = root
    self._root.title("NETSPY")

    p1 = Pmw.PanedWidget(self._root,orient="horizontal",
        hull_width=800,hull_height=650)
    p1.pack(side="top",padx=8,pady=8,fill="both",expand="true")

    p1.add("left",size=400)
    p1.add("right")

    p2 = Pmw.PanedWidget(p1.pane("left"),orient="vertical")
    p2.pack(side="top",padx=4,pady=4,fill="both",expand="true")

    p2.add("top",size=200,min=100)
    p2.add("bottom",min=200)

    g1 = Pmw.Group(p2.pane("top"),tag_text="Groups")
    g1.pack(side="top",padx=4,pady=4,fill="both",expand="true")

    self._groupView = GroupCatalog(g1.interior(),
     catalog_dblclickcommand=self._filterGroup)
    self._groupView.pack(side="top",padx=8,pady=8,fill="both",expand="true")

    g2 = Pmw.Group(p2.pane("bottom"),tag_text="Services")
    g2.pack(side="top",padx=4,pady=4,fill="both",expand="true")

    self._serviceView = ServiceCatalog(g2.interior(),
     catalog_dblclickcommand=self._browseService)
    self._serviceView.pack(side="top",padx=8,pady=8,fill="both",expand="true")

    # Naughty, as delving into implementation.
    self._serviceView._buttonBox.button(self._serviceView._buttonBox.index(
        "Filter")).configure(command=self._applyFilters)
    self._serviceView._buttonBox.button(self._serviceView._buttonBox.index(
        "Reset")).configure(command=self._resetFilters)

    """
    p3 = Pmw.PanedWidget(p1.pane("right"),orient="vertical")
    p3.pack(side="top",padx=4,pady=4,fill="both",expand="true")

    p3.add("agents",size=250,min=150)
    p3.add("inline-events",min=200)
    #p3.add("events",min=100,max=100)
    """

    #g3 = Pmw.Group(p3.pane("agents"),tag_text="Agents")
    #g3.pack(side="top",padx=4,pady=4,fill="both",expand="true")
    g3 = Pmw.Group(p1.pane("right"),tag_text="Agents")
    g3.pack(side="top",padx=8,pady=8,fill="both",expand="true")

    #self._agentView = AgentCatalog(g3.interior(),catalog_listbox_height=30,
    # catalog_dblclickcommand=self._browseAgent)
    self._agentView = AgentCatalog(g3.interior(),
     catalog_dblclickcommand=self._browseAgent)
    self._agentView.pack(side="top",padx=8,pady=8,fill="both",expand="true")

    """
    g4 = Pmw.Group(p3.pane("inline-events"),tag_text="Events")
    g4.pack(side="top",padx=4,pady=4,fill="both",expand="true")

    self._registryView = RegistryLogger(g4.interior(),events_text_height=5)
    self._registryView.pack(side="top",padx=4,pady=4,fill="both",expand="true")
    """

    g7 = Pmw.Group(p1.pane("right"),tag_text="Request")
    g7.pack(side="top",padx=8,pady=8,fill="both")

    f7 = Tkinter.Frame(g7.interior())
    f7.pack(side="top")

    f7a = Tkinter.Frame(f7)
    f7a.pack(side="left",padx=4,pady=4)

    self._requestServiceField = Pmw.EntryField(f7a,labelpos="w",
        entry_width=25,label_text="Service:",validate=None,value="")
    self._requestServiceField.pack(side="top",padx=4,pady=4)

    self._requestIdentityField = Pmw.EntryField(f7a,labelpos="w",
        entry_width=25,label_text="Identity:",validate=None,value="")
    self._requestIdentityField.pack(side="top",padx=4,pady=4)

    Pmw.alignlabels((self._requestServiceField,self._requestIdentityField))

    b7 = Pmw.ButtonBox(f7,orient="horizontal")
    b7.pack(side="left",padx=8,pady=8)
    b7.add("Generate",command=self._requestService)

    g5 = Pmw.Group(p1.pane("right"),tag_text="Reports")
    g5.pack(side="top",padx=8,pady=8,fill="both")

    f1 = Tkinter.Frame(g5.interior())
    f1.pack(side="top")

    f1a = Tkinter.Frame(f1)
    f1a.pack(side="left",padx=4,pady=4)

    self._serviceField = Pmw.EntryField(f1a,labelpos="w",
        entry_width=25,label_text="Service:",validate=None,value="")
    self._serviceField.pack(side="top",padx=4,pady=4)

    self._identityField = Pmw.EntryField(f1a,labelpos="w",
        entry_width=25,label_text="Identity:",validate=None,value="")
    self._identityField.pack(side="top",padx=4,pady=4)

    self._subjectField = Pmw.EntryField(f1a,labelpos="w",
        entry_width=25,label_text="Subject:",validate=None,value="*")
    self._subjectField.pack(side="top",padx=4,pady=4)

    Pmw.alignlabels((self._serviceField,self._identityField,self._subjectField))

    b1 = Pmw.ButtonBox(f1,orient="horizontal")
    b1.pack(side="left",padx=8,pady=8)
    b1.add("Subscribe",command=self._subscribeService)

    g6 = Pmw.Group(p1.pane("right"),tag_text="Events")
    g6.pack(side="top",padx=8,pady=8,fill="both")

    f2 = Tkinter.Frame(g6.interior())
    f2.pack(side="left")

    self._groupField = Pmw.EntryField(f2,labelpos="w",
        entry_width=25,label_text="Group:",validate=None,value="")
    self._groupField.pack(side="top",padx=8,pady=8)

    self._registryServiceField = Pmw.EntryField(f2,labelpos="w",
        entry_width=25,label_text="Service:",validate=None,value="")
    self._registryServiceField.pack(side="top",padx=8,pady=8)

    Pmw.alignlabels((self._groupField,self._registryServiceField))

    b1 = Pmw.ButtonBox(g6.interior(),orient="vertical")
    b1.pack(side="left",padx=8,pady=8)
    b1.add("Monitor",command=self._monitorRegistry)

  def _browseService(self):
    services = self._serviceView.getcurselection()
    if len(services) != 0:
      self._agentView.defineService(services[0])
      self._serviceField.setvalue(services[0])
      self._identityField.setvalue("")
      self._requestServiceField.setvalue(services[0])
      self._requestIdentityField.setvalue("")
      self._registryServiceField.setvalue(services[0])
      if not '*' in self._serviceView._groupField.get():
        self._groupField.setvalue(self._serviceView._groupField.get())
      else:
        self._groupField.setvalue("")

  def _browseAgent(self):
    agents = self._agentView.getcurselection()
    if len(agents) != 0:
      self._serviceField.setvalue(self._agentView._service)
      self._identityField.setvalue(agents[0])
      self._requestServiceField.setvalue(self._agentView._service)
      self._requestIdentityField.setvalue(agents[0])

  def _filterGroup(self):
    groups = self._groupView.getcurselection()
    if len(groups) != 0:
      self._serviceView.defineGroup(groups[0])
      self._groupField.setvalue(groups[0])
      self._registryServiceField.setvalue("")

  def _subscribeService(self):
    service = self._serviceField.get()
    if service == "": service = None
    identity = self._identityField.get()
    if identity == "": identity = None
    subject = self._subjectField.get()
    child = ReportWindow(service=service,identity=identity,subject=subject)

  def _requestService(self):
    service = self._serviceField.get()
    if service == "": service = None
    identity = self._identityField.get()
    if identity == "": identity = None
    #child = ReportWindow(service=service,identity=identity,subject=subject)

  def _monitorRegistry(self):
    group = self._groupField.get()
    if group == "":
      group = None
    service = self._registryServiceField.get()
    if service == "":
      service = None
    child = RegistryWindow(group=group,service=service)

  def _applyFilters(self):
    if not '*' in self._serviceView._groupField.get():
      self._groupField.setvalue(self._serviceView._groupField.get())
      self._registryServiceField.setvalue("")
    else:
      self._groupField.setvalue("")
      self._registryServiceField.setvalue("")
    self._serviceView._applyFilters()

  def _resetFilters(self):
    self._groupField.setvalue("")
    self._registryServiceField.setvalue("")
    self._serviceView._resetFilters()


import ProgramStartup
ProgramStartup.run(MainWindow)
