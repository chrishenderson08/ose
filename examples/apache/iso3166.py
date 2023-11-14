import _mputils.cache

import os

cache = _mputils.cache.ModuleCache()
directory = os.path.dirname(_mputils.__file__)
table = cache.importModule("table",directory)

__handle__ = [ ".txt", ".html" ]

def handler_txt(req):
  return table.handler_txt(req)

def handler_html(req):
  return table.handler_html(req,title="Country Codes")
