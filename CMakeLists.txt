cmake_minimum_required(VERSION 3.5)

project (obs-remote VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(LibObs REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Core Widgets)

set(obs-remote_SOURCES
		src/obs-remote.cpp
	)

add_library(obs-remote MODULE
		${obs-remote_SOURCES}
	)

target_link_libraries(obs-remote
		libobs
	)
