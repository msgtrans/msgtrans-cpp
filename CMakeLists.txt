cmake_minimum_required(VERSION 3.13)
project(msgtrans_cpp C CXX)


MESSAGE(STATUS "The OS is ${CMAKE_SYSTEM}")
MESSAGE(STATUS "The platform is ${CMAKE_SYSTEM_NAME}")


# CMake -DCMAKE_BUILD_TYPE=Debug .
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
#SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

set(CMAKE_CXX_STANDARD 11)


if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")

    set(BUILD_SHARED true)

elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin") 

    #ios
    set(BUILD_SHARED false)

else()
    MESSAGE(FATAL_ERROR "Unsupported platform.")
endif()

set(LIBRARIES evpp_static event pthread protobuf glog )

# Includes
set(INCLUDE_BASE ${PROJECT_SOURCE_DIR}/include)

include_directories(
    ${INCLUDE_BASE} 
    ${INCLUDE_BASE}/common
    ${INCLUDE_BASE}/channel
    ${INCLUDE_BASE}/ee2e
)

LINK_DIRECTORIES(

)

set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

FILE(GLOB_RECURSE All_SOURCE ${SOURCE_DIR}/*.cpp ${SOURCE_DIR}/common/*.cpp 
    ${SOURCE_DIR}/channel/*.cpp ${SOURCE_DIR}/ee2e/*.cpp 
    ${SOURCE_DIR}/ee2e/*.cc)

# Library
if (BUILD_SHARED)
    message(STATUS "Building shared version...")
    add_library (msgtrans_cpp SHARED ${All_SOURCE})
else()
    message(STATUS "Building static version...")
    add_library (msgtrans_cpp STATIC ${All_SOURCE})
endif()

# Link the privchat with the LIBRARIES
target_link_libraries(msgtrans_cpp ${LIBRARIES})


# Executable
# add_executable (demo examples/client/main.cpp)

# Link the library with the executable
target_link_libraries(msgtrans_cpp ${LIBRARIES})

#Installation

FILE(GLOB_RECURSE HEADER_FILES  ${INCLUDE_BASE}/*.h)
FILE(GLOB_RECURSE CHANNEL_HEADER_FILES  ${INCLUDE_BASE}/channel/*.h)
FILE(GLOB_RECURSE COMMON_HEADER_FILES  ${INCLUDE_BASE}/common/*.h)
FILE(GLOB_RECURSE EE2E_HEADER_FILES  ${INCLUDE_BASE}/ee2e/*.h)

install (TARGETS msgtrans_cpp DESTINATION lib)

set(INCLUDE_TARGET_BASE include/msgtrans)

install(FILES ${HEADER_FILES} DESTINATION ${INCLUDE_TARGET_BASE})
install(FILES ${CHANNEL_HEADER_FILES} DESTINATION ${INCLUDE_TARGET_BASE}/channel)
install(FILES ${COMMON_HEADER_FILES} DESTINATION ${INCLUDE_TARGET_BASE}/common)
install(FILES ${EE2E_HEADER_FILES} DESTINATION ${INCLUDE_TARGET_BASE}/ee2e)

