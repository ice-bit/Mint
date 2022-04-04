project(src)

set(HEADER_FILES
    Mint.h
    Token.h
    Lexer.h
    Expr.h
    Parser.h
    Interpreter.h
    RuntimeError.h
    Stmt.h
    Environment.h
    MintCallable.h
    MintFunction.h
    Resolver.h)

set(SOURCE_FILES
    Mint.cpp
    Token.cpp
    Lexer.cpp
    Parser.cpp
    Interpreter.cpp
    Environment.cpp
    MintFunction.cpp
    Resolver.cpp)

add_library(src STATIC ${SOURCE_FILES} ${HEADER_FILES})
