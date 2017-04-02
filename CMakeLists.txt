cmake_minimum_required(VERSION 3.6)
project(sort_grouping)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp mergesort.h timer.h quicksort.h)
add_executable(sort_grouping ${SOURCE_FILES})