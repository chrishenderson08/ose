#!/usr/bin/env python

# The source code for the Python wrappers encapsulating aspects of the
# functionality provided by the C++ library and which are contained within
# the "python", "netrpc", "netsvc" and "zsirpc" directories, are made
# available under the terms of a BSD style license. Details of this license
# are listed below.

# Copyright 2003-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
#
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# 
#   Redistributions of source code must retain the above copyright notice,
#   this list of conditions and the following disclaimer.
# 
#   Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
# 
#   Neither the name of the Dumpleton Software Consulting Pty Limited nor the
#   names of its contributors may be used to endorse or promote products
#   derived from this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

# This is a standard Python distutils "setup.py" file. If you are not
# familiar with these files, try running:
#
#   python setup.py build
#   python setup.py install
#
# If you are only interested in installing the "netrpc" and "zsirpc"
# modules, rename the "netsvc" directory to "netsvc-uninstalled" before
# attempting installation. Doing this will prevent any attempt to
# build the C++ code into a loadable module.
#
# If you do want the "netsvc" module, unless you are using Microsoft C++
# on a native Win32 system, you must first run the "configure" script.
# If using Microsoft C++, you must have available a POSIX thread layer
# library available. One such Win32 POSIX thread library can be obtained
# from [http://sources.redhat.com/pthreads-win32]. You will need to tweak
# this file to define the location you have installed this library. See
# the "Win32" related code sections further down in this file.
#
# If you aren't using a recent version of Python, you may have to tweak
# this file to override what program is used when the dynamically loadable
# module is created. See the code related to "LDSHARED" further down in
# this file. The dynamically loadable module must be created using a C++
# compiler or other tool which will ensure that correct runtime C++
# initialisation occurs.
#
# In practice the "distutils" modules still isn't well setup for C++ code
# on UNIX and only GNU C/C++ may actually work. This is because the C
# compiler is used to compile all files even though they may equate to
# C++ code. The C++ compiler is only used for the creation of the loadable
# module. Thus whatever is defined for the C compiler must be able to
# recognise what file extensions equate to C++ code and compile them as
# C++ code instead of C code. The GNU C compiler will do this, but other
# C compilers may not. Because a mix of C and C++ code is used, you cannot
# simply make it compile all files as C++ code.
#
# If you are using a recent version of Python and need to override which
# C and C++ compilers are used, the CC and CXX environment variables should
# be set. For example, on Mac OS X, the default C and C++ compiler is
# the GNU compiler version 2.95.3. Under Mac OS X however, this version
# of the GNU compilers doesn't perform correct initialisation of static
# data in a loadable module using C++ code. Specifically, aspects of 
# the C++ streams libraries don't work. Thus on Mac OS X, you would build
# the loadable module by running:
#
#   CC=/usr/bin/gcc-3.3 CXX=/usr/bin/g++-3.3 python build

import sys, os, glob, string

from distutils import core
from distutils import sysconfig

file = open("PACKAGE","r")
value = file.readline()
file.close()

package = ""
version = ""

i = string.index(value,"-")

package = value[:i]
version = value[i+1:-1]

# This file does double duty. That is, the same file is used for the
# installation of the full Python wrappers for OSE, as well as the
# case where the "netrpc" module is distributed as a distinct package.
# The test checks for the existance of the "netsvc" directory. Thus,
# if you have the full OSE distribution but only want the "netrpc"
# modules, rename the "netsvc" directory before using this setup script
# to perform an install.

if not os.access("netsvc",os.F_OK):

  # The first part file deals with installing just the "netrpc" module

  core.setup(
   name="netrpc",
   version=version,
   packages=[ "netrpc", "zsirpc" ],
   description="Python Modules For RPC Over HTTP Protocols.",
   author="Graham Dumpleton",
   author_email="grahamd@dscpl.com.au",
   url="http://ose.sourceforge.net"
  )

else:

  # This second part of the module deals with building the full wrappers.
  # Note that on UNIX like platforms the "configure" script needs to
  # have been run before trying to use "setup.py" and Python to build
  # the package. Note that if using native Win32 Microsoft C++ compiler,
  # it is assumed that "include/OSE/version.h" has been shipped with
  # the source code in generated form appropriate for version.

  sources = []

  sources.extend(glob.glob("python/*.cc"))

  sources.extend(glob.glob("library/OTC/collctn/otc*.cc"))
  sources.extend(glob.glob("library/OTC/debug/otc*.cc"))
  sources.extend(glob.glob("library/OTC/dispatch/otc*.cc"))
  sources.extend(glob.glob("library/OTC/internet/otc*.cc"))
  sources.extend(glob.glob("library/OTC/memory/otc*.cc"))
  sources.extend(glob.glob("library/OTC/message/otc*.cc"))
  sources.extend(glob.glob("library/OTC/misc/otc*.cc"))
  sources.extend(glob.glob("library/OTC/refcnt/otc*.cc"))
  sources.extend(glob.glob("library/OTC/system/otc*.cc"))
  sources.extend(glob.glob("library/OTC/text/otc*.cc"))
  sources.extend(glob.glob("library/OTC/thread/otc*.cc"))
  sources.extend(glob.glob("library/OTC/types/otc*.cc"))

  sources.extend(glob.glob("library/OTC/hash/hash_*.c"))
  sources.extend(glob.glob("library/OTC/regex/reg*.c"))

  define_macros=[("OSE_WITH_THREADS",1)]
  #define_macros.extend([("OTCLIB_TRACE",1)])

  include_dirs=["include"]

  library_dirs=[]
  libraries=[]

  # If old version of Python is used, might be necessary to
  # uncomment the following and adjust as necessary to ensure
  # that C++ compiler is used for linking.

  #sysconfig._config_vars["LDSHARED"] = string.replace( \
  # sysconfig.get_config_var("LDSHARED"),"gcc","g++")

  # For native Win32 compiler, must use POSIX pthread library.

  if sys.platform == "win32":
    include_dirs.extend(["c:/posix/include"])
    library_dirs.extend(["c:/posix/lib"])
    libraries.extend(["pthreadVC","ws2_32"])

  module = core.Extension(
   name="_ose",
   sources=sources,
   define_macros=define_macros,
   include_dirs=include_dirs,
   library_dirs=library_dirs,
   libraries=libraries
  )

  packages=['netrpc', 'zsirpc', 'netsvc', 'netsvc.xmlrpc', 'netsvc.soap',
      'netsvc.client', 'netsvc.apache', 'netsvc.cache' ]

  core.setup(
   name="netsvc",
   version=version,
   description="Python Wrappers For OSE C++ Class Library",
   author="Graham Dumpleton",
   author_email="grahamd@dscpl.com.au",
   url="http://ose.sourceforge.net",
   packages=packages,
   ext_modules=[module]
  )
