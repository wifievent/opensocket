# Example CMake file for compiling & linking a project
#
# Copyright (c) 2012-2020 cpro95 (cpro95@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt or
# copy at http://opensource.org/licenses/MIT)

# for "CMAKE_CXX_STANDARD" version
cmake_minimum_required(VERSION 3.1)
project(socket)

# set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE Debug)

# for C++11 compiler
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external)

# set socket base source
set(TCP_SERVER_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/tcpserver.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/tcpserver.cpp
)
set(TCP_CLIENT_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/tcpclient.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/tcpclient.cpp
)
set(UDP_SERVER_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/udpserver.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/udpserver.cpp
)
set(UDP_CLIENT_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/udpclient.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/udpclient.cpp
)

# static library set
set(-DBUILD_SHARED_LIBS OFF)

# set lib directory
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../lib)

# set tcp-socket library result
add_library(
    tcpserver
    STATIC
    ${TCP_SERVER_SRC}
)
set_target_properties(
    tcpserver
    PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR}
    OUTPUT_NAME tcp-server
)
add_library(
    tcpclient
    STATIC
    ${TCP_CLIENT_SRC}
)
set_target_properties(
    tcpclient
    PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR}
    OUTPUT_NAME tcp-client
)

# set udp-socket library result
add_library(
    udpserver
    STATIC
    ${UDP_SERVER_SRC}
)
set_target_properties(
    udpserver
    PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR}
    OUTPUT_NAME udp-server
)
add_library(
    udpclient
    STATIC
    ${UDP_CLIENT_SRC}
)
set_target_properties(
    udpclient
    PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR}
    OUTPUT_NAME udp-client
)
