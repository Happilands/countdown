cmake_minimum_required(VERSION 3.26)
project(countdown)

set(CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")

add_executable(countdown main.cpp
        src/countdown.cpp
        src/countdown.h
)
