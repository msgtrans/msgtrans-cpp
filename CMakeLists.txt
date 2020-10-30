cmake_minimum_required(VERSION 3.13)
project(msgtrans_cpp C CXX)

# CMake -DCMAKE_BUILD_TYPE=Debug .
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
#SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

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

set(INCLUDE_BASE ${PROJECT_SOURCE_DIR}/include)

include_directories(
    ${INCLUDE_BASE} 
    ${INCLUDE_BASE}/common
    ${INCLUDE_BASE}/channel
    ${INCLUDE_BASE}/ee2e
)

LINK_DIRECTORIES(
        #./libs/mac
        #    ./libs/ios
#	/Users/putao/lib
	# /Users/putao/ImServices/im_sdk/libs/mac
)

set(NETWORK_DIR ${PROJECT_SOURCE_DIR}/src)

FILE(GLOB_RECURSE NETWORK_SRCS  ${NETWORK_DIR}/*.cpp ${NETWORK_DIR}/common/*.cpp 
    ${NETWORK_DIR}/channel/*.cpp ${NETWORK_DIR}/ee2e/*.cpp 
    ${NETWORK_DIR}/ee2e/*.cc)
#FILE(GLOB_RECURSE JSON_SRCS  ${JSON_DIR}/*.cpp)

# Library
if (BUILD_SHARED)
    add_library (msgtrans_cpp SHARED ${NETWORK_SRCS})
    message(STATUS "Building shared version...")
else()
    add_library (msgtrans_cpp STATIC ${NETWORK_SRCS})
    message(STATUS "Building static version...")
endif()

# Link the privchat with the LIBRARIES
target_link_libraries(msgtrans_cpp ${LIBRARIES})


# Executable
# add_executable (demo examples/client/main.cpp)

# Link the library with the executable
target_link_libraries(msgtrans_cpp ${LIBRARIES})
#target_link_libraries(msgtrans_cpp ${LIBRARIES})

#Installation
set (CMAKE_INSTALL_PREFIX "${imsdk_SOURCE_DIR}")
install (TARGETS msgtrans_cpp DESTINATION lib)
install (FILES Client.h DESTINATION include)

#set(CMAKE_CXX_STANDARD 14)
#
#add_library(msgtrans_cpp library.cpp library.h)
