cmake_minimum_required(VERSION 3.14)
project(msgtrans_client C CXX)

# CMake -DCMAKE_BUILD_TYPE=Debug .
SET(CMAKE_CXX_FLAGS_DEBUG"$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
#SET(CMAKE_CXX_FLAGS_RELEASE"$ENV{CXXFLAGS} -O3 -Wall")

set(CMAKE_CXX_STANDARD 11)

#ios
set(BUILD_SHARED false)
#anroid
#set(BUILD_SHARED true)

set(LIBRARIES evpp_static event pthread protobuf glog )

# Includes
#include_directories (
#        ${imsdk_SOURCE_DIR}
#        #./include/mac
#        #  ./include/ios
#
#)

set(PRO_INCLUDE_DIR1 ./include)
set(PRO_INCLUDE_DIR2 ./include/common)
set(PRO_INCLUDE_DIR3 ./include/channel)

include_directories(${PRO_INCLUDE_DIR1})
include_directories(${PRO_INCLUDE_DIR2})
include_directories(${PRO_INCLUDE_DIR3})

LINK_DIRECTORIES(
        #./libs/mac
        #    ./libs/ios
        ./libs
)


# Source files
#set (SOURCES
#        Client.cpp
#        ClientImpl.cpp
#        Tcp.cpp
#        Tool.cpp
#        Packet.cpp
#        Http.cpp
#        ObjectMapper.cpp
#        ./proto/chat.base.pb.cc
#        ./proto/chat.user.pb.cc
#        ./proto/chat.authenticate.pb.cc
#        ./proto/chat.contacts.pb.cc
#        ./proto/chat.message.pb.cc
#        )

set(NETWORK_DIR ${PROJECT_SOURCE_DIR}/src)

FILE(GLOB_RECURSE NETWORK_SRCS  ${NETWORK_DIR}/*.cpp)
#FILE(GLOB_RECURSE JSON_SRCS  ${JSON_DIR}/*.cpp)

# Library
if (BUILD_SHARED)
    add_library (msgtrans_client SHARED ${NETWORK_SRCS})
    message(STATUS "Building shared version...")
else()
    add_library (msgtrans_client STATIC ${NETWORK_SRCS})
    message(STATUS "Building static version...")
endif()

# Link the privchat with the LIBRARIES
target_link_libraries(msgtrans_client ${LIBRARIES})


# Executable
add_executable (demo examples/client/main.cpp)

# Link the library with the executable
target_link_libraries(demo msgtrans_client ${LIBRARIES})
#target_link_libraries(msgtrans_client ${LIBRARIES})

#Installation
set (CMAKE_INSTALL_PREFIX "${imsdk_SOURCE_DIR}")
install (TARGETS msgtrans_client DESTINATION lib)
install (FILES Client.h DESTINATION include)

#set(CMAKE_CXX_STANDARD 14)
#
#add_library(msgtrans_client library.cpp library.h)