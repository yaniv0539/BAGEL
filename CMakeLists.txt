cmake_minimum_required(VERSION 3.7)
project(BAGEL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(BAGEL main.cpp
        bagel.h
        tests.cpp
        bagel_cfg.h
)
