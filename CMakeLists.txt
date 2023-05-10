cmake_minimum_required(VERSION 3.16)
project(SNLInterpreter)

set(CMAKE_CXX_STANDARD 14)

add_executable(SNLInterpreter main.cpp scanner.h scanner.cpp util.h global.h)