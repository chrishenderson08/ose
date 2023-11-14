import Pmw

from AgentBrowser import AgentBrowser

class AgentWindow(Pmw.MegaToplevel):

  def __init__(self,**kw):
    optiondefs = (
      ("servicename","",Pmw.INITOPT),
    )

    self.defineoptions(kw,optiondefs)

    Pmw.MegaToplevel.__init__(self)
    self.withdraw()

    self.title("NETSPY:SERVICE - " + self["servicename"])

    g1 = Pmw.Group(self.interior(),tag_text="Service")
    g1.pack(side="top",padx=8,pady=8,fill="both",expand="true")

    self._agentView = AgentBrowser(g1.interior(),
        servicename=self["servicename"])
    self._agentView.pack(side="top",padx=8,pady=8,fill="both",expand="true")

    self.update()
    self.deiconify()
    self.update_idletasks()

