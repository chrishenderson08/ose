from mod_python import apache

import os
import time
import string

from _mputils import markup

cache = markup.TemplateCache()


__handle__ = [ ".txt", ".html" ]


# Handler for ".txt".

def handler_txt(req):

  # Convert CSV file to tab separated data.

  req.content_type = "text/plain"
  req.send_http_header()

  name = os.path.splitext(req.filename)[0] + ".csv"
  file = open(name,'r')

  for line in file.xreadlines():
    fields = string.split(line,',')
    req.write(string.join(fields,'\t'))

  file.close()

  return apache.OK


# Handler for ".html".

def handler_html(req,title="Title"):

  # Load the page template.

  file = os.path.dirname(__file__) + "/table.html"
  template = cache.loadTemplate(file)

  if not os.path.exists(file):
    return apache.DECLINED

  # Fill in the page content.

  template.headTitle.content = title
  template.bodyTitle.content = title

  name = os.path.splitext(req.filename)[0] + ".csv"

  mtime = os.path.getmtime(name)
  mtime = time.localtime(mtime)
  value = time.strftime("Updated: %a %b %e %H:%M:%S %Y",mtime)
  template.generated.content = value

  file = open(name,'r')

  line = file.readline()
  line = string.rstrip(line)
  fields = string.split(line,',')

  def renderField(node,value):
    node.field.content = value

  template.header.fields.repeat(renderField,fields)

  def renderRow(node,line):
    line = string.rstrip(line)
    fields = string.split(line,',')
    node.fields.repeat(renderField,fields)

  template.row.repeat(renderRow,file.xreadlines())

  file.close()

  # Return the rendered page content.

  req.content_type = "text/html"
  req.send_http_header()
  req.write(template.render())

  return apache.OK
