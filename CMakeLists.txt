cmake_minimum_required(VERSION 3.10)

set(CMAKE_CUDA_HOST_COMPILER /usr/bin/g++-16)

project(LCP1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(a a.cpp)

