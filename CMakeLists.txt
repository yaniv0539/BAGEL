cmake_minimum_required(VERSION 3.30)
project(BAGEL)

set(CMAKE_CXX_STANDARD 17)

add_executable(BAGEL main.cpp
        bagel.h
        tests.cpp
)
