cmake_minimum_required(VERSION 3.5)
project(persistent_set)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic -Werror --warn-uninitialized")

set(SOURCE_FILES main.cpp
        persistent_set.cpp
        persistent_set.h)

add_executable(persistent_set ${SOURCE_FILES})