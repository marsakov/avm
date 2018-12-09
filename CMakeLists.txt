cmake_minimum_required(VERSION 3.12)
project(avm)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(avm
        Creator.cpp
        Creator.hpp
        IOperand.h
        main.cpp
        Operand.hpp
        Parser.cpp
        Parser.hpp Exception.cpp Exception.hpp)
