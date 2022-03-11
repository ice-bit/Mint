cmake_minimum_required(VERSION 3.21)
project(Mint)

set(CMAKE_CXX_STANDARD 17)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

add_executable(Mint src/main.cpp src/Mint.cpp src/Mint.h src/Token.cpp src/Token.h src/Lexer.cpp src/Lexer.h src/Expr.h src/Parser.cpp src/Parser.h src/Interpreter.cpp src/Interpreter.h src/RuntimeError.h src/Stmt.h src/Environment.cpp src/Environment.h src/MintCallable.h src/MintFunction.cpp src/MintFunction.h src/Resolver.cpp src/Resolver.h)
