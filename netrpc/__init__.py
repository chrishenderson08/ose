# COPYRIGHT 2001-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

from common import *

class _RemoteService:

  def __call__(self,*args,**kwargs):
    import remote
    global RemoteService
    RemoteService = remote.RemoteService
    return apply(RemoteService,args,kwargs)

  def __getattr__(self,name):
    import remote
    global RemoteService
    RemoteService = remote.RemoteService
    return getattr(RemoteService,name)

  def __setattr__(self,name,value):
    import remote
    global RemoteService
    RemoteService = remote.RemoteService
    RemoteService.__dict__[name] = value

RemoteService = _RemoteService()

def asCGI(callback):
  from remote import _CGI
  _CGI(callback).execute()
