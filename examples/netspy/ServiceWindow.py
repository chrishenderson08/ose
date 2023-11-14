import Pmw

from ServiceCatalog import ServiceCatalog
#from AgentWindow import AgentWindow

class ServiceWindow(Pmw.MegaToplevel):

  def __init__(self,**kw):
    optiondefs = (
      ("namefilter","*",Pmw.INITOPT),
      ("groupfilter","*",Pmw.INITOPT), 
    )

    self.defineoptions(kw,optiondefs)

    Pmw.MegaToplevel.__init__(self)
    self.withdraw()

    self.title("NETSPY:GROUP - " + self["groupfilter"])

    g1 = Pmw.Group(self.interior(),tag_text="Services")
    g1.pack(side="top",padx=8,pady=8,fill="both",expand="true")

    self._serviceView = ServiceCatalog(g1.interior(),
	group_entry_state="disabled",
	namefilter=self["namefilter"],groupfilter=self["groupfilter"],
	catalog_dblclickcommand=self._browseService)
    self._serviceView.pack(side="top",padx=8,pady=8,fill="both",expand="true")

    self.update()
    self.deiconify()
    self.update_idletasks()

  def _browseService(self):
    services = self._serviceView.getcurselection()
    if len(services) != 0:
      #child = AgentWindow(servicename=services[0])
      pass

