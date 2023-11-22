cmake_minimum_required(VERSION 3.26)
project(countdown)

set(CMAKE_CXX_STANDARD 17)

add_executable(countdown main.cpp
        src/countdown.cpp
        src/countdown.h
)
