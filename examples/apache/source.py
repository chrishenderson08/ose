from mod_python import apache

import os

import _mputils.markup

cache = _mputils.markup.TemplateCache()


__handle__ = [ "", ".html" ]


# Handler for ".html".

def handler_html(req):

  # Block access to HTML file we wrap source into.

  return apache.HTTP_FORBIDDEN


# Default handler.

def handler(req):

  # Determine name of source file to view.

  if req.path_info == "":
    return apache.HTTP_BAD_REQUEST
  elif req.path_info == "/":
    return apache.HTTP_BAD_REQUEST
  else:
    name = req.path_info[1:]

  # Don't bother with files that don't exist.

  root = os.path.dirname(req.filename)
  path = os.path.join(root,name)

  if not os.path.exists(path):
    return apache.HTTP_NOT_FOUND

  # Block access to files we don't want visible.

  extn = os.path.splitext(name)[1]

  if not extn in [ ".py", ".html" ]:
    return apache.HTTP_FORBIDDEN

  # Load the page template.

  page = req.filename + ".html"

  if not os.path.exists(page):
    return apache.HTTP_INTERNAL_SERVER_ERROR

  template = cache.loadTemplate(page)

  # Fill in the page content.

  template.name.content = name

  file = open(path,'r')
  template.source.content = file.read()
  file.close()

  # Return the rendered page content.

  req.content_type = "text/html"
  req.write(template.render())

  return apache.OK
