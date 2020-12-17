#-------------------------------------------------------------------------------
# Copyright (C) 2020 Leonid Kuzin
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#-------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.5)

project(obs-remote VERSION 0.0.1)
project(obs-remote LANGUAGES CXX C)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(cmake/findavahi2.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive -D_REENTRANT")

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "..." FORCE)
endif()

set(CPP_STD "17")
if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang" OR ${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
  include(CheckCXXCompilerFlag)
  CHECK_CXX_COMPILER_FLAG("-std=c++${CPP_STD}" COMPILER_SUPPORTS_CXX${CPP_STD})
  if(COMPILER_SUPPORTS_CXX${CPP_STD})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++${CPP_STD}")
  else()
    message(FATAL_ERROR "ERROR: The compiler ${CMAKE_CXX_COMPILER} has no C++${CPP_STD} support.\n"
                        "Please use a different C++ compiler.")
  endif()
endif()

add_definitions(${QT_DEFINITIONS})

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

find_package(LibObs 26.0.2 REQUIRED)
find_package(Qt5 5.15.2 REQUIRED COMPONENTS Core Widgets)
find_package(gRPC 1.33.2 REQUIRED)


set(OBS_REMOTE_SOURCES
	src/obsremote.cpp
	src/errorhandler.cpp
	src/zeroconfdnssd.cpp
	src/ui/obsremotesettings.cpp
)

set(OBS_REMOTE_HEADERS
	src/obsremote.hpp
	src/errorhandler.hpp
	src/zeroconfdnssd.hpp
	src/ui/obsremotesettings.hpp
)

add_library(obs-remote MODULE
    ${OBS_REMOTE_SOURCES}
    ${OBS_REMOTE_HEADERS}
)

target_link_libraries(obs-remote
    libobs
    Qt5::Widgets
    Qt5::Core
)

if(UNIX AND NOT APPLE)
	include(GNUInstallDirs)

	set_target_properties(obs-remote PROPERTIES PREFIX "")
	target_link_libraries(obs-remote obs-frontend-api)

	file(GLOB locale_files data/locale/*.ini)
	install(TARGETS obs-remote
		LIBRARY DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}/obs-plugins")

	install(FILES ${locale_files}
		DESTINATION "${CMAKE_INSTALL_FULL_DATAROOTDIR}/obs/obs-plugins/obs-websocket/locale")
endif()
