
cmake_minimum_required(VERSION 2.8.0)
project(echo)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
add_executable(echo echo.cpp)

