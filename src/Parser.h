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
#include "Stmt.h"

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens) : tokens(tokens) {};
    std::vector<std::shared_ptr<Stmt>> parse();
private:
    class ParseError : public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };
    std::shared_ptr<Expr> expression();
    std::shared_ptr<Stmt> declaration();
    std::shared_ptr<Stmt> statement();
    std::shared_ptr<Stmt> print_statement();
    std::shared_ptr<Stmt> var_declaration();
    std::shared_ptr<Stmt> expr_statement();
    std::vector<std::shared_ptr<Stmt>> block();
    std::shared_ptr<Expr> assignment();
    std::shared_ptr<Expr> equality();
    std::shared_ptr<Expr> comparison();
    std::shared_ptr<Expr> term();
    std::shared_ptr<Expr> factor();
    std::shared_ptr<Expr> unary();
    std::shared_ptr<Expr> primary();
    Token consume(token_type type, const std::string& msg);
    Token advance();
    Token peek();
    Token previous();
    template<class... T>
    bool match(T... type);
    bool is_at_end();
    bool check(token_type type);
    static ParseError error(const Token& token, const std::string& msg);
    void synchronize();

    const std::vector<Token>& tokens;
    unsigned int current = 0;
};


#endif //MINT_PARSER_H
