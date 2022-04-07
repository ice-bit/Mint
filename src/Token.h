/*
 * Mint interpreter
 * Developed by Marco Cetica (c) 2022
 * Released under GPLv3
 *
 */
#pragma once
#include <any>
#include <string>
#include <utility>

enum class token_type {
    // Single character tokens(i.e., dots, parenthesis, etc.
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,
    // Single or two characters tokens(i.e., !=, ==, >=, etc.)
    BANG, BANG_EQUAL, EQUAL, EQUAL_EQUAL,
    GREATER, RIGHT_SHIFT, GREATER_EQUAL, LESS, LEFT_SHIFT, LESS_EQUAL, MODULO,
    // Literals(i.e., strings, numbers, etc.)
    IDENTIFIER, STRING, NUMBER,
    // Builtin keywords(fn, if, else, etc.)
    AND, BIT_AND, ELSE, FALSE, FN, FOR, IF, NIL, OR, BIT_OR,
    XOR, NOT, PRINT, RETURN, TRUE, LET, WHILE,
    // End of file
    MINT_EOF
};


class Token {
public:
    Token(token_type type, std::string lexeme, std::any literal, unsigned int line)
        : type(type), literal(std::move(literal)), lexeme(std::move(lexeme)), line(line) {};
    [[nodiscard]] auto to_string() -> std::string const;

    const token_type type;
    const std::any literal;
    const std::string lexeme;
    const unsigned int line;
};