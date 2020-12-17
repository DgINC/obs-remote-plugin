# Copyright (c) 2020 <copyright holder> <email>
# 
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use,
# copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following
# conditions:
# 
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
# OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.

# - Try to find Avahi
# Once done this will define
#  AVAHI_FOUND - System has Avahi
#  AVAHI_INCLUDE_DIRS - The Avahi include directories
#  AVAHI_LIBRARIES - The libraries needed to use Avahi
#  AVAHI_DEFINITIONS - Compiler switches required for using Avahi

find_package(PkgConfig)
if(PKG_CONFIG_FOUND)
	pkg_check_modules(PC_AVAHI QUIET avahi-client)
endif()

set(AVAHI_DEFINITIONS ${PC_AVAHI_CFLAGS_OTHER})

find_path(AVAHI_INCLUDE_DIR avahi-client avahi-common
          HINTS ${PC_AVAHI_INCLUDEDIR} ${PC_AVAHI_INCLUDE_DIRS}
          PATH_SUFFIXES avahi )

find_library(AVAHI_LIBRARY NAMES avahi-client avahi-common
             HINTS ${PC_AVAHI_LIBDIR} ${PC_AVAHI_LIBRARY_DIRS} )

set(AVAHI_LIBRARIES ${AVAHI_LIBRARY} )
set(AVAHI_INCLUDE_DIRS ${AVAHI_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set AVAHI_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(Avahi  DEFAULT_MSG
                                  AVAHI_LIBRARY AVAHI_INCLUDE_DIR)
mark_as_advanced(AVAHI_INCLUDE_DIR AVAHI_LIBRARY )
