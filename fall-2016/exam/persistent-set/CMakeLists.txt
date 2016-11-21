cmake_minimum_required(VERSION 3.5)
project(persistent-set)

include_directories(${persistent-set_SOURCE_DIR})

set(SRC_FILES main.cpp
            persistent_set.h
            persistent_set.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Werror -Wextra -pedantic -Wformat=2 \
    -Wfloat-equal -Wconversion -Wlogical-op -Wcast-qual -Wcast-align -fsanitize=address -fstack-protector -lmcheck")

add_executable(persistent-set ${SRC_FILES})
