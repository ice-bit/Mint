//
// Created by marco on 3/8/22.
//

#ifndef MINT_LEXER_H
#define MINT_LEXER_H

#include <string>
#include <utility>
#include <vector>
#include <map>

#include "Token.h"

class Lexer {
public:
    explicit Lexer(std::string source) : source(std::move(source)) {};
    std::vector<Token> scan_tokens();

private:
    bool is_at_end();
    void scan_token();
    char advance();
    void add_token(token_type type);
    void add_token(token_type type, const std::any& literal);
    bool match(char expected);
    char peek();
    void parse_string();
    void parse_number();
    static constexpr bool is_digit(char c);
    static constexpr bool is_alpha(char c);
    static constexpr bool is_alphanumeric(char c);
    char peek_next();
    void identifier();


    std::vector<Token> tokens;
    unsigned int start = 0;
    unsigned int current = 0;
    unsigned int line = 1;
    std::string source;
    const std::map<std::string, token_type> keywords = {
            {"and", token_type::AND},
            {"or", token_type::OR},
            {"class", token_type::CLASS},
            {"else", token_type::ELSE},
            {"false", token_type::FALSE},
            {"for", token_type::FOR},
            {"fn", token_type::FN},
            {"if", token_type::IF},
            {"nil", token_type::NIL},
            {"or", token_type::OR},
            {"print", token_type::PRINT},
            {"return", token_type::RETURN},
            {"super", token_type::SUPER},
            {"this", token_type::THIS},
            {"true", token_type::TRUE},
            {"let", token_type::LET},
            {"while", token_type::WHILE},
    };
};


#endif //MINT_LEXER_H
