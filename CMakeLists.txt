cmake_minimum_required(VERSION 3.16)
project(SNLInterpreter)

set(CMAKE_CXX_STANDARD 14)


add_executable(SNLInterpreter main.cpp LexicalAnalysis/scanner.h
        LexicalAnalysis/scanner.cpp util.h global.h util.cpp
        SyntaxAnalysis/parse.h SyntaxAnalysis/parse.cpp
        SemanticAnalysis/symbTable.h SemanticAnalysis/symbTable.cpp
        SemanticAnalysis/analyze.h SemanticAnalysis/analyze.cpp
        MidCodeGeneration/midcode.h MidCodeGeneration/midcode.cpp)

