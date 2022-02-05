cmake_minimum_required(VERSION 3.10)

# set the project name
project(arduino-resource-monitor-server-linux)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(../lib/include)

set(serial_SRCS ../lib/src/serial.cc ../lib/include/serial/serial.h 
    ../lib/include/serial/v8stdint.h ../lib/src/impl/list_ports/list_ports_linux.cc)
list(APPEND serial_SRCS ../lib/src/impl/unix.cc)

add_library(serial ${serial_SRCS})
target_link_libraries(serial rt)

add_executable(arduino-resource-monitor-server-linux server.cpp)
target_link_libraries(arduino-resource-monitor-server-linux serial)