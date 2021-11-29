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
set(SOCKET_BASE_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/base/socket.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/base/socket.cpp
)

# TCP Socket base source
set(TCP_BASE_SRC
    ${SOCKET_BASE_SRC}
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/base/tcpsocket.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/base/tcpsocket.cpp
)

# UDP Socket base source
set(UDP_BASE_SRC
    ${SOCKET_BASE_SRC}
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/base/udpsocket.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/base/udpsocket.cpp
)

# SSL Socket base source
set(UDP_BASE_SRC
    ${SOCKET_BASE_SRC}
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/base/sslsocket.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/base/sslsocket.cpp
)

# TCP Server/Client source
set(TCP_SC_SRC
    ${SOCKET_BASE_SRC}
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/tcpserver.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/tcpserver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/tcpclient.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/tcpclient.cpp
)

# UDP Server/Client source
set(UDP_SC_SRC
    ${UDP_BASE_SRC}
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/udpserver.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/udpserver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/udpclient.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/udpclient.cpp
)

# SSL Server/Client source
set(SSL_SC_SRC
    ${SSL_BASE_SRC}
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/sslserver.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/sslserver.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/../src/sslclient.h
    #${CMAKE_CURRENT_SOURCE_DIR}/../src/sslclient.cpp
)

# static library set
set(-DBUILD_SHARED_LIBS OFF)

# set lib directory
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../lib)

# set tcp-socket library result
add_library(
    opensocket
    STATIC
    ${TCP_SC_SRC}
    ${UDP_SC_SRC}
    ${SSL_SC_SRC}
)
set_target_properties(
    opensocket
    PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR}
    OUTPUT_NAME opensocket
)
