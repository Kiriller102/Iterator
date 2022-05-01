cmake_minimum_required(VERSION 3.21)
project(kash_task)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(kash_task
        Iterator_main.cpp
        List.h)
