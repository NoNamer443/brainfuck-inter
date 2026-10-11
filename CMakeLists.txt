cmake_minimum_required(VERSION 3.10)

project(brainfuck)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(shell
    shell.cpp
)

add_executable(test
    test.cpp
)