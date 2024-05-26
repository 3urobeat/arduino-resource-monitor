cmake_minimum_required(VERSION 3.10)

# Set the project name
project(arduino-resource-monitor-server-linux)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "-std=gnu99 -lm") # I'd like to use inits in for loops as well as the math library, thanks


# Flags
option(BUILD_RELEASE "Build release" OFF)
if (BUILD_RELEASE)
    add_definitions(-DBUILD_RELEASE)
endif()

option(BUILD_RELEASE_CLIENT_LESS "Build release + clientLessMode" OFF)
if (BUILD_RELEASE_CLIENT_LESS)
    add_definitions(-DBUILD_RELEASE_CLIENT_LESS)
endif()


# Relic from when C++ was used:
# Stuff so that threads work https://stackoverflow.com/a/61439452
#set(CMAKE_THREAD_LIBS_INIT "-lpthread")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
#set(CMAKE_HAVE_THREADS_LIBRARY 1)
#set(CMAKE_USE_WIN32_THREADS_INIT 0)
#set(CMAKE_USE_PTHREADS_INIT 1)
#set(THREADS_PREFER_PTHREAD_FLAG ON)


# Include Serial lib
include_directories(lib/c-periphery/src)

set(serial_SRCS lib/c-periphery/src/serial.h lib/c-periphery/src/serial.c)

add_library(serial STATIC ${serial_SRCS})


# Add all source files to compile
set(SOURCES
    src/comm/comm.h
    src/comm/makeConnection.c
    src/comm/sendMeasurements.c
    src/comm/serialWrapper.c
    src/data/data.h
    src/data/handleStreams.c
    src/helpers/helpers.h
    src/helpers/misc.c
    src/sensors/getMeasurements.c
    src/sensors/getSensors.c
    src/server.c
    src/server.h
)


# DO IT
add_executable(arduino-resource-monitor-server-linux ${SOURCES})
target_link_libraries(arduino-resource-monitor-server-linux serial)
