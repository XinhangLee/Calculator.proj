cmake_minimum_required(VERSION 3.30)
project(Calculator_proj C)

set(CMAKE_C_STANDARD 11)

include_directories(calculation)

add_executable(Calculator_proj
        test.c
        calculation/calculation.c)
