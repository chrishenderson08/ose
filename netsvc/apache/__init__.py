# COPYRIGHT 2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

from mod_python import apache

import os
import string

from netsvc import cache

_moduleCache = cache.ModuleCache()

# Define the mod_python handler.

def handler(req):

  # Determine type of file based on extension.

  stub,extn = os.path.splitext(req.filename)

  # Hack because indexes sometimes not working.

  if extn == "" and os.path.basename(stub) == "":
    extn = ".html"
    stub = stub + "index"
    file = stub + extn

    if not os.path.exists(file):
      return apache.DECLINED
    else:
      req.filename = file

  # Forbid requests for Python code modules.

  if extn in [".py",".pyc"]:
    if os.path.exists(req.filename):
      return apache.HTTP_FORBIDDEN

  # Check if target file has associated python code.

  code = stub + ".py"
  if not os.path.exists(code):
    return apache.DECLINED

  # Avoid using this code file as the handler.

  if code == __file__:
    return apache.DECLINED

  # Attempt to load Python code module from cache.

  directory = os.path.dirname(stub)
  name = os.path.basename(stub)

  module = _moduleCache.importModule(name,directory)

  # Check if Python code actually handles file type.

  if hasattr(module,"__handle__"):
    if not extn in module.__handle__:
      return apache.DECLINED
  elif extn != ".html":
    return apache.DECLINED

  # Execute the Python code module handler.

  if extn != "":
    method = "handler_%s" % extn[1:]
    if hasattr(module,method):
      return getattr(module,method)(req)

  return module.handler(req)
