//
// Created by marco on 3/8/22.
//

#pragma once
#include <string>
#include <utility>
#include <vector>
#include <map>

#include "Token.h"

class Lexer {
public:
    explicit Lexer(std::string source) : source(std::move(source)) {};
    auto scan_tokens() -> std::vector<Token>;

private:
    auto is_at_end() -> bool;
    auto scan_token() -> void;
    auto advance() -> char;
    auto add_token(token_type type) -> void;
    auto add_token(token_type type, const std::any& literal) -> void;
    auto match(char expected) -> bool;
    auto peek() -> char;
    auto parse_string() -> void;
    auto parse_number() -> void;
    static constexpr auto is_digit(char c) -> bool;
    static constexpr auto is_alpha(char c) -> bool;
    static constexpr auto is_alphanumeric(char c) -> bool;
    auto peek_next() -> char;
    auto identifier() -> void;

    std::vector<Token> tokens;
    unsigned int start = 0;
    unsigned int current = 0;
    unsigned int line = 1;
    std::string source;
    const std::map<std::string, token_type> keywords = {
            {"&&", token_type::AND},
            {"||", token_type::OR},
            {"^", token_type::XOR},
            {"~", token_type::NOT},
            {"else", token_type::ELSE},
            {"false", token_type::FALSE},
            {"for", token_type::FOR},
            {"function", token_type::FN},
            {"if", token_type::IF},
            {"nil", token_type::NIL},
            {"or", token_type::OR},
            {"print", token_type::PRINT},
            {"return", token_type::RETURN},
            {"true", token_type::TRUE},
            {"let", token_type::LET},
            {"while", token_type::WHILE},
    };
};