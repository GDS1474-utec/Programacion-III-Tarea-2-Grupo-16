cmake_minimum_required(VERSION 4.2)
project(PROGRA_3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(".")

add_executable(PROGRA_3
        "core_numeric.h"
        "test.cpp")
