# COPYRIGHT 2001-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

import imp
import md5
import base64
import os
import time
import string
import types


class _ModuleInfo:

  def __init__(self,name,label,file,mtime):
    self.name = name
    self.label = label
    self.file = file
    self.mtime = mtime
    self.module = None
    self.generation = None
    self.children = []
    self.atime = None
    self.direct = 0
    self.indirect = 0
    self.stub,self.extn = os.path.splitext(file)

class _ModuleCache:

  _prefix = "_cache_"

  def __init__(self):
    self._cache = {}
    self._generation = 0

  def cachedModules(self):
    return self._cache.keys()

  def moduleInfo(self,label):
    return self._cache[label]

  def unloadModule(self,label):
    if self._cache.has_key(label):
      del self._cache[label]

  def purgeDefunct(self):
    for label in self.cachedModules():
      cache = self.moduleInfo(label)
      if not os.path.exists(cache.file):
        self.unloadModule(label)

  def importModule(self,name,path):
    try:
      fp = None
      path = os.path.normpath(path)
      fp,file,info = imp.find_module(name,[path])
      label = self._moduleLabel(file)
      if self._requiresUpdate(name,label,file):
        cache = self._cache[label]
	module = imp.load_module(label,fp,file,info)
	cache.module = module
	self._generation = self._generation + 1
	cache.generation = self._generation
	cache.atime = time.time()
	cache.direct = 1
	cache.indirect = 0
	children = []
	for object in module.__dict__.values():
	  if type(object) == types.ModuleType:
	    if object.__name__[:len(self._prefix)] == self._prefix:
	      children.append(object.__name__)
	cache.children = children
      else:
        cache = self._cache[label]
	cache.direct = cache.direct + 1
	cache.atime = time.time()
	module = cache.module
    finally:
      if fp != None:
        fp.close()
    return module

  def _requiresUpdate(self,name,label,file):
    # Check if this is a new module.
    if not self._cache.has_key(label):
      mtime = os.path.getmtime(file)
      self._cache[label] = _ModuleInfo(name,label,file,mtime)
      return not None

    # Grab entry from cache.
    cache = self._cache[label]

    # Check if extension has changed.
    extn = os.path.splitext(file)[1]
    if extn != cache.extn:
      cache.extn = extn
      cache.file = file
      cache.mtime = os.path.getmtime(file)
      return not None

    # Has modification time changed.
    try:
      mtime = os.path.getmtime(file)
    except:
      # Must have been removed just then.
      del self._cache[label]
      raise
    if mtime != cache.mtime:
      cache.mtime = mtime
      return not None

    # Check if children have changed or have
    # been reloaded since module last used.
    if cache.children != []:
      atime = time.time()
      dependencies = []
      visited = { label: 1 }
      dependencies.extend(cache.children)
      while len(dependencies) != 0:
        next = dependencies.pop()
	if not visited.has_key(next):
	  if self._cache.has_key(next):
	    temp = self._cache[next]
	    temp.indirect = temp.indirect + 1
	    temp.atime = atime

	    # Child has been reloaded.
	    if temp.generation > cache.generation:
	      return not None

	    # Check if extension may have changed.
	    if temp.extn != ".py":
	      if os.path.exists(temp.stub+".py"):
		return not None

	    try:
	      mtime = os.path.getmtime(temp.file)
	    except:
	      # Purge if can't check it.
	      del self._cache[next]
	      return not None

	    # Child has been modified.
	    if mtime != temp.mtime:
	      return not None

	    dependencies.extend(temp.children)
	  else:
	    return not None

	  visited[next] = 1

    return None

  def _moduleLabel(self,file):
    # This is overkill, but for some reason that
    # I have forgotten, using long module names
    # was giving problems. Alternatively, I may
    # have cosmetically wanted the encoded module
    # names not to be too long. We need to at
    # least get rid of non alphanumeric characters
    # and also probably ensure no leading numbers
    # in the generated name.
    stub = os.path.splitext(file)[0]
    label = md5.new(stub).digest()
    label = base64.encodestring(label)
    label = string.replace(label," ","")
    label = string.replace(label,"=","")
    label = string.replace(label,"/","_2b_")
    label = string.replace(label,"+","_2f_")
    label = string.replace(label,"\r","")
    label = string.replace(label,"\n","")
    label = self._prefix + label
    label = label + "_" + str(len(stub))
    return label


_moduleCache = _ModuleCache()

def ModuleCache():
  return _moduleCache
