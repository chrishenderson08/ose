import signal
import netsvc
import netrpc

class Responder(netsvc.Service):
  def __init__(self):
    netsvc.Service.__init__(self,"responder")
    self.joinGroup("example")
    self.exportMethod(self.call)
    self.exportMethod(self.echo)
    self._callbacks = {}
  def call(self,url,method,delay,*args):
    details = (url,method,args)
    name = str(id(details))
    self._callbacks[name] = details
    self.startTimer(self.callback,delay,name)
    return 0
  def callback(self,name):
    url,method,args = self._callbacks[name]
    del self._callbacks[name]
    service = netrpc.RemoteService(url)
    try:
      getattr(service,method)(*args)
    except:
      print netsvc.exceptionDetails()
  def echo(self,*args):
    return args

dispatcher = netsvc.Dispatcher()
dispatcher.monitor(signal.SIGINT)
responder = Responder()
daemon = netsvc.HttpDaemon(8000)
rpcgw = netsvc.RpcGateway("example")
daemon.attach("/",rpcgw)
daemon.start()
dispatcher.run()
