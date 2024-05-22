cmake_minimum_required(VERSION 3.10)

# Set the project name
project(arduino-resource-monitor-server-linux)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "-std=gnu99 -lm") # I'd like to use inits in for loops as well as the math library, thanks


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
    server.c
    server.h
    helpers/getMeasurements.c
    helpers/getSensors.c
    helpers/handleStreams.c
    helpers/helpers.h
    helpers/makeConnection.c
    helpers/misc.c
    helpers/sendMeasurements.c
    helpers/serialWrapper.c
)


# DO IT
add_executable(arduino-resource-monitor-server-linux ${SOURCES})
target_link_libraries(arduino-resource-monitor-server-linux serial)
