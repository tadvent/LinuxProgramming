
cmake_minimum_required(VERSION 2.8)
project(cat)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
add_executable(cat cat.cpp)

