# COPYRIGHT 2001-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
#
# This program also requires:
#
#   Python Mega Widgets library (http://pmw.sourceforge.net)
#

import time
import types
import Tkinter
import Pmw
import sys
import string

import netrpc.xmlrpc
RemoteService = netrpc.xmlrpc.RemoteService

RemoteService._debug = 1

class RemoteServiceView:

  def __init__(self,root=None,**kw):

    if root == None:
      root = Pmw.MegaToplevel()

    self._root = root
    self._root.withdraw()

    server = "http://localhost:8000"
    method = "echo"

    if kw.has_key("title"):
      self._root.title(kw["title"])

    if kw.has_key("server"):
      server = kw["server"]
    if kw.has_key("method"):
      method = kw["method"]

    f1 = Tkinter.Frame(self._root)
    f1.pack(side="top",padx=4,pady=4)

    f2 = Tkinter.Frame(f1)
    f2.pack(side="left",padx=8,pady=8)

    self._serverField = Pmw.EntryField(f2,labelpos='w',entry_width=50,
        label_text="URL:",validate=None,value=server)
    self._serverField.pack(side="top",padx=4,pady=4)

    self._methodField = Pmw.EntryField(f2,labelpos='w',entry_width=50,
        label_text="Method:",validate=None,value=method)
    self._methodField.pack(side="top",padx=4,pady=4)

    Pmw.alignlabels((self._serverField,self._methodField))

    b1 = Pmw.ButtonBox(f1,orient="vertical")
    b1.pack(side="right")
    b1.add("Execute",command=self._executeMethod)

    self._methodParams = Pmw.ScrolledText(self._root,labelpos='nw',
        label_text="Params:",text_padx=4,text_pady=4,text_wrap="char",
	text_height=8,borderframe=1,vscrollmode="static")
    self._methodParams.insert("end","()")
    self._methodParams.pack(side="top",padx=8,pady=8,fill="both",
        expand="true")

    self._methodResult = Pmw.ScrolledText(self._root,labelpos='nw',
        label_text="Result:",text_padx=4,text_pady=4,text_wrap="char",
	text_height=16,borderframe=1,vscrollmode="static")
    self._methodResult.pack(side="top",padx=8,pady=8,fill="both",
        expand="true")

    self._methodResult.tag_configure("HEADING",underline=1)

    self._root.update()
    self._root.deiconify()
    self._root.update_idletasks()

  def _executeMethod(self):
    try:
      params = string.strip(self._methodParams.get())
      if params == "":
        params = ()
      else:
        locals = {}
	locals["time"] = time
	locals["Boolean"] = netrpc.Boolean
	locals["False"] = netrpc.False
	locals["True"] = netrpc.True
	locals["DateTime"] = netrpc.DateTime
	#locals["Time"] = netrpc.Time
	locals["Binary"] = netrpc.Binary
	#locals["Date"] = netrpc.Date
	#locals["Duration"] = netrpc.Duration
	#locals["Opaque"] = netrpc.Opaque
        params = eval(params,locals)
    except:
	info = sys.exc_info()
	exc_type = info[0]
	exc_value = info[1]
	exc_traceback = info[2]
	error = str(exc_type) + ": " + str(exc_value)
	self._methodResult.clear()
	self._methodResult.insert("end","Parameter Error\n","HEADING")
	self._methodResult.insert("end","\n")
	self._methodResult.insert("end",error)
    else:
      try:
	url = self._serverField.get()
	server = RemoteService(url)
	result = apply(server.__getattr__(self._methodField.get()),params)
      except netrpc.ServiceFailure,exception:
	self._methodResult.clear()
	self._methodResult.insert("end","Call Failure\n","HEADING")
	self._methodResult.insert("end","\n")
	self._methodResult.insert("end","Error: ")
	self._methodResult.insert("end",str(exception.error))
	self._methodResult.insert("end","\n")
	self._methodResult.insert("end","Description: ")
	self._methodResult.insert("end",exception.description)
	self._methodResult.insert("end","\n")
	if exception.origin != "":
	  self._methodResult.insert("end","Origin: ")
	  self._methodResult.insert("end",exception.origin)
	  self._methodResult.insert("end","\n")
	if exception.details != "":
	  self._methodResult.insert("end","\n")
	  self._methodResult.insert("end","Failure Details\n","HEADING")
	  self._methodResult.insert("end","\n")
	  self._methodResult.insert("end",exception.details)
	  self._methodResult.insert("end","\n")
	if RemoteService._request != "":
	  self._methodResult.insert("end","\n")
	  self._methodResult.insert("end","HTTP Request\n","HEADING")
	  self._methodResult.insert("end","\n")
	  self._methodResult.insert("end",RemoteService._request)
	if RemoteService._response != "":
	  self._methodResult.insert("end","\n")
	  self._methodResult.insert("end","HTTP Response\n","HEADING")
	  self._methodResult.insert("end","\n")
	  self._methodResult.insert("end",RemoteService._response)
      except:
	info = sys.exc_info()
	exc_type = info[0]
	exc_value = info[1]
	exc_traceback = info[2]
	error = str(exc_type) + ": " + str(exc_value)
	self._methodResult.clear()
	self._methodResult.insert("end","Error Occurred\n","HEADING")
	self._methodResult.insert("end","\n")
	self._methodResult.insert("end",error)
	self._methodResult.insert("end","\n")
      else:
	self._methodResult.clear()
	self._methodResult.insert("end","Call Successful\n","HEADING")
	self._methodResult.insert("end","\n")
	self._methodResult.insert("end",str(repr(result)))
	self._methodResult.insert("end","\n")
	if RemoteService._request != "":
	  self._methodResult.insert("end","\n")
	  self._methodResult.insert("end","HTTP Request\n","HEADING")
	  self._methodResult.insert("end","\n")
	  self._methodResult.insert("end",RemoteService._request)
	if RemoteService._response != "":
	  self._methodResult.insert("end","\n")
	  self._methodResult.insert("end","HTTP Response\n","HEADING")
	  self._methodResult.insert("end","\n")
	  self._methodResult.insert("end",RemoteService._response)


root = Pmw.initialise()
title = "XML-RPC Debugger"
main = RemoteServiceView(root,title=title)
root.mainloop()
