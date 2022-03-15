cmake_minimum_required(VERSION 3.12)
project(Mint)

set(CMAKE_CXX_STANDARD 17)

add_executable(Mint src/main.cpp src/Mint.cpp src/Mint.h src/Token.cpp src/Token.h src/Lexer.cpp src/Lexer.h src/Expr.h src/Parser.cpp src/Parser.h src/Interpreter.cpp src/Interpreter.h src/RuntimeError.h src/Stmt.h src/Environment.cpp src/Environment.h src/MintCallable.h src/MintFunction.cpp src/MintFunction.h src/Resolver.cpp src/Resolver.h)
