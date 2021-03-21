cmake_minimum_required(VERSION 3.15)
project(scanner)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        scanner scanner.cpp fileIO.cpp fileIO.h scanner.h
        regexTree.cpp regexTree.h DFA.cpp DFA.h)