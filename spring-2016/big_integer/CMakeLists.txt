cmake_minimum_required(VERSION 3.5)

project(big_integer)

include_directories(${big_integer_SOURCE_DIR})

set(SRC_FILES src/big_integer.cpp
		includes/big_integer.h
		test/big_integer_testing.cpp
		test/gtest/gtest.h
		test/gtest/gtest-all.cc
		test/gtest/gtest_main.cc)

add_executable(big_integer_testing ${SRC_FILES})

set(TEST_FILES src/big_integer.cpp
        includes/big_integer.h
        test/test.cpp)

add_executable(local_test ${TEST_FILES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -pedantic -Werror --warn-uninitialized")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

target_link_libraries(big_integer_testing -lpthread)