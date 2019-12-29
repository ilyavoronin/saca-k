cmake_minimum_required(VERSION 3.15)
project(SACA_K)

set(CMAKE_CXX_STANDARD 17)

include_directories(include/)

add_executable(SACA_K src/main.cpp
               src/SuffixSort.cpp include/SuffixSort.h)