//
// Created by marco on 3/8/22.
//

#ifndef MINT_PARSER_H
#define MINT_PARSER_H

#include <memory>
#include <vector>
#include <stdexcept>
#include "Token.h"
#include "Expr.h"

class Parser {
public:
    Parser(const std::vector<Token>& tokens) : tokens(tokens) {};
    std::shared_ptr<Expr> parse();
private:
    class ParseError : public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };
    std::shared_ptr<Expr> equality();
    std::shared_ptr<Expr> comparison();
    std::shared_ptr<Expr> term();
    std::shared_ptr<Expr> factor();
    std::shared_ptr<Expr> unary();
    std::shared_ptr<Expr> primary();
    template<class... T>
    bool match(T... type);
    Token consume(token_type type, const std::string_view msg);
    Token advance();
    Token peek();
    Token previous();
    constexpr bool is_at_end();
    constexpr bool check(token_type type);
    ParseError error(const Token& token, std::string_view msg);
    void synchronize();

    const std::vector<Token>& tokens;
    unsigned int current = 0;
};


#endif //MINT_PARSER_H