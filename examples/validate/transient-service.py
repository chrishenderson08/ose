import signal

import netsvc

class Object(netsvc.Service):
  index = {}
  def __init__(self,name,delay):
    netsvc.Service.__init__(self,name)
    Object.index[name] = self
    self._name = name
    self._delay = delay
    self._data = ""
    self.joinGroup("user")
    self.exportMethod(self.name)
    self.exportMethod(self.data)
    self.exportMethod(self.set)
    self.startTimer(self.timeout,self._delay,"idle")
    print "%s: new %s %s" % (str(netsvc.DateTime()),name,delay)
  def __del__(self):
    print "%s: del %s" % (str(netsvc.DateTime()),self._name)
  def destroyAgent(self):
    # MUST FIRST CALL THE BASE CLASS METHOD.
    netsvc.Service.destroyAgent(self)
    # MUST NOW UNDO OWN CIRCULAR REFERENCES.
    del Object.index[self._name]
  def reset(self):
    self.cancelTimer("idle")
    self.startTimer(self.timeout,self._delay,"idle")
  def delete(self):
    # VERY IMPORTANT TO CALL FOLLOWING METHOD.
    self.destroyAgent()
  def timeout(self,name):
    self.delete()
  def name(self):
    self.reset()
    return self._name
  def data(self):
    self.reset()
    return self._data
  def set(self,data):
    self.reset()
    self._data = data
    return netsvc.True

class Admin(netsvc.Service):
  def __init__(self):
    netsvc.Service.__init__(self,"admin")
    self.joinGroup("admin")
    self.exportMethod(self.create)
    self.exportMethod(self.list)
    self.exportMethod(self.delete)
  def create(self,name,delay):
    if Object.index.has_key(name):
      return netsvc.False
    user = Object(name,int(delay))
    return netsvc.True
  def list(self):
    result = []
    for user in Object.index.values():
      result.append(user.name())
    return result
  def delete(self,name):
    if not Object.index.has_key(name):
      return netsvc.False
    user = Object.index[name]
    user.delete()
    return netsvc.True

dispatcher = netsvc.Dispatcher()
dispatcher.monitor(signal.SIGINT)
daemon = netsvc.HttpDaemon(8000)

admin = Admin()

admin.create("zachariah",320)
admin.create("zachary",160)
admin.create("zack",80)
admin.create("zoe",40)
admin.create("zona",20)
admin.create("zora",10)

rpcgw1 = netsvc.RpcGateway("admin")
daemon.attach("/",rpcgw1)
rpcgw2 = netsvc.RpcGateway("user")
daemon.attach("/user",rpcgw2)

daemon.start()
dispatcher.run()
