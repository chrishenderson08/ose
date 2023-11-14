from mod_python import apache

import os
import time

import _mputils.markup

cache = _mputils.markup.TemplateCache()

codehits = 0


# Default handler.

def handler(req):

  # Load the page template.

  if not os.path.exists(req.filename):
    return apache.DECLINED

  template = cache.loadTemplate(req.filename)

  # Fill in the page content.

  global codehits
  codehits = codehits + 1

  pagehits = cache.templateInfo(req.filename).hits

  template.date.content = time.asctime()
  template.htmlfile.content = req.filename
  template.codefile.content = __file__
  template.processid.content = str(os.getpid())
  template.codehits.content = str(codehits)
  template.pagehits.content = str(pagehits)

  # Return the rendered page content.

  req.content_type = "text/html"
  req.send_http_header()
  req.write(template.render())

  return apache.OK
