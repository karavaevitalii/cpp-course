cmake_minimum_required(VERSION 3.5)

project(either)

include_directories(${either_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Werror -Wextra -pedantic -Wformat=2 \
    -Wfloat-equal -Wconversion -Wlogical-op -Wcast-qual -Wcast-align -fsanitize=address -fstack-protector -lmcheck")

set (either main.cpp either.h)

add_executable(either ${either})

