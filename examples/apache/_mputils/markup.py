import os
import time

import HTMLTemplate

# Create version of template which splits cloning out from
# rendering so we can manage template caching. The callback
# is also eliminated.

class Template(HTMLTemplate.Template):

    def __init__(self, html, attribute='node',
            codecs=(HTMLTemplate.defaultEncoder, HTMLTemplate.defaultDecoder)):
        HTMLTemplate.Template.__init__(self,None,html,attribute,codecs)

    def render(self):
        """Render this template; *args will be passed directly to the template.
	"""
        collector = []
        self._renderContent(collector)
        try: # quick-n-dirty error reporting; not a real substitute for type-
            # checking for bad value assignments at point of origin, but cheap
            return ''.join(collector)
        except TypeError:
            raise TypeError, "Can't render template: some node's content " \
                    "was set to a non-text value."

# Fix up bug in HTMLTemplate which causes it to die
# for element attributes with no actual value. Only
# needed for HTMLTemplate 1.1.0 or earlier.

HTMLTemplate.xrenderAtts = HTMLTemplate.renderAtts

def renderAtts(atts):
    result = ''
    for name, value in atts:
        if value is not None:
            if '"' in value:
                result += " %s='%s'" % (name, value)
            else:
                result += ' %s="%s"' % (name, value)
        else:
            result += ' %s' % name
    return result

HTMLTemplate.renderAtts = renderAtts

# Template cache. Will automatically reload a file if it
# has changed. This includes its modification time being
# set backwards in time.

class _TemplateInfo:

  def __init__(self,path,mtime,template):
    self.path = path
    self.mtime = mtime
    self.template = template
    self.atime = time.time()
    self.hits = 0

class _TemplateCache:

  def __init__(self):
    self._cache = {}

  def cachedTemplates(self):
    return self._cache.keys()

  def templateInfo(self,path):
    path = os.path.normpath(path)
    return self._cache[path]

  def unloadTemplate(self,path):
    path = os.path.normpath(path)
    if self._cache.has_key(path):
      del self._cache[path]

  def loadTemplate(self,path):
    record = None
    # ensure the path is normalised
    path = os.path.normpath(path)
    # is page template already loaded
    if self._cache.has_key(path):
      record = self._cache[path]
      # has page template been changed
      try:
	mtime = os.path.getmtime(path)
      except:
	# page template must not exist
	del self._cache[path]
	raise
      else:
	if record.mtime != mtime:
	  # force reloading of page template
	  del self._cache[path]
	  record = None
    # need to load the page template
    if record is None:
      file = open(path,"r")
      content = file.read()
      mtime = os.path.getmtime(path)
      template = Template(content)
      record = _TemplateInfo(path,mtime,template)
      file.close()
      self._cache[path] = record
    else:
      template = record.template
    # return clone of page template
    record.hits = record.hits + 1
    record.atime = time.time()
    return template._initRichClone(HTMLTemplate.CloneNode(template))


_templateCache = _TemplateCache()

def TemplateCache():
  return _templateCache
