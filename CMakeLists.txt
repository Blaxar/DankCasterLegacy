cmake_minimum_required(VERSION 2.6)
 
project(DankCaster)
 
#set(CMAKE_DEBUG_POSTFIX "_d")
 
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/bin")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
"${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(Threads REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)

find_package(SDL2 REQUIRED)
find_package(CUnit)

set(DEFAULT_SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(DEFAULT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(DEFAULT_LINK_DIR ${CMAKE_SOURCE_DIR}/lib)
set(DEFAULT_BIN_DIR ${CMAKE_SOURCE_DIR}/bin)

message(STATUS ${GSTREAMER_INCLUDE_DIRS})

#Define all the include directories
include_directories(
    ${DEFAULT_INCLUDE_DIR}
	${GSTREAMER_INCLUDE_DIRS}
	${SDL2_INCLUDE_DIR}
)

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-rdynamic -O0)
ENDIF(CMAKE_BUILD_TYPE MATCHES Debug)

IF(CMAKE_BUILD_TYPE MATCHES Release)
	add_definitions(-rdynamic -O3)
ENDIF(CMAKE_BUILD_TYPE MATCHES Release)

link_directories(${DEFAULT_LINK_DIR})
add_subdirectory(src)
