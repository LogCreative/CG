cmake_minimum_required(VERSION 3.23)
project(IncrPoly)

set(CMAKE_CXX_STANDARD 14)

add_executable(IncrPoly main.cpp IncrPoly.hpp IncrPoly-cli.h)

#add_subdirectory(tests)