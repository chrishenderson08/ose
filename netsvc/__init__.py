# COPYRIGHT 2001-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

import os
import sys
import types
import string
import posixpath
import fnmatch

try:
  import thread
  class _Lock:
    def __init__(self):
      self._mutex = thread.allocate_lock()
    def acquire(self):
      self._mutex.acquire()
    def release(self):
      self._mutex.release()
except:
  class _Lock:
    def acquire(self):
      pass
    def release(self):
      pass

import _ose

from netrpc.common import *
from cache import *

# Setup constants for dispatcher.

IDLE_JOB = 0
STANDARD_JOB = 1
PRIORITY_JOB = 2

# Force initialisation of dispatcher.

_ose_dispatcher = _ose.Dispatcher()

class _DispatcherTask:

  def __init__(self):
    self.start = _ose_dispatcher.task_start
    self.stop = _ose_dispatcher.task_stop
    self.wait = _ose_dispatcher.task_wait

_ose_dispatcher_task = _DispatcherTask()

class _Dispatcher:

  def __init__(self):
    self.run = _ose_dispatcher.run
    self.stop = _ose_dispatcher.stop
    self.schedule = _ose_dispatcher.schedule
    self.monitor = _ose_dispatcher.monitor
    self.shutdown = _ose_dispatcher.shutdown
    self.reset = _ose_dispatcher.reset
    self.wakeup = _ose_dispatcher.wakeup
    self.isWaiting = _ose_dispatcher.isWaiting
    self.isRunning = _ose_dispatcher.isRunning
    self.isStopped = _ose_dispatcher.isStopped
    self.warningsEnabled = _ose_dispatcher.warningsEnabled
    self.enableWarnings = _ose_dispatcher.enableWarnings
    self.disableWarnings = _ose_dispatcher.disableWarnings

  def task(self):
    return _ose_dispatcher_task

_dispatcher = _Dispatcher()

def Dispatcher():
  return _dispatcher

class Job:

  def __init__(self,callback,params=()):
    self._callback = callback
    self._params = params
    self._runnable = 1

  def execute(self,params=None):
    if self._runnable:
      if params == None:
        params = self._params
      apply(self._callback,params)

  def schedule(self,type,params=None):
    if params != None:
      self._params = params
    if self._runnable:
      _dispatcher.schedule(self,type)

  def cancel(self):
    self._runnable = 0

  def reset(self):
    self._runnable = 1

  def __nonzero__(self):
    return self._runnable != 0

  def __call__(self):
    self.execute(self._params)


# Define way of trying to stop the whole application.

def terminate():
  _dispatcher.stop()
  raise SystemExit

# Define wrapper for dispatcher job callback function.

def _apply(callback,arguments):

  try:
    return apply(callback,arguments)
  except SystemExit:
    terminate()
  except:
    logException(1)
    #terminate()

_ose.init_JobCallback(_apply)


# Force initialisation of program data.

_program = _ose.Program()

ID_LONG_FORMAT = 0
ID_SHORT_FORMAT = 1

uniqueId = _program.uniqueId
processIdentity = _program.processIdentity

removeAllConfig = _program.removeAllConfig
removeConfig = _program.removeConfig
loadConfig = _program.loadConfig
mergeConfig = _program.mergeConfig
lookupConfig = _program.lookupConfig
saveConfig = _program.saveConfig

lookupEnviron = _program.lookupEnviron
mergeEnviron = _program.mergeEnviron
expandEnviron = _program.expandEnviron

threadingEnabled = _program.threadingEnabled

# Setup constants for logger.

LOG_EMERGENCY = 0
LOG_ALERT = 1
LOG_CRITICAL = 2
LOG_ERROR = 3
LOG_WARNING = 4
LOG_NOTICE = 5
LOG_INFO = 6
LOG_DEBUG = 7

# Define logger, setup framework for channel monitors and function
# for dumping details of an exception out to the C++ logger.

_logSubscribers = {}

def _handleLogMessage(channel,level,message):
  for logger in _logSubscribers.keys():
    logger._handleLogMessage(channel,level,message)

_logger = _ose.new_Logger(_handleLogMessage)
if not lookupEnviron("OTCLIB_NOLOGSTDERR"):
  _logger.enableStderrOutput()


def logException(skip=0):
  error = exceptionDetails(skip)
  _logger.notify(LOG_ERROR,error)


class Logger:

  def __init__(self):
    self._callbacks = {}

    self.notify = _logger.notify
    self.notifyChannel = _logger.notifyChannel
    self.logFile = _logger.logFile
    self.setLogFile = _logger.setLogFile
    self.stderrOutputEnabled = _logger.stderrOutputEnabled
    self.enableStderrOutput = _logger.enableStderrOutput
    self.disableStderrOutput = _logger.disableStderrOutput
    self.longFormatEnabled = _logger.longFormatEnabled
    self.enableLongFormat = _logger.enableLongFormat
    self.disableLongFormat = _logger.disableLongFormat

  def destroyCallbacks(self):
    global _logSubscribers
    self._callbacks = {}
    if _logSubscribers.has_key(self):
       del _logSubscribers[self]

  def destroyReferences(self):
    # for backward compatibility only
    # use destroyCallbacks() instead
    self.destroyCallbacks()

  def monitorChannel(self,channel,callback):
    global _logSubscribers
    if callback == None:
      if self._callbacks.has_key(channel):
        del self._callbacks[channel]
        if len(self._callbacks.keys()) == 0:
          del _logSubscribers[self]
    else:
      self._callbacks[channel] = callback
      _logSubscribers[self] = 1

  def unexpectedException(self):
    logException()

  def _handleLogMessage(self,channel,level,message):
    try:
      if self._callbacks.has_key(channel):
        callback = self._callbacks[channel]
        callback(channel,level,message)
    except SystemExit:
      terminate()
    except:
      self.unexpectedException()


# Setup constants for agents.

SOCKET_POLLIN = 1
SOCKET_POLLPRI = 2
SOCKET_POLLOUT = 4
SOCKET_POLLERR = 8
SOCKET_POLLHUP = 16
SOCKET_POLLNVAL = 32

SHUTDOWN_PENDING = 0
SHUTDOWN_ARRIVED = 1

CALLBACK_TRANSIENT = 0
CALLBACK_PERSISTENT = 1

# Agent wrapper.

class Agent:

  def __init__(self,inner=None):
    if inner == None:
      inner = _ose.new_Agent(self)
    self._Agent = inner
    self._timerCallbacks = {}
    self._alarmCallbacks = {}
    self._cronCallbacks = {}
    self._jobCallbacks = {}
    self._transientCallbacks = {}
    self._persistentCallbacks = {}
    self._socketInCallbacks = {}
    self._socketPriCallbacks = {}
    self._socketOutCallbacks = {}
    self._signalCallbacks = {}
    self._startupCallback = None
    self._shutdownCallback = None

  def destroyAgent(self):
    self._Agent.suspendThreading()
    self.cancelAllTimers()
    self.cancelAllAlarms()
    self.cancelAllActions()
    self.unsubscribeAllSockets()
    self.unsubscribeAllSignals()
    self.unsubscribeStartup()
    self.unsubscribeShutdown()

  def destroyReferences(self):
    # for backward compatibility only
    # use destroyAgent() instead
    self.destroyAgent()

  def suspendEvents(self):
    self._Agent.suspendEvents()

  def resumeEvents(self):
    self._Agent.resumeEvents()

  def threadingEnabled(self):
    return self._Agent.threadingEnabled()

  def activateThreading(self):
    self._Agent.activateThreading(0)

  def shutdownThreading(self):
    self._Agent.shutdownThreading()

  def unexpectedException(self):
    logException(1)

  def startTimer(self,callback,delay,name=None):
    if name == None:
      name = "$TIMER?%s" % uniqueId(ID_SHORT_FORMAT)
    else:
      self.cancelTimer(name)
    self._timerCallbacks[name] = callback
    self._Agent.startTimer(delay,name)
    return name

  def cancelTimer(self,name):
    if self._timerCallbacks.has_key(name):
      del self._timerCallbacks[name]
      self._Agent.cancelTimer(name)

  def cancelAllTimers(self,pattern=None):
    if pattern != None:
      for name in self._timerCallbacks.keys():
	if fnmatch.fnmatchcase(name,pattern):
	  self.cancelTimer(name)
    else:
      self._timerCallbacks = {}
      self._Agent.cancelAllTimers()

  def timerPending(self,name):
    return self._timerCallbacks.has_key(name)

  def _handleTimer(self,name):
    try:
      if self._timerCallbacks.has_key(name):
	callback = self._timerCallbacks[name]
	del self._timerCallbacks[name]
	if type(callback) == types.MethodType:
	  nargs = callback.im_func.func_code.co_argcount-1
	elif type(callback) == types.FunctionType:
	  nargs = callback.func_code.co_argcount
	else:
	  nargs = 1
	if nargs == 0:
	  callback()
	else:
	  callback(name)
    except SystemExit:
      terminate()
    except:
      self.unexpectedException()

  def setAlarm(self,callback,time,name=None):
    if name == None:
      name = "$ALARM?%s" % uniqueId(ID_SHORT_FORMAT)
    else:
      self.cancelAlarm(name)
    self._alarmCallbacks[name] = callback
    self._Agent.setAlarm(time,name)
    return name

  def cancelAlarm(self,name):
    if self._alarmCallbacks.has_key(name):
      del self._alarmCallbacks[name]
      self._Agent.cancelAlarm(name)

  def cancelAllAlarms(self):
    self._alarmCallbacks = {}
    self._Agent.cancelAllAlarms()

  def alarmPending(self,name):
    return self._alarmCallbacks.has_key(name)

  def _handleAlarm(self,name):
    try:
      if self._alarmCallbacks.has_key(name):
	callback = self._alarmCallbacks[name]
	del self._alarmCallbacks[name]
        if type(callback) == types.MethodType:
          nargs = callback.im_func.func_code.co_argcount-1
        elif type(callback) == types.FunctionType:
          nargs = callback.func_code.co_argcount
        else:
          nargs = 1
	if nargs == 0:
	  callback()
	else:
	  callback(name)
    except SystemExit:
      terminate()
    except:
      self.unexpectedException()

  def scheduleJob(self,job,type):
    _dispatcher.schedule(job,type)

  def scheduleAction(self,callback,mode=STANDARD_JOB,name=None):
    if name == None:
      name = "$ACTION?%s" % uniqueId(ID_SHORT_FORMAT)
    else:
      self.cancelAction(name)
    if type(mode) is types.IntType:
      self._jobCallbacks[name] = callback
      self._Agent.scheduleAction(mode,name)
    else:
      self._cronCallbacks[name] = callback
      self._Agent.scheduleCronAction(mode,name)
    return name

  def subscribeAction(self,callback,mode=CALLBACK_TRANSIENT,name=None):
    if name == None:
      name = "$ACTION?%s" % uniqueId(ID_SHORT_FORMAT)
    else:
      self.cancelAction(name)
    if mode == CALLBACK_PERSISTENT:
      self._persistentCallbacks[name] = callback
    else:
      self._transientCallbacks[name] = callback
    return name

  def triggerAction(self,name,type):
    self._Agent.scheduleAction(type,name)

  def cancelAction(self,name):
    self._Agent.cancelAction(name)
    if self._cronCallbacks.has_key(name):
      del self._cronCallbacks[name]
    elif self._jobCallbacks.has_key(name):
      del self._jobCallbacks[name]
    elif self._transientCallbacks.has_key(name):
      del self._transientCallbacks[name]
    elif self._persistentCallbacks.has_key(name):
      del self._persistentCallbacks[name]

  def cancelAllActions(self):
    self._cronCallbacks = {}
    self._jobCallbacks = {}
    self._transientCallbacks = {}
    self._persistentCallbacks = {}
    self._Agent.cancelAllActions()

  def _handleAction(self,name):
    try:
      if self._cronCallbacks.has_key(name):
	callback = self._cronCallbacks[name]
        if type(callback) == types.MethodType:
          nargs = callback.im_func.func_code.co_argcount-1
        elif type(callback) == types.FunctionType:
          nargs = callback.func_code.co_argcount
        else:
          nargs = 1
	if nargs == 0:
	  callback()
	else:
	  callback(name)
      elif self._jobCallbacks.has_key(name):
	callback = self._jobCallbacks[name]
	del self._jobCallbacks[name]
        if type(callback) == types.MethodType:
          nargs = callback.im_func.func_code.co_argcount-1
        elif type(callback) == types.FunctionType:
          nargs = callback.func_code.co_argcount
        else:
          nargs = 1
	if nargs == 0:
	  callback()
	else:
	  callback(name)
      elif self._transientCallbacks.has_key(name):
	callback = self._transientCallbacks[name]
	del self._transientCallbacks[name]
        if type(callback) == types.MethodType:
          nargs = callback.im_func.func_code.co_argcount-1
        elif type(callback) == types.FunctionType:
          nargs = callback.func_code.co_argcount
        else:
          nargs = 1
	if nargs == 0:
	  callback()
	else:
	  callback(name)
      elif self._persistentCallbacks.has_key(name):
	callback = self._persistentCallbacks[name]
        if type(callback) == types.MethodType:
          nargs = callback.im_func.func_code.co_argcount-1
        elif type(callback) == types.FunctionType:
          nargs = callback.func_code.co_argcount
        else:
          nargs = 1
	if nargs == 0:
	  callback()
	else:
	  callback(name)
    except SystemExit:
      terminate()
    except:
      self.unexpectedException()

  def subscribeSocket(self,callback,fileno,event=SOCKET_POLLIN):
    self.unsubscribeSocket(fileno,event)
    if event & SOCKET_POLLIN:
      self._socketInCallbacks[fileno] = callback
    if event & SOCKET_POLLPRI:
      self._socketPriCallbacks[fileno] = callback
    if event & SOCKET_POLLOUT:
      self._socketOutCallbacks[fileno] = callback
    self._Agent.subscribeSocket(fileno,event)

  def unsubscribeSocket(self,fileno,event=~0):
    if event & SOCKET_POLLIN:
      if self._socketInCallbacks.has_key(fileno):
	del self._socketInCallbacks[fileno]
    if event & SOCKET_POLLPRI:
      if self._socketPriCallbacks.has_key(fileno):
	del self._socketPriCallbacks[fileno]
    if event & SOCKET_POLLOUT:
      if self._socketOutCallbacks.has_key(fileno):
	del self._socketOutCallbacks[fileno]
    self._Agent.unsubscribeSocket(fileno,event)

  def unsubscribeAllSockets(self):
    self._socketInCallbacks = {}
    self._socketPriCallbacks = {}
    self._socketOutCallbacks = {}
    self._Agent.unsubscribeAllSockets()

  def _handleSocket(self,fileno,event):
    try:
      prievents = event & SOCKET_POLLPRI
      if prievents != 0:
	if self._socketPriCallbacks.has_key(fileno):
	  callback = self._socketPriCallbacks[fileno]
	  if type(callback) == types.MethodType:
	    nargs = callback.im_func.func_code.co_argcount-1
	  elif type(callback) == types.FunctionType:
	    nargs = callback.func_code.co_argcount
	  else:
	    nargs = 3
	  if nargs == 0:
	    callback()
	  elif nargs == 1:
	    callback(fileno)
	  else:
	    callback(fileno,SOCKET_POLLPRI)
      outevents = event & SOCKET_POLLOUT
      event = event & ~SOCKET_POLLPRI
      event = event & ~SOCKET_POLLOUT
      if event != 0:
	if self._socketInCallbacks.has_key(fileno):
	  callback = self._socketInCallbacks[fileno]
	  if type(callback) == types.MethodType:
	    nargs = callback.im_func.func_code.co_argcount-1
	  elif type(callback) == types.FunctionType:
	    nargs = callback.func_code.co_argcount
	  else:
	    nargs = 2
	  if nargs == 0:
	    callback()
	  elif nargs == 1:
	    callback(fileno)
	  else:
	    callback(fileno,event)
      if outevents != 0:
	if self._socketOutCallbacks.has_key(fileno):
	  callback = self._socketOutCallbacks[fileno]
	  if type(callback) == types.MethodType:
	    nargs = callback.im_func.func_code.co_argcount-1
	  elif type(callback) == types.FunctionType:
	    nargs = callback.func_code.co_argcount
	  else:
	    nargs = 3
	  if nargs == 0:
	    callback()
	  elif nargs == 1:
	    callback(fileno)
	  else:
	    callback(fileno,SOCKET_POLLOUT)
    except SystemExit:
      terminate()
    except:
      self.unexpectedException()

  def subscribeSignal(self,callback,num):
    self.unsubscribeSignal(num)
    self._signalCallbacks[num] = callback
    self._Agent.subscribeSignal(num)

  def unsubscribeSignal(self,num):
    if self._signalCallbacks.has_key(num):
      del self._signalCallbacks[num]
      self._Agent.unsubscribeSignal(num)

  def unsubscribeAllSignals(self):
    self._signalCallbacks = {}
    self._Agent.unsubscribeAllSignals()

  def _handleSignal(self,num):
    try:
      if self._signalCallbacks.has_key(num):
	callback = self._signalCallbacks[num]
	callback(num)
    except SystemExit:
      terminate()
    except:
      self.unexpectedException()

  def scheduleStartup(self):
    self.scheduleAction(self._handleStartup)

  def subscribeStartup(self,callback):
    self._startupCallback = callback

  def unsubscribeStartup(self):
    self._startupCallback = None

  def _handleStartup(self):
    if self._startupCallback != None:
      callback = self._startupCallback
      self._startupCallback = None
      callback()

  def scheduleShutdown(self,delay=0.0):
    self._Agent.scheduleShutdown(float(delay))

  def suspendShutdown(self):
    self._Agent.suspendShutdown()

  def resumeShutdown(self):
    self._Agent.resumeShutdown()

  def subscribeShutdown(self,callback):
    self.unsubscribeShutdown()
    self._shutdownCallback = callback
    self._Agent.subscribeShutdown()

  def unsubscribeShutdown(self):
    if self._shutdownCallback != None:
      self._shutdownCallback = None
      self._Agent.unsubscribeShutdown()

  def _handleShutdown(self,category,description):
    try:
      if self._shutdownCallback != None:
	callback = self._shutdownCallback
        if type(callback) == types.MethodType:
          nargs = callback.im_func.func_code.co_argcount-1
        elif type(callback) == types.FunctionType:
          nargs = callback.func_code.co_argcount
        else:
          nargs = 2
	if nargs == 1:
	  callback(category)
	else:
	  callback(category,description)
    except SystemExit:
      terminate()
    except:
      self.unexpectedException()


# Setup constants for services.

SERVICE_LOCAL = 0
SERVICE_REMOTE = 1

SERVICE_AVAILABLE = 0
SERVICE_WITHDRAWN = 1

SERVICE_NAME = 0
SERVICE_ADDRESS = 1
SERVICE_REGISTRY = 2
SERVICE_GROUP = 3

SUBSCRIPTION_REQUESTED = 0
SUBSCRIPTION_WITHDRAWN = 1

SERVICE_HIDDEN = 0
SERVICE_VISIBLE = 1

# Define classes for registry monitor.

class Monitor(Agent):

  def __init__(self,inner=None):
    if inner == None:
      inner = _ose.new_Monitor(self)
    Agent.__init__(self,inner)
    self._serviceNameCallbacks = {}
    self._serviceAddressCallbacks = {}
    self._serviceGroupCallbacks = {}
    self._registryCallback = None

  def destroyAgent(self):
    Agent.destroyAgent(self)
    self._registryCallback = None
    self._serviceNameCallbacks = {}
    self._serviceAddressCallbacks = {}
    self._serviceGroupCallbacks = {}
    self._Agent.unsubscribeAllRegistry()

  def serviceAgents(self):
    return self._Agent.serviceAgents()

  def serviceGroups(self,serviceAddress=None):
    return self._Agent.serviceGroups(serviceAddress)

  def lookupServiceName(self,serviceName):
    return self._Agent.lookupServiceName(serviceName)

  def lookupServiceAddress(self,serviceAddress):
    return self._Agent.lookupServiceAddress(serviceAddress)

  def lookupServiceGroup(self,serviceGroup):
    return self._Agent.lookupServiceGroup(serviceGroup)

  def subscribeRegistry(self,callback):
    self.unsubscribeRegistry()
    self._registryCallback = callback
    self._Agent.subscribeRegistry()

  def unsubscribeRegistry(self):
    if self._registryCallback != None:
      self._registryCallback = None
      self._Agent.unsubscribeRegistry()

  def _handleRegistry(self,binding,group,status):
    try:
      if self._registryCallback != None:
	callback = self._registryCallback
	callback(binding,group,status)
    except SystemExit:
      terminate()
    except:
      self.unexpectedException()

  def subscribeServiceName(self,callback,serviceName):
    self.unsubscribeServiceName(serviceName)
    self._serviceNameCallbacks[serviceName] = callback
    self._Agent.subscribeServiceName(serviceName)

  def unsubscribeServiceName(self,serviceName):
    if self._serviceNameCallbacks.has_key(serviceName):
      del self._serviceNameCallbacks[serviceName]
      self._Agent.unsubscribeServiceName(serviceName)

  def _handleServiceName(self,binding,status):
    try:
      serviceName = binding.serviceName()
      if self._serviceNameCallbacks.has_key(serviceName):
	callback = self._serviceNameCallbacks[serviceName]
        if type(callback) == types.MethodType:
          nargs = callback.im_func.func_code.co_argcount-1
        elif type(callback) == types.FunctionType:
          nargs = callback.func_code.co_argcount
        else:
          nargs = 3
	if nargs == 2:
	  callback(binding,status)
	else:
	  callback(binding,None,status)
    except SystemExit:
      terminate()
    except:
      self.unexpectedException()

  def subscribeServiceAddress(self,callback,serviceAddress,subscription=""):
    key = (serviceAddress,subscription)
    self.unsubscribeServiceAddress(serviceAddress,subscription)
    self._serviceAddressCallbacks[key] = callback
    return self._Agent.subscribeServiceAddress(serviceAddress,subscription)

  def unsubscribeServiceAddress(self,serviceAddress,subscription=""):
    key = (serviceAddress,subscription)
    if self._serviceAddressCallbacks.has_key(key):
      del self._serviceAddressCallbacks[key]
      self._Agent.unsubscribeServiceAddress(serviceAddress,subscription)

  def _handleServiceAddress(self,binding,group,status,subscription):
    try:
      serviceAddress = binding.serviceAddress()
      key = (serviceAddress,subscription)
      if self._serviceAddressCallbacks.has_key(key):
	callback = self._serviceAddressCallbacks[key]
        if type(callback) == types.MethodType:
          nargs = callback.im_func.func_code.co_argcount-1
        elif type(callback) == types.FunctionType:
          nargs = callback.func_code.co_argcount
        else:
          nargs = 4
	if nargs == 3:
	  callback(binding,group,status)
	else:
	  callback(binding,group,status,subscription)
    except SystemExit:
      terminate()
    except:
      self.unexpectedException()

  def subscribeServiceGroup(self,callback,serviceGroup):
    self.unsubscribeServiceGroup(serviceGroup)
    self._serviceGroupCallbacks[serviceGroup] = callback
    self._Agent.subscribeServiceGroup(serviceGroup)

  def unsubscribeServiceGroup(self,serviceGroup):
    if self._serviceGroupCallbacks.has_key(serviceGroup):
      del self._serviceGroupCallbacks[serviceGroup]
      self._Agent.unsubscribeServiceGroup(serviceGroup)

  def _handleServiceGroup(self,binding,group,status):
    try:
      if self._serviceGroupCallbacks.has_key(group):
	callback = self._serviceGroupCallbacks[group]
        if type(callback) == types.MethodType:
          nargs = callback.im_func.func_code.co_argcount-1
        elif type(callback) == types.FunctionType:
          nargs = callback.func_code.co_argcount
        else:
          nargs = 3
	if nargs == 2:
	  callback(binding,status)
	else:
	  callback(binding,group,status)
    except SystemExit:
      terminate()
    except:
      self.unexpectedException()


class _Registry:

  def __init__(self):
    self._Monitor = _ose.new_Monitor(self)

  def serviceAgents(self):
    return self._Monitor.serviceAgents()

  def serviceGroups(self,serviceAddress=None):
    return self._Monitor.serviceGroups(serviceAddress)

  def lookupServiceName(self,serviceName):
    return self._Monitor.lookupServiceName(serviceName)

  def lookupServiceAddress(self,serviceAddress):
    return self._Monitor.lookupServiceAddress(serviceAddress)

  def lookupServiceGroup(self,serviceGroup):
    return self._Monitor.lookupServiceGroup(serviceGroup)


_registry = _Registry()

def Registry():
  return _registry


# Define outer classes for corresponding inner C++ classes.

class _Message:

  def __init__(self,inner,service):
    self._inner = inner
    self._service = service
    self.sender = self._inner.sender
    self.conversationId = self._inner.conversationId
    self.sequenceNumber = self._inner.sequenceNumber
    self.messageId = self._inner.messageId
    self.description = self._inner.description
    self.content = self._inner.content
    self.contentType = self._inner.contentType


class _Request:

  def __init__(self,inner,service):
    self._inner = inner
    self._service = service
    self.sender = self._inner.sender
    self.conversationId = self._inner.conversationId
    self.sequenceNumber = self._inner.sequenceNumber
    self.methodName = self._inner.methodName
    self.bounceRequest = self._inner.bounceRequest
    self.sendFailure = self._inner.sendFailure

  def content(self):
    return self._inner.content(self._service.decodeValue)

  def sendResponse(self,content):
    self._inner.sendResponse(content,self._service.encodeObject)


class _Response:

  def __init__(self,inner,service):
    self._inner = inner
    self._service = service
    self.sender = self._inner.sender
    self.conversationId = self._inner.conversationId
    self.sequenceNumber = self._inner.sequenceNumber

  def content(self):
    return self._inner.content(self._service.decodeValue)


class _Report:

  def __init__(self,inner,service):
    self._inner = inner
    self._service = service
    self.publisher = self._inner.publisher
    self.subjectName = self._inner.subjectName
    self.subjectPattern = self._inner.subjectPattern

  def content(self):
    return self._inner.content(self._service.decodeValue)


class _RemoteFailure:

  def __init__(self,inner):
    self._inner = inner
    self.conversationId = self._inner.conversationId
    self.error = self._inner.error
    self.description = self._inner.description
    self.origin = self._inner.origin
    self.details = self._inner.details

  def __repr__(self):
    return ("<_RemoteFailure %s, description=%s, origin=%s, details=%s>" %
        (self.error(),`self.description()`,`self.origin()`,`self.details()`))

  __str__ = __repr__


class _LocalFailure:

  def __init__(self,conversationId,error,description,origin="",details=""):
    self._conversationId = conversationId
    self._error = error
    self._description = description
    self._origin = origin
    self._details = details

  def conversationId(self):
    return self._conversationId

  def error(self):
    return self._error

  def description(self):
    return self._description

  def origin(self):
    return self._origin

  def details(self):
    return self._details

  def __repr__(self):
    return ("<_LocalFailure %s, description=%s, origin=%s, details=%s>" %
        (self.error(),`self.description()`,`self.origin()`,`self.details()`))

  __str__ = __repr__


class _RequestTimeout:

  def __init__(self,conversationId,error,description,origin="",details=""):
    self._conversationId = conversationId
    self._error = error
    self._description = description
    self._origin = origin
    self._details = details

  def conversationId(self):
    return self._conversationId

  def sequenceNumber(self):
    return self._sequenceNumber

  def error(self):
    return self._error

  def description(self):
    return self._description

  def origin(self):
    return self._origin

  def details(self):
    return self._details

  def __repr__(self):
    return ("<_RequestTimeout %s, description=%s, origin=%s, details=%s>" %
        (self.error(),`self.description()`,`self.origin()`,`self.details()`))

  __str__ = __repr__


class _ServiceWithdrawn:

  def __init__(self,conversationId,error,description,origin="",details=""):
    self._conversationId = conversationId
    self._error = error
    self._description = description
    self._origin = origin
    self._details = details

  def conversationId(self):
    return self._conversationId

  def sequenceNumber(self):
    return self._sequenceNumber

  def error(self):
    return self._error

  def description(self):
    return self._description

  def origin(self):
    return self._origin

  def details(self):
    return self._details

  def __repr__(self):
    return ("<_ServiceWithdrawn %s, description=%s, origin=%s, details=%s>" %
        (self.error(),`self.description()`,`self.origin()`,`self.details()`))

  __str__ = __repr__


class DelayedResponse:
  pass


class _ServiceMethod:

  def __init__(self,local,serviceName,binding,methodName):
    self._local = local
    self._serviceName = serviceName
    self._binding = binding
    self._methodName = methodName

  def __getattr__(self,methodName):
    return _ServiceMethod(self._local,self._serviceName,self._binding,
        "%s.%s"%(self._methodName,methodName))

  def __call__(self,*args):
    if self._binding == None:
      self._local._lastRequest = None
      error = CLIENT_SERVICE_UNAVAILABLE
      description = CLIENT_SERVICE_UNAVAILABLE_MESSAGE
      origin = "netsvc"
      details = self._serviceName
      raise ServiceUnavailable(error,description,origin,details)
    id = self._local._sendRequest(self._binding,self._methodName,args)
    return id


class _ServiceEndPoint:

  def __init__(self,local,serviceName,binding):
    self._local = local
    self._serviceName = serviceName
    self._binding = binding

  def __getattr__(self,methodName):
    return _ServiceMethod(self._local,
        self._serviceName,self._binding,methodName)

  def __coerce__(self,other):
    if other == None:
      return (self._binding,other)
    return None

  def __ne__(self,other):
    return self._binding != other

  def __nonzero__(self):
    return self._binding != None


class Service(Monitor):

  _services = {}
  _servicesLock = _Lock()

  def __init__(self,name="",audience="*",visibility=SERVICE_VISIBLE):
    inner = _ose.new_Service(self,name,audience,visibility)
    Monitor.__init__(self,inner)
    self._serviceBinding = self._Agent.serviceBinding()
    Service._servicesLock.acquire()
    if not Service._services.has_key(name):
      Service._services[name] = [self]
    else:
      Service._services[name].append(self)
    Service._servicesLock.release()
    self._exportedMethods = {}
    self._lastRequest = None
    self._requestCallbacks = {}
    self._failureCallbacks = {}
    self._messageSubscriptions = {}
    self._activeSubscriptions = {}
    self._passiveSubscriptions = {}
    self._delayedResponses = {}
    self._pendingCancellations = {}
    self._currentMessage = None
    self._currentRequest = None
    self._currentResponse = None
    self._currentFailure = None
    self._currentReport = None

  def destroyAgent(self):
    Monitor.destroyAgent(self)
    Service._servicesLock.acquire()
    index = Service._services[self.serviceName()].index(self)
    del Service._services[self.serviceName()][index]
    if len(Service._services[self.serviceName()]) == 0:
      del Service._services[self.serviceName()]
    Service._servicesLock.release()
    self._serviceBinding = None
    self._exportedMethods = {}
    self._lastRequest = None
    self._requestCallbacks = {}
    self._failureCallbacks = {}
    self._messageSubscriptions = {}
    self._activeSubscriptions = {}
    self._passiveSubscriptions = {}
    self._delayedResponses = {}
    self._pendingCancellations = {}
    self._currentMessage = None
    self._currentRequest = None
    self._currentResponse = None
    self._currentFailure = None
    self._currentReport = None

  def serviceBinding(self):
    return self._serviceBinding

  def serviceName(self):
    return self._serviceBinding.serviceName()

  def serviceAudience(self):
    return self._serviceBinding.serviceAudience()

  def serviceAddress(self):
    return self._serviceBinding.serviceAddress()

  def agentIdentity(self):
    return self._serviceBinding.agentIdentity()

  def processIdentity(self):
    return self._serviceBinding.processIdentity()

  """
  def networkGroup(self):
    return self._serviceBinding.networkGroup()
  """

  def exportMethod(self,method,methodName=None):
    if methodName == None:
      methodName = method.__name__
    self._exportedMethods[methodName] = method

  """
  def exportBuiltins(self):
    self.exportMethod(self.exportedMethods)
    self.exportMethod(self.serviceName)
    self.exportMethod(self.serviceAddress)
    self.exportMethod(self.serviceAudience)
    self.exportMethod(self.agentIdentity)
    self.exportMethod(self.processIdentity)
    self.exportMethod(self.networkGroup)
  """

  def exportedMethods(self):
    return self._exportedMethods.keys()

  def lookupMethod(self,methodName):
    if self._exportedMethods.has_key(methodName):
      return self._exportedMethods[methodName]
    return None

  def serviceEndPoint(self,service):
    binding = None
    if type(service) != type(self.serviceBinding()):
      bindings = self.lookupServiceName(service)
      if len(bindings) != 0:
        binding = bindings[0]
    else:
      binding = service
      service = binding.serviceName()
    return _ServiceEndPoint(self,service,binding)

  def decodeValue(self,name,string):
    return decodeValue(name,string)

  def encodeObject(self,object):
    return encodeObject(object)

  def serviceVisibility(self):
    return self._Agent.serviceVisibility()

  def announceAvailability(self):
    self._Agent.announceAvailability()

  def joinGroup(self,serviceGroup):
    self._Agent.joinGroup(serviceGroup)

  def leaveGroup(self,serviceGroup):
    self._Agent.leaveGroup(serviceGroup)

  def sendMessage(self,agent,messageId,description,content,
      contentType="",sequenceNumber=0):
    return self._Agent.sendMessage(agent,messageId,
	description,content,contentType,sequenceNumber)

  def _sendRequest(self,agent,methodName,content):
    id = self._Agent.sendRequest(agent,methodName,content,self.encodeObject)
    self._lastRequest = (id,agent,methodName)
    return id

  def sendRequest(self,agent,methodName,*args):
    return self._sendRequest(agent,methodName,args)

  def sendResponse(self,request,result):
    request.sendResponse(result)

  def sendFailure(self,request,error,description,origin="",details=""):
    request.sendFailure(error,description,origin,details)

  def returnResponse(self,result=None):
    request = self._currentRequest
    self.sendResponse(request,result)
    self.delayResponse()

  def deliverResponse(self,id,result=None):
    if self._delayedResponses.has_key(id):
      request,callback = self._delayedResponses[id]
      del self._delayedResponses[id]
      self.sendResponse(request,result)

  def conversationId(self):
    if self._currentRequest:
      return self._currentRequest.conversationId()
    elif self._currentResponse:
      return self._currentResponse.conversationId()
    elif self._currentFailure:
      return self._currentFailure.conversationId()
    elif self._currentMessage:
      return self._currentMessage.conversationId()
    else:
      return None

  def currentMessage(self):
    return self._currentMessage

  def currentRequest(self):
    return self._currentRequest

  def currentResponse(self):
    return self._currentResponse

  def currentFailure(self):
    return self._currentFailure

  def failureOccurred(self):
    return self._currentFailure != None

  def currentReport(self):
    return self._currentReport

  """
  def lastRequest(self):
    return self._lastRequest
  """

  def abortResponse(self,error,description,origin="",details=""):
    raise ServiceFailure(error,description,origin,details)

  def delayResponse(self):
    raise DelayedResponse()

  def executeMethod(self,methodName,method,params):
    return apply(method,params)

  def _localRequest(self,methodName,params):
    try:
      self._Agent.acquireObject()
      method = None
      if self._exportedMethods.has_key(methodName):
	method = self._exportedMethods[methodName]

      if method != None:
	argcount = method.im_func.func_code.co_argcount-1
	if method.im_func.func_defaults != None:
	  argdflts = len(method.im_func.func_defaults)
	else:
	  argdflts = 0
	varargs = method.im_func.func_code.co_flags & 4
	minargs = argcount - argdflts
	maxargs = argcount
	numargs = len(params)
	if numargs < minargs or (numargs > maxargs and not varargs):
	  error = SERVER_PARAMETERS_INCORRECT
	  description = SERVER_PARAMETERS_INCORRECT_MESSAGE
	  origin = "netsvc"
	  if not varargs:
	    if minargs != maxargs:
	      details = "Expected %d-%d arguments, received %d."
	      details = details % (minargs,maxargs,numargs)
	    else:
	      details = "Expected %d arguments, received %d."
	      details = details % (minargs,numargs)
	  else:
	    details = "Expected %d or more arguments, received %d."
	    details = details % (minargs,numargs)
	  self.abortResponse(error,description,origin,details)
	result = self.executeMethod(methodName,method,params)
      else:
	result = self.invalidMethod(methodName,params)
    except ServiceFailure:
      self._Agent.releaseObject()
      raise
    except SystemExit:
      self._Agent.releaseObject()
      raise
    except:
      self._Agent.releaseObject()
      error = SERVER_APPLICATION_ERROR
      description = SERVER_APPLICATION_ERROR_MESSAGE
      origin = "netsvc"
      details = exceptionDetails()
      self.abortResponse(error,description,origin,details)
    else:
      self._Agent.releaseObject()
      return result

  def processMessages(self,callback,sender,messageId):
    if not self._messageSubscriptions.has_key(sender):
      self._messageSubscriptions[sender] = {}
    self._messageSubscriptions[sender][messageId] = callback

  def discardMessages(self,sender,messageId=None):
    if self._messageSubscriptions.has_key(sender):
      if messageId != None:
	if self._messageSubscriptions[sender].has_key(messageId):
	  del self._messageSubscriptions[sender][messageId]
      else:
	del self._messageSubscriptions[sender]

  def _handleMessage(self,message):
    try:
      self.handleMessage(_Message(message,self))
    except SystemExit:
      terminate()
    except:
      self.unexpectedException()

  def handleMessage(self,message):
    sender = message.sender()
    messageId = message.messageId()
    description = message.description()
    content = message.content()
    contentType = message.contentType()
    sequenceNumber = message.sequenceNumber()
    callback = None
    if self._messageSubscriptions.has_key(sender):
      if self._messageSubscriptions[sender].has_key(messageId):
	callback = self._messageSubscriptions[sender][messageId]
    if callback == None:
      callback = self.unexpectedMessage
    self._currentMessage = message
    try:
      if type(callback) == types.MethodType:
	nargs = callback.im_func.func_code.co_argcount-1
      elif type(callback) == types.FunctionType:
	nargs = callback.func_code.co_argcount
      else:
	nargs = 6
      if nargs == 3:
	callback(description,content,contentType)
      elif nargs == 4:
	callback(description,content,contentType,sequenceNumber)
      else:
	callback(sender,messageId,description,content,
	    contentType,sequenceNumber)
      self._currentMessage = None
    except SystemExit:
      self._currentMessage = None
      terminate()
    except:
      self._currentMessage = None
      self.unexpectedException()

  def unexpectedMessage(self,sender,messageId,description,
      content,contentType,sequenceNumber):
    return

  def _handleRequest(self,request):
    try:
      self.handleRequest(_Request(request,self))
    except SystemExit:
      terminate()
    except:
      self.unexpectedException()

  def handleRequest(self,request):
    method = None
    methodName = request.methodName()

    if self._exportedMethods.has_key(methodName):
      method = self._exportedMethods[methodName]

    try:
      params = request.content()
    except:
      error = SERVER_REQUEST_DECODING_ERROR
      description = SERVER_REQUEST_DECODING_ERROR_MESSAGE
      origin = "netsvc"
      details = exceptionDetails()
      self.sendFailure(request,error,description,origin,details)
      return
    self._currentRequest = request
    try:
      if method != None:
	argcount = method.im_func.func_code.co_argcount-1
	if method.im_func.func_defaults != None:
	  argdflts = len(method.im_func.func_defaults)
	else:
	  argdflts = 0
	varargs = method.im_func.func_code.co_flags & 4
	minargs = argcount - argdflts
	maxargs = argcount
	numargs = len(params)
	if numargs < minargs or (numargs > maxargs and not varargs):
	  error = SERVER_PARAMETERS_INCORRECT
	  description = SERVER_PARAMETERS_INCORRECT_MESSAGE
	  origin = "netsvc"
	  if not varargs:
	    if minargs != maxargs:
	      details = "Expected %d-%d arguments, received %d."
	      details = details % (minargs,maxargs,numargs)
	    else:
	      details = "Expected %d arguments, received %d."
	      details = details % (minargs,numargs)
	  else:
	    details = "Expected %d or more arguments, received %d."
	    details = details % (minargs,numargs)
	  self.abortResponse(error,description,origin,details)
	result = self.executeMethod(methodName,method,params)
      else:
        result = self.invalidMethod(methodName,params)
      self._currentRequest = None
    except DelayedResponse:
      self._currentRequest = None
      pass
    except ServiceFailure,exception:
      self._currentRequest = None
      error = exception.error
      description = exception.description
      origin = exception.origin
      details = exception.details
      self.sendFailure(request,error,description,origin,details)
    except SystemExit:
      self._currentRequest = None
      raise
    except:
      self._currentRequest = None
      error = SERVER_APPLICATION_ERROR
      description = SERVER_APPLICATION_ERROR_MESSAGE
      origin = "netsvc"
      details = exceptionDetails()
      self.sendFailure(request,error,description,origin,details)
    else:
      self._currentRequest = None
      try:
	self.sendResponse(request,result)
      except:
	error = SERVER_RESPONSE_ENCODING_ERROR
	description = SERVER_RESPONSE_ENCODING_ERROR_MESSAGE
	origin = "netsvc"
	details = exceptionDetails()
	self.sendFailure(request,error,description,origin,details)

  def invalidMethod(self,methodName,params):
    error = SERVER_METHOD_UNAVAILABLE
    description = SERVER_METHOD_UNAVAILABLE_MESSAGE
    origin = "netsvc"
    self.abortResponse(error,description,origin)

  def monitorResponse(self,callback,id,timeout=None):
    if id != None:
      if self._lastRequest == None or self._lastRequest[0] != id:
        raise RuntimeError("Conversation ID doesn't match last request.")
      id,binding,methodName = self._lastRequest
      self._requestCallbacks[id] = (callback,binding)
      self._failureCallbacks[id] = (callback,binding)
      if timeout != None:
	timer = "$RESPONSE-TIMEOUT/%s" % id
	self.startTimer(self._handleResponseTimeout,timeout,timer)

  def processResponse(self,callback,id):
    if id != None:
      if self._lastRequest == None or self._lastRequest[0] != id:
        raise RuntimeError("Conversation ID doesn't match last request.")
      id,binding,methodName = self._lastRequest
      self._requestCallbacks[id] = (callback,binding)

  def processFailure(self,callback,id,timeout=None):
    if id != None:
      if self._lastRequest == None or self._lastRequest[0] != id:
        raise RuntimeError("Conversation ID doesn't match last request.")
      id,binding,methodName = self._lastRequest
      self._failureCallbacks[id] = (callback,binding)
      if timeout != None:
	timer = "$RESPONSE-TIMEOUT/%s" % id
	self.startTimer(self._handleResponseTimeout,timeout,timer)
      subscription = "$SERVICE-STATUS/%s" % id
      if self.subscribeServiceAddress(self._handleServiceWithdrawn, \
          binding.serviceAddress(),subscription) == SERVICE_WITHDRAWN:
	self.unsubscribeServiceAddress(binding.serviceAddress(),subscription)
	self.scheduleAction(self._handleServiceUnavailable, \
	    PRIORITY_JOB,subscription)

  def ignoreResponse(self,id):
    binding = None
    if self._requestCallbacks.has_key(id):
      binding = self._requestCallbacks[id][1]
      del self._requestCallbacks[id]
    if self._failureCallbacks.has_key(id):
      binding = self._failureCallbacks[id][1]
      del self._failureCallbacks[id]
    if binding:
      subscription = "$SERVICE-STATUS/%s" % id
      self.unsubscribeServiceAddress(binding.serviceAddress(),subscription)
    timer = "$RESPONSE-TIMEOUT/%s" % id
    self.cancelTimer(timer)

  def responsePending(self,id):
    return self._requestCallbacks.has_key(id) or \
	self._failureCallbacks.has_key(id)

  def _voidResponse(self):
    return None

  def suspendResponse(self,callback=None):
    if self._currentRequest != None:
      if callback == None:
	callback = self._voidResponse
      id = self._currentRequest.conversationId()
      self._delayedResponses[id] = (self._currentRequest,callback)
      self.delayResponse()

  def resumeResponse(self,id):
    action = "$RESUME-RESPONSE/%s" % id
    self.scheduleAction(self._handleResponseResumption,PRIORITY_JOB,action)

  def _handleResponseResumption(self,action):
    id = string.splitfields(action,'/')[1]
    if self._delayedResponses.has_key(id):
      request,callback = self._delayedResponses[id]
      del self._delayedResponses[id]
      self._currentRequest = request
      try:
        if type(callback) == types.MethodType:
          nargs = callback.im_func.func_code.co_argcount-1
        elif type(callback) == types.FunctionType:
          nargs = callback.func_code.co_argcount
        else:
          nargs = 1
	if nargs == 0:
	  result = callback()
	else:
	  result = callback(self.conversationId())
	self.sendResponse(request,result)
      except DelayedResponse:
	pass
      except ServiceFailure,exception:
	self.sendFailure(request,exception.error(),exception.description(),
	    exception.origin(),exception.details())
      except:
	error = SERVER_APPLICATION_ERROR
	description = SERVER_APPLICATION_ERROR_MESSAGE
	origin = "netsvc"
	details = exceptionDetails()
	self.sendFailure(request,error,description,origin,details)
      self._currentRequest = None

  def abandonResponse(self,id):
    if self._delayedResponses.has_key(id):
      request,callback = self._delayedResponses[id]
      del self._delayedResponses[id]

  def cancelResponse(self,id,error,description,origin="",details=""):
    if self._delayedResponses.has_key(id):
      request,callback = self._delayedResponses[id]
      del self._delayedResponses[id]
      self.sendFailure(request,error,description,origin,details)

  def cancelAllResponses(self,error,description,origin="",details=""):
    for id in self._delayedResponses.keys():
      self.cancelResponse(id,error,description,origin,details)

  def cancelRequest(self,id,error,description,origin="",details=""):
    action = "$CANCEL-REQUEST/%s" % id
    if not self._pendingCancellations.has_key(action):
      self.scheduleAction(self._handleRequestCancellation,PRIORITY_JOB,action)
      self._pendingCancellations[action] = (id,error,description,origin,details)

  def cancelAllRequests(self,error,description,origin="",details=""):
    ids = {}
    for id in self._requestCallbacks.keys():
      ids[id] = None
    for id in self._failureCallbacks.keys():
      ids[id] = None
    for id in ids.keys():
      self.cancelRequest(id,error,description,origin,details)

  def _handleRequestCancellation(self,action):
    if self._pendingCancellations.has_key(action):
      (id,error,description,origin,details) = self._pendingCancellations[action]
      del self._pendingCancellations[action]
      failure = _LocalFailure(id,error,description,origin,details)
      try:
	self.handleFailure(failure)
      except SystemExit:
	terminate()
      except:
	self.unexpectedException()

  def _handleResponse(self,response):
    try:
      self.handleResponse(_Response(response,self))
    except SystemExit:
      terminate()
    except:
      self.unexpectedException()

  def handleResponse(self,response):
    id = response.conversationId()
    fallback = None
    if self._failureCallbacks.has_key(id):
      fallback,binding = self._failureCallbacks[id]
    if self._requestCallbacks.has_key(id):
      callback,binding = self._requestCallbacks[id]
      self.ignoreResponse(id)
      try:
	content = response.content()
      except:
	if fallback != None:
	  error = CLIENT_RESPONSE_DECODING_ERROR
	  description = CLIENT_RESPONSE_DECODING_ERROR_MESSAGE
	  origin = "netsvc"
	  details = exceptionDetails()
	  self._currentFailure = _LocalFailure(id,
	      error,description,origin,details)
	  self._currentResponse = response
	  try:
	    if type(fallback) == types.MethodType:
	      nargs = fallback.im_func.func_code.co_argcount-1
	    elif type(fallback) == types.FunctionType:
	      nargs = fallback.func_code.co_argcount
	    else:
	      nargs = 1
	    if nargs == 0:
	      fallback()
	    elif nargs == 1:
	      if fallback == callback:
		# For backwards compatbility.
		fallback(None)
	      else:
		fallback(id)
	    else:
	      fallback(id,self._currentFailure.error(),
		  self._currentFailure.description(),
		  self._currentFailure.origin(),
		  self._currentFailure.details())
	  finally:
	    self._currentFailure = None
	    self._currentResponse = None
      else:
	self._currentResponse = response
        try:
	  if type(callback) == types.MethodType:
	    nargs = callback.im_func.func_code.co_argcount-1
	  elif type(callback) == types.FunctionType:
	    nargs = callback.func_code.co_argcount
	  else:
	    nargs = 2
	  if nargs == 0:
	    callback()
	  elif nargs == 1:
	    callback(content)
	  else:
	    callback(id,content)
	finally:
	  self._currentResponse = None
    else:
      self.ignoreResponse(id)

  def _handleFailure(self,failure):
    try:
      self.handleFailure(_RemoteFailure(failure))
    except SystemExit:
      terminate()
    except:
      self.unexpectedException()

  def handleFailure(self,failure):
    id = failure.conversationId()
    if self._failureCallbacks.has_key(id):
      callback,binding = self._failureCallbacks[id]
      self.ignoreResponse(id)
      self._currentFailure = failure
      try:
        if type(callback) == types.MethodType:
          nargs = callback.im_func.func_code.co_argcount-1
        elif type(callback) == types.FunctionType:
          nargs = callback.func_code.co_argcount
        else:
          nargs = 5
	if nargs == 0:
	  callback()
	elif nargs == 1:
	  if self._requestCallbacks.has_key(id):
	    if self._requestCallbacks[id] == callback:
	      # For backwards compatbility.
	      callback(None)
	    else:
	      callback(id)
	  else:
	    callback(id)
	elif nargs == 4:
	  callback(failure.error(),failure.description(),
	      failure.origin(),failure.details())
	else:
	  callback(id,failure.error(),failure.description(),
	      failure.origin(),failure.details())
      finally:
        self._currentFailure = None
    else:
      self.ignoreResponse(id)

  def _handleResponseTimeout(self,timer):
    id = string.splitfields(timer,'/')[1]
    if self._failureCallbacks.has_key(id):
      callback,binding = self._failureCallbacks[id]
      error = CLIENT_REQUEST_TIMEOUT
      description = CLIENT_REQUEST_TIMEOUT_MESSAGE
      origin = "netsvc"
      failure = _RequestTimeout(id,error,description,origin)
      try:
	self.handleFailure(failure)
      except SystemExit:
	terminate()
      except:
	self.unexpectedException()

  def _handleServiceUnavailable(self,subscription):
    id = string.splitfields(subscription,'/')[1]
    if self._failureCallbacks.has_key(id):
      callback,binding = self._failureCallbacks[id]
      self._handleServiceWithdrawn(binding,None,SERVICE_WITHDRAWN,subscription)

  def _handleServiceWithdrawn(self,binding,group,status,subscription):
    id = string.splitfields(subscription,'/')[1]
    if self._failureCallbacks.has_key(id):
      if group == None and status == SERVICE_WITHDRAWN:
	self.unsubscribeServiceAddress(binding.serviceAddress(),subscription)
	callback,binding = self._failureCallbacks[id]
	error = SERVER_APPLICATION_ERROR
	description = SERVER_APPLICATION_ERROR_MESSAGE
	origin = "netsvc"
	details = "Service was withdrawn before response received."
	failure = _ServiceWithdrawn(id,error,description,origin,details)
	try:
	  self.handleFailure(failure)
	except SystemExit:
	  terminate()
	except:
	  self.unexpectedException()
    else:
      subscription = "$SERVICE-STATUS/%s" % id
      self.unsubscribeServiceAddress(binding.serviceAddress(),subscription)

  def _handleSubscription(self,subscription):
    try:
      self.handleSubscription(subscription)
    except SystemExit:
      terminate()
    except:
      self.unexpectedException()

  def handleSubscription(self,subscription):
    return

  def matchSubject(self,subjectPattern,subjectName):
    return self._Agent.matchSubject(subjectPattern,subjectName)

  def subscribeReports(self,target,subjectPattern):
    self._Agent.subscribeReports(target,subjectPattern)

  def unsubscribeReports(self,target,subjectPattern):
    self._Agent.unsubscribeReports(target,subjectPattern)

  def publishReport(self,subjectName,content,lifeTime=0):
    self._Agent.publishReport(subjectName,
        content,lifeTime,self.encodeObject)

  def sendReport(self,agent,subjectName,content):
    return self._Agent.sendReport(agent,
        subjectName,content,self.encodeObject)

  def _handlePublisherNotification(self,notification):
    try:
      self.handlePublisherNotification(notification)
    except SystemExit:
      terminate()
    except:
      self.unexpectedException()

  def handlePublisherNotification(self,notification):
    return

  def monitorReports(self,callback,target,subjectPattern):
    if type(target) is types.StringType:
      subscription = (target,"")
    else:
      subscription = (target.serviceName(),target.serviceAddress())
    if not self._activeSubscriptions.has_key(subscription):
      self._activeSubscriptions[subscription] = {}
    if not self._activeSubscriptions[subscription].has_key(subjectPattern):
      self.subscribeReports(target,subjectPattern)
    self._activeSubscriptions[subscription][subjectPattern] = callback

  def acceptReports(self,callback,target,subjectName):
    if type(target) is types.StringType:
      subscription = (target,"")
    else:
      subscription = (target.serviceName(),target.serviceAddress())
    if not self._passiveSubscriptions.has_key(subscription):
      self._passiveSubscriptions[subscription] = {}
    self._passiveSubscriptions[subscription][subjectName] = callback

  def ignoreReports(self,target,subjectPattern=None):
    if type(target) is types.StringType:
      subscription = (target,"")
    else:
      subscription = (target.serviceName(),target.serviceAddress())
    if self._activeSubscriptions.has_key(subscription):
      if subjectPattern != None:
	if self._activeSubscriptions[subscription].has_key(subjectPattern):
	  del self._activeSubscriptions[subscription][subjectPattern]
	  self.unsubscribeReports(target,subjectPattern)
      else:
	for subjectPattern in self._activeSubscriptions[subscription].keys():
	  self.subscribeReports(target,subjectPattern)
	del self._activeSubscriptions[subscription]
    if self._passiveSubscriptions.has_key(subscription):
      if subjectPattern != None:
	if self._passiveSubscriptions[subscription].has_key(subjectPattern):
	  del self._passiveSubscriptions[subscription][subjectPattern]
      else:
	del self._passiveSubscriptions[subscription]

  def _handleReport(self,report):
    try:
      self.handleReport(_Report(report,self))
    except SystemExit:
      terminate()
    except:
      self.unexpectedException()

  def handleReport(self,report):
    serviceAddress = report.publisher().serviceAddress()
    serviceName = report.publisher().serviceName()
    subjectName = report.subjectName()
    subjectPattern = report.subjectPattern()
    callback = None
    subscription = (serviceName,serviceAddress)
    if callback == None:
      if self._activeSubscriptions.has_key(subscription):
	if self._activeSubscriptions[subscription].has_key(subjectPattern):
	  callback = self._activeSubscriptions[subscription][subjectPattern]
    if callback == None:
      if self._passiveSubscriptions.has_key(subscription):
	if self._passiveSubscriptions[subscription].has_key(subjectName):
	  callback = self._passiveSubscriptions[subscription][subjectName]
    subscription = (serviceName,"")
    if callback == None:
      if self._activeSubscriptions.has_key(subscription):
	if self._activeSubscriptions[subscription].has_key(subjectPattern):
	  callback = self._activeSubscriptions[subscription][subjectPattern]
    if callback == None:
      if self._passiveSubscriptions.has_key(subscription):
	if self._passiveSubscriptions[subscription].has_key(subjectName):
	  callback = self._passiveSubscriptions[subscription][subjectName]
    if callback == None:
      callback = self.unexpectedReport
    try:
      content = report.content()
    except SystemExit:
      terminate()
    except:
      _logger.notify(LOG_ERROR,"Invalid Report")
      _logger.notify(LOG_ERROR,"Service: %s" % `serviceName`)
      _logger.notify(LOG_ERROR,"SubjectName: %s" % `subjectName`)
      logException()
    else:
      self._currentReport = report
      try:
        if type(callback) == types.MethodType:
          nargs = callback.im_func.func_code.co_argcount-1
        elif type(callback) == types.FunctionType:
          nargs = callback.func_code.co_argcount
        else:
          nargs = 3
	if nargs == 1:
	  callback(content)
	elif nargs == 2:
	  callback(subjectName,content)
	else:
	  callback(serviceName,subjectName,content)
	self._currentReport = None
      except SystemExit:
	self._currentReport = None
	terminate()
      except:
	self._currentReport = None
	self.unexpectedException()

  def unexpectedReport(self,serviceName,subjectName,content):
    return

  def expireReports(self,subjectPattern,age=0):
    self._Agent.expireReports(subjectPattern,age)

  def expireAllReports(self,age):
    self._Agent.expireAllReports(age)

  def purgeReports(self):
    self._Agent.purgeReports()


class _LocalMethod:

  def __init__(self,serviceName,methodName):
    self._serviceName = serviceName
    self._methodName = methodName

  def __getattr__(self,methodName):
    return _LocalMethod(self._serviceName,
        "%s.%s"%(self._methodName,methodName))

  def __call__(self,*args):
    Service._servicesLock.acquire()
    if not Service._services.has_key(self._serviceName):
      error = CLIENT_SERVICE_UNAVAILABLE
      description = CLIENT_SERVICE_UNAVAILABLE_MESSAGE
      origin = "netsvc"
      details = self._serviceName
      Service._servicesLock.release()
      raise ServiceUnavailable(error,description,origin,details)
    service = Service._services[self._serviceName][0]
    Service._servicesLock.release()
    return service._localRequest(self._methodName,args)


class LocalService:

  def __init__(self,serviceName):
    self._serviceName = serviceName

  def __getattr__(self,methodName):
    return _LocalMethod(self._serviceName,methodName)

  def __coerce__(self,other):
    if other == None:
      Service._servicesLock.acquire()
      if not Service._services.has_key(self._serviceName):
	Service._servicesLock.release()
	return (None,other)
      Service._servicesLock.release()
    return None

  def __ne__(self,other):
    Service._servicesLock.acquire()
    if other == None:
      Service._servicesLock.release()
      return Service._services.has_key(self._serviceName)
    Service._servicesLock.release()
    return not None

  def __nonzero__(self):
    Service._servicesLock.acquire()
    result = Service._services.has_key(self._serviceName)
    Service._servicesLock.release()
    return result


# Setup constants for exchange.

EXCHANGE_CLIENT = 0
EXCHANGE_SERVER = 1

CONNECTION_ACTIVE = 0
CONNECTION_INACTIVE = 1

# Exchange wrapper.

class Exchange:

  def __init__(self,mode,group=""):
    self._Exchange = _ose.new_Exchange(self,mode,group)

  def connect(self,hostname,port,retryDelay=5.0):
    return self._Exchange.connect(hostname,port,retryDelay)

  def listen(self,port):
    self._Exchange.listen(port)

  def unexpectedException(self):
    logException()

  def _authorise(self,host):
    try:
      result = self.authorise(host)
      if result:
        return not None
      else:
        return None
    except SystemExit:
      terminate()
    except:
      self.unexpectedException()
      return None

  def authorise(self,host):
    return not None

  def _handleConnection(self,announcement):
    try:
      self.handleConnection(announcement)
    except SystemExit:
      terminate()
    except:
      self.unexpectedException()

  def handleConnection(self,announcement):
    return


# HTTP daemon and server wrappers.

CONNECTION_BLOCKED = 0
CONNECTION_CLEARED = 1
CONNECTION_BACKLOG = 2

class HttpDaemon:

  def __init__(self,port):
    self._HttpDaemon = _ose.new_HttpDaemon(self,port)

  def port(self):
    return self._HttpDaemon.port()

  def unexpectedException(self):
    logException()

  def _authorise(self,host):
    try:
      result = self.authorise(host)
      if result:
        return not None
      return None
    except SystemExit:
      terminate()
    except:
      self.unexpectedException()
      return None

  def authorise(self,host):
    return not None

  def start(self):
    self._HttpDaemon.start()

  def stop(self):
    self._HttpDaemon.stop()

  def attach(self,path,server):
    if server != None:
      self._HttpDaemon.attach(path,server._HttpServer.instance())
    else:
      self._HttpDaemon.attach(path,None)


class HttpServlet:

  _bufferedMaximum = 4096

  def __init__(self,session):
    self._HttpServlet = _ose.new_HttpServlet(self,session.instance())
    self.contentLength = self._HttpServlet.contentLength
    self.contentType = self._HttpServlet.contentType
    self.requestMethod = self._HttpServlet.requestMethod
    self.url = self._HttpServlet.url
    self.protocolVersion = self._HttpServlet.protocolVersion
    self.serverHost = self._HttpServlet.serverHost
    self.serverPort = self._HttpServlet.serverPort
    self.requestPath = self._HttpServlet.requestPath
    self.queryString = self._HttpServlet.queryString
    self.clientHost = self._HttpServlet.clientHost
    self.clientPort = self._HttpServlet.clientPort
    self.remoteUser = self._HttpServlet.remoteUser
    self.headers = self._HttpServlet.headers
    self.containsHeader = self._HttpServlet.containsHeader
    self.header = self._HttpServlet.header
    self.serverRoot = self._HttpServlet.serverRoot
    self.serverPath = self._HttpServlet.serverPath
    self.servletPath = self._HttpServlet.servletPath
    self.servletInfo = self._HttpServlet.servletInfo
    self.sessionId = self._HttpServlet.sessionId
    self.sessionContext = self._HttpServlet.sessionContext
    self.sendResponse = self._HttpServlet.sendResponse
    self.sendHeader = self._HttpServlet.sendHeader
    self.sendError = self._HttpServlet.sendError
    self.sendComplete = self._HttpServlet.sendComplete
    self.shutdown = self._HttpServlet.shutdown
    self.statusText = self._HttpServlet.statusText
    self.subscribeCongestion = self._HttpServlet.subscribeCongestion
    self.unsubscribeCongestion = self._HttpServlet.unsubscribeCongestion
    self._congestionCallback = None
    self._bufferedOutput = []
    self._bufferedLength = 0
    self._bufferedMaximum = HttpServlet._bufferedMaximum

  def unexpectedException(self):
    logException()

  def _destroyServlet(self):
    try:
      self.destroyServlet()
    except SystemExit:
      terminate()
    except:
      self.unexpectedException()

  def destroyServlet(self):
    self._congestionCallback = None
    self.unsubscribeCongestion()

  def monitorCongestion(self,callback):
    if self._congestionCallback == None:
      self.subscribeCongestion()
    self._congestionCallback = callback

  def ignoreCongestion(self):
    self._congestionCallback = None
    self.unsubscribeCongestion()

  def _handleCongestion(self,status,pending):
    try:
      self.handleCongestion(status,pending)
    except SystemExit:
      terminate()
    except:
      self.unexpectedException()
      self.shutdown()

  def handleCongestion(self,status,pending):
    if self._congestionCallback != None:
      self._congestionCallback(status,pending)

  def _processRequest(self):
    try:
      self.processRequest()
    except SystemExit:
      terminate()
    except:
      self.unexpectedException()
      self.shutdown()

  def processRequest(self):
    self.sendError(501)

  def _processContent(self,content):
    try:
      self.processContent(content)
    except SystemExit:
      terminate()
    except:
      self.unexpectedException()
      self.shutdown()

  def processContent(self,content):
    pass

  def endHeaders(self):
    self._HttpServlet.endHeaders()
    self._bufferedOutput = []
    self._bufferedLength = 0

  def sendContent(self,content):
    self._bufferedOutput.append(content)
    self._bufferedLength = self._bufferedLength + len(content)
    if self._bufferedLength > self._bufferedMaximum:
      self.flushContent()

  def flushContent(self):
    if self._bufferedLength != 0:
      self._bufferedOutput = string.join(self._bufferedOutput,"")
      self._HttpServlet.sendContent(self._bufferedOutput)
      self._bufferedOutput = []
      self._bufferedLength = 0

  def endContent(self):
    self.flushContent()
    self._HttpServlet.endContent()


class HttpServer:

  def __init__(self,inner=None):
    if inner == None:
      inner =  _ose.new_HttpServer(self)
    self._HttpServer = inner

  def unexpectedException(self):
    logException()

  def _authorise(self,login,password):
    try:
      result = self.authorise(login,password)
      if result:
        return not None
      return None
    except SystemExit:
      terminate()
    except:
      self.unexpectedException()
      return None

  def authorise(self,login,password):
    return not None

  def _servlet(self,session):
    try:
      result = self.servlet(session)
      if result == None:
        return None
      return result._HttpServlet.instance()
    except SystemExit:
      terminate()
    except:
      self.unexpectedException()
      result = ErrorServlet(session,500)
      return result._HttpServlet.instance()

  def servlet(self,session):
    return None


class ErrorServlet(HttpServlet):

  def __init__(self,session,error,content=None):
    HttpServlet.__init__(self,session)
    self._error = error
    self._content = content

  def processRequest(self):
    self.sendError(self._error,self._content)


class EchoServlet:

  def __init__(self,session):
    self._HttpServlet = _ose.new_EchoServlet(self,session.instance())

  def _destroyServlet(self):
    self._HttpServlet = None


REDIRECT_PERMANENT = 301
REDIRECT_TEMPORARY = 302

class RedirectServlet(HttpServlet):

  def __init__(self,session,location,error=REDIRECT_TEMPORARY):
    HttpServlet.__init__(self,session)
    if location[0] == "/":
      if session.serverPort() != "80":
        location = "http://%s:%s%s" % (session.serverHost(),
	    session.serverPort(),location)
      else:
        location = "http://%s%s" % (session.serverHost(),
	    location)
    self._location = location
    self._error = error

  def processRequest(self):
    self.sendResponse(self._error)
    self.sendHeader("Location",self._location)
    self.endHeaders()
    self.endContent()


class RedirectServer(HttpServer):

  def __init__(self,location,error=REDIRECT_PERMANENT):
    HttpServer.__init__(self)
    self._location = location
    self._error = error

  def servlet(self,session):
    location = string.split(session.url(),'?')
    if session.servletPath() != "":
      location[0] = posixpath.join(self._location,session.servletPath())
    else:
      location[0] = self._location
    location = string.join(location,'?')
    if location[0] == '/':
      if session.serverPort() != "80":
        location = "http://%s:%s%s" % (session.serverHost(),
            session.serverPort(),location)
      else:
        location = "http://%s%s" % (session.serverHost(),location)
    return RedirectServlet(session,location,self._error)


class FileServlet:

  def __init__(self,session,fileName,fileType=""):
    self._HttpServlet = _ose.new_FileServlet(self,
        session.instance(),fileName,fileType)

  def _destroyServlet(self):
    self._HttpServlet = None


class FilePlugin:

  def __init__(self):
    self._mimeTypes = {}

  def _mimeType(self,extension):
    if self._mimeTypes.has_key(extension):
      return self._mimeTypes[extension]
    import mimetypes
    mimeType = mimetypes.guess_type(extension)
    if mimeType != None:
      return mimeType[0]

  def map(self,extension,type):
    self._mimeTypes[extension] = type

  def __call__(self,session,path):
    extension = os.path.splitext(path)[1]
    type = self._mimeType(extension)
    return FileServlet(session,path,type)


class FileServer(HttpServer):

  def __init__(self,directory,default=None):
    HttpServer.__init__(self)
    self._directory = directory
    self._plugins = {}
    self._views = {}
    self._hidden = {}
    self._index = []
    self._default = default
    if not self._default:
      self._default = FilePlugin()

  def directory(self):
    return self._directory

  def map(self,extension,type):
    self._default.map(extension,type)

  def plugin(self,extension,factory,alias=None):
    if alias != None and alias != extension:
      partial,suffix = posixpath.splitext(alias)
      if not self._views.has_key(suffix):
	self._views[suffix] = []
      self._views[suffix].insert(0,(partial,extension,factory))
    else:
      self._plugins[extension] = factory

  def hide(self,extension):
    self._hidden[extension] = 1

  def index(self,index):
    self._index.append(index)

  def servlet(self,session):
    instance = self._search(session,session.servletPath())
    if instance != None:
      return instance
    return ErrorServlet(session,404)

  def _search(self,session,servletPath,servletInfo=""):
    if servletPath == "":
      # must check if request path has trailing slash
      if posixpath.split(session.requestPath())[1] == "":
	path = self._directory + "/"
      else:
	path = self._directory
    else:
      path = posixpath.join(self._directory,servletPath)
    # handle directory request
    if os.path.isdir(path):
      # directory path must be exact
      if servletInfo != "":
        return None
      # directory needs a trailing slash
      if path[-1:] != '/':
	location = string.split(session.url(),'?')
	location[0] = location[0] + '/'
	location = string.join(location,'?')
	return RedirectServlet(session,location)
      else:
        # directory diverted to index file
        for file in self._index:
	  index = path + file
	  if not os.path.isdir(index):
	    instance = self._search(session,servletPath+file,servletInfo)
	    if instance != None:
	      return instance
	  else:
	    return ErrorServlet(session,403)
    # look for plugin through a view
    servletStub,suffix = posixpath.splitext(servletPath)
    if self._views.has_key(suffix):
      for partial,extension,factory in self._views[suffix]:
	actualStub = servletStub
	stub = posixpath.basename(servletStub)
	if len(partial) != 0:
	  if stub[-len(partial):] == partial:
	    actualStub = servletStub[:-len(partial)]
	  else:
	    continue
	view = "%s%s" % (posixpath.join(self._directory,actualStub),extension)
	if os.path.exists(view) and os.path.isfile(view):
	  if factory == None:
	    if self._plugins.has_key(extension):
	      if self._hidden.has_key(suffix):
		continue
	      factory = self._plugins[extension]
          if factory == None:
	    continue
	  session.bindServlet(servletPath,servletInfo)
	  return self._create(factory,session,path)
    # handle file request
    if os.path.exists(path):
      extension = posixpath.splitext(path)[1]
      # file type cannot be hidden
      if not self._hidden.has_key(extension):
	# file type managed by a plugin
	if self._plugins.has_key(extension):
	  factory = self._plugins[extension]
	  session.bindServlet(servletPath,servletInfo)
	  return self._create(factory,session,path)
	# file path must be exact
	if servletInfo != "":
	  return None
	return self._create(self._default,session,path)
    # consider request has extra path info
    if servletPath != "":
      split = posixpath.split(servletPath)
      if servletInfo != "":
        servletInfo = posixpath.join(split[1],servletInfo)
      else:
        servletInfo = split[1]
      servletPath = split[0]
      if servletPath != "":
	return self._search(session,servletPath,servletInfo)
    return None

  def _create(self,factory,session,path):
    # ensure path is normalised
    path = os.path.normpath(path)
    return factory(session,path)

class PythonPlugin:

  def __call__(self,session,path):
    return self.execute(session,path)

  def execute(self,session,path):
    try:
      servlet = self.lookup(path)
      return self.create(servlet,session,path)
    except:
      logException()
      return ErrorServlet(session,500)

  def lookup(self,path):
    path = os.path.normpath(path)
    directory = os.path.dirname(path)
    name = os.path.splitext(os.path.basename(path))[0]
    module = _moduleCache.importModule(name,directory)
    return module.__servlet__

  def create(self,servlet,session,path):
    return servlet(session)


class FormServlet(HttpServlet):

  def __init__(self,session):
    HttpServlet.__init__(self,session)
    self._content = []
    self._contentLength = 0
    self._fields = {}
    global cgi,StringIO
    import cgi,StringIO

  def processRequest(self):
    if self.requestMethod() not in ["GET","POST"]:
      self.sendError(501,"Request method type is not supported.")
    elif self.requestMethod() == "POST" and self.contentLength() < 0:
      self.sendError(400,"Content length required for POST.")
    elif self.requestMethod() == "GET" and self.contentLength() > 0:
      self.sendError(400,"Unexpected content length for GET.")
    elif self.requestMethod() == "GET":
      environ = {}
      environ["REQUEST_METHOD"] = self.requestMethod()
      environ["QUERY_STRING"] = self.queryString()
      headers = self.headers()
      headers["content-type"] = "application/x-www-form-urlencoded"
      try:
	form = cgi.FieldStorage(headers=headers,environ=environ,
	    keep_blank_values=1)
	self._handleRequest(form)
      except:
        logException()
        self.shutdown()
    elif self.requestMethod() == "POST" and self.contentLength() == 0:
      environ = {}
      environ["REQUEST_METHOD"] = self.requestMethod()
      fp = StringIO.StringIO("")
      headers = self.headers()
      try:
	form = cgi.FieldStorage(headers=headers,environ=environ,
	    keep_blank_values=1,fp=fp)
	self._handleRequest(form)
      except:
        logException()
        self.shutdown()

  def processContent(self,content):
    self._content.append(content)
    self._contentLength = self._contentLength + len(content)
    if self._contentLength >= self.contentLength():
      environ = {}
      environ["REQUEST_METHOD"] = self.requestMethod()
      self._content = string.join(self._content,"")
      self._content = self._content[:self.contentLength()]
      fp = StringIO.StringIO(self._content)
      headers = self.headers()
      try:
	form = cgi.FieldStorage(headers=headers,environ=environ,
	    keep_blank_values=1,fp=fp)
	self._handleRequest(form)
      except:
        logException()
        self.shutdown()

  def _handleRequest(self,form):
    self._fields = {}
    try:
      keys = form.keys()
    except TypeError:
      keys = []
    for key in keys:
      value = form[key]
      if type(value) is not types.ListType:
	value = value.value
      else:
	value = map(lambda x: x.value, value)
      self._fields[key] = value
    self.handleRequest()

  def containsField(self,name):
    return self._fields.has_key(name)

  def field(self,name):
    return self._fields[name]

  def fields(self):
    return self._fields

  def handleRequest(self):
    self.sendResponse(501)


class RpcServlet:

  def __init__(self,session,binding):
    self._HttpServlet = _ose.new_RpcServlet(self,
        session.instance(),binding)

  def _destroyServlet(self):
    self._HttpServlet = None


class RpcGateway(HttpServer):

  def __init__(self,selection,type=SERVICE_GROUP):
    inner = _ose.new_RpcGateway(self,selection,type)
    HttpServer.__init__(self,inner)


class HttpBridge(HttpServer,Monitor):

  def __init__(self,selection,type=SERVICE_GROUP):
    HttpServer.__init__(self)
    Monitor.__init__(self)
    self._members = {}
    self._bindings = {}
    self._selection = selection
    self._type = type
    if type == SERVICE_REGISTRY:
      self.subscribeRegistry(self.register)
    elif type == SERVICE_GROUP:
      self.subscribeServiceGroup(self.register,selection)
    elif type == SERVICE_NAME:
      self.subscribeServiceName(self.register,selection)
    elif type == SERVICE_ADDRESS:
      self.subscribeServiceAddress(self.register,selection)

  def register(self,binding,group,status):
    if self._type == SERVICE_GROUP or group == None:
      serviceName = binding.serviceName()
      agentIdentity = binding.agentIdentity()
      processAddress = binding.processAddress()
      serviceAudience = binding.serviceAudience()
      if self._type != SERVICE_REGISTRY or self._selection == "*" \
       or (self._selection == "" and processAddress == "") \
       or (self._selection != "" and self._selection == serviceAudience):
	if status == SERVICE_AVAILABLE:
	  if not self._bindings.has_key(agentIdentity):
	    self._bindings[agentIdentity] = binding
	    if self._members.has_key(serviceName):
	      self._members[serviceName].append(agentIdentity)
	    else:
	      self._members[serviceName] = [agentIdentity]
	else:
	  if self._bindings.has_key(agentIdentity):
	    del self._bindings[agentIdentity]
	    members = self._members[serviceName]
	    del members[members.index(agentIdentity)]
	    if len(members) == 0:
	      del self._members[serviceName]

  def servlet(self,session):
    serviceName = session.servletPath()
    if self._type == SERVICE_NAME or \
     self._type == SERVICE_ADDRESS:
        if serviceName == "" and len(self._members) != 0:
	  agentIdentity = self._members.values()[0][0]
	  binding = self._bindings[agentIdentity]
	  return self.create(session,binding)
    else:
	if self._members.has_key(serviceName):
	  agentIdentity = self._members[serviceName][0]
	  binding = self._bindings[agentIdentity]
	  return self.create(session,binding)
    return ErrorServlet(session,404)

  def create(self,session,binding):
    return EchoServlet(session)
