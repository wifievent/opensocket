# Example CMake file for compiling & linking a project
#
# Copyright (c) 2012-2020 cpro95 (cpro95@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt or
# copy at http://opensource.org/licenses/MIT)

# for "CMAKE_CXX_STANDARD" version
cmake_minimum_required(VERSION 3.1)
project(UdpMessenger)

# set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE Debug)

# for C++11 compiler
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set the path to the client source code
set(CLIENT_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/udpmsgclient.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/udpmsgclient.cpp
)

# set the path to the server source code
set(SERVER_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/udpmsgserver.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/udpmsgserver.cpp
)

# set execute file result
add_executable(
    udpmsgclient
    ${CLIENT_DIR}
)

add_executable(
    udpmsgserver
    ${SERVER_DIR}
)

# link opensocket library
target_include_directories(udpmsgclient
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../src/
)

target_include_directories(udpmsgserver
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../src/
)

# glog library link
find_package (glog REQUIRED)

target_link_libraries(udpmsgclient
    PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}/../../../../../lib/libopensocket.a
    PUBLIC
    glog::glog
)

target_link_libraries(udpmsgserver
    PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}/../../../../../lib/libopensocket.a
    PUBLIC
    glog::glog
)

# set the path to the executable
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/../bin)

set_target_properties(
    udpmsgclient
    PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
    OUTPUT_NAME udpmsgclient
)

set_target_properties(
    udpmsgserver
    PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
    OUTPUT_NAME udpmsgserver
)
