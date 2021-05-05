cmake_minimum_required(VERSION 3.15)
project(miniC_compiler)

set(CMAKE_CXX_STANDARD 17)

add_executable(miniC_compiler src/parser.cpp src/scanner.cpp src/main.cpp src/translate.cpp src/translate.h)