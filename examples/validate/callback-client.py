import os
import signal
import netsvc
import netrpc

class Callback(netsvc.Service):
  def __init__(self,remote,local):
    netsvc.Service.__init__(self,"callback")
    self._remote = remote
    self._local = local
    self.joinGroup("callback")
    self.exportMethod(self.update)
    self.startTimer(self.timeout,2,"call")
  def timeout(self,name):
    service = netrpc.RemoteService(self._remote)
    try:
      now = netsvc.DateTime()
      service.call(self._local,"update",1,now)
    except:
      print netsvc.exceptionDetails()
    self.startTimer(self.timeout,5,"call")
  def update(self,time):
    now = netsvc.DateTime()
    print "update: now=%s then=%s" % (`str(now)`,`str(time)`)
    return 0


dispatcher = netsvc.Dispatcher()
dispatcher.monitor(signal.SIGINT)
daemon = netsvc.HttpDaemon(0)
rpcgw = netsvc.RpcGateway("callback")
daemon.attach("/",rpcgw)
daemon.start()

remote = "http://localhost:8000/responder"
local = "http://%s:%d/callback" % (os.uname()[1],daemon.port())
service = Callback(remote,local)

dispatcher.run()
