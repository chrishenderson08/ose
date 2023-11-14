#! /usr/bin/env python

import Tkinter
import Pmw
import netsvc

from ReportViewer import ReportViewer

class MainWindow:

  def __init__(self,root):
    self._root = root
    self._root.title("NETSPY-REPORTS")

    g1 = Pmw.Group(self._root,tag_text="Reports")
    g1.pack(side="top",padx=8,pady=8,fill="both",expand="true")

    self._reportView = ReportViewer(g1.interior())
    self._reportView.pack(side="top",padx=8,pady=8,fill="both",expand="true")


import ProgramStartup
ProgramStartup.run(MainWindow)
