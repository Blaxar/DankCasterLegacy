cmake_minimum_required(VERSION 2.6)
 
project(DankCaster)
 
#set(CMAKE_DEBUG_POSTFIX "_d")
 
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/bin")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
"${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(Threads REQUIRED)

set(DEFAULT_SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(DEFAULT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(DEFAULT_LINK_DIR ${CMAKE_SOURCE_DIR}/lib)
set(DEFAULT_BIN_DIR ${CMAKE_SOURCE_DIR}/bin)


#Define all the include directories
include_directories(
    ${DEFAULT_INCLUDE_DIR}
)

add_definitions(-rdynamic -O2)

link_directories(${DEFAULT_LINK_DIR})
add_subdirectory(src)
