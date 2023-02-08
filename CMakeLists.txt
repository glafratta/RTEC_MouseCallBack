cmake_minimum_required(VERSION 3.1.0)

project(mouse_callback)

set (CMAKE_CXX_STANDARD 17)

find_package(Threads)

add_executable(getMouseInput main.cpp)

target_link_libraries(getMouseInput rt cpptimer)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(CMAKE_BUILD_TYPE Debug)

#set(CMAKE_BUILD_TYPE RelWithDebInfo)
