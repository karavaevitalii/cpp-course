cmake_minimum_required(VERSION 3.5)

project(big_integer)

include_directories(${big_integer_SOURCE_DIR})

set(SRC_FILES src/big_integer.cpp
		includes/big_integer.h
		test/big_integer_testing.cpp
		includes/storage.h
		src/storage.cpp
		test/gtest/gtest.h
		test/gtest/gtest-all.cc
		test/gtest/gtest_main.cc)

add_executable(big_integer_testing ${SRC_FILES})

set(TEST_FILES src/big_integer.cpp
                includes/big_integer.h
		src/storage.cpp
		includes/storage.h
                test/test.cpp)

add_executable(local_test ${TEST_FILES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Werror -Wextra -pedantic -Wformat=2 \
    -Wfloat-equal -Wconversion -Wlogical-op -Wcast-qual -Wcast-align -fsanitize=address -fstack-protector -lmcheck")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

target_link_libraries(big_integer_testing -lpthread)
