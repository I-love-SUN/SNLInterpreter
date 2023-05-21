cmake_minimum_required(VERSION 3.16)
project(SNLInterpreter)

set(CMAKE_CXX_STANDARD 14)

add_executable(SNLInterpreter main.cpp LexicalAnalysis/scanner.h LexicalAnalysis/scanner.cpp util.h util.cpp global.h)