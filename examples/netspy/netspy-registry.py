#! /usr/bin/env python

import Tkinter
import Pmw
import netsvc

from RegistryLogger import RegistryLogger

class MainWindow:

  def __init__(self,root):
    self._root = root
    self._root.title("NETSPY-REGISTRY")

    g1 = Pmw.Group(self._root,tag_text="Events")
    g1.pack(side="top",padx=8,pady=8,fill="both",expand="true")

    self._reportView = RegistryLogger(g1.interior(),
        events_text_height=40,events_text_width=50)
    self._reportView.pack(side="top",padx=8,pady=8,fill="both",expand="true")


import ProgramStartup
ProgramStartup.run(MainWindow)
