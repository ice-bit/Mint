//
// Created by marco on 3/8/22.
//

#pragma once 
#include <memory>
#include <vector>
#include <stdexcept>
#include "Token.h"
#include "Expr.h"
#include "Stmt.h"

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens) : tokens(tokens) {};
    auto parse() -> std::vector<std::shared_ptr<Stmt>>;
private:
    class ParseError : public std::runtime_error {
    public:
      using std::runtime_error::runtime_error;
    };

    auto expression() -> std::shared_ptr<Expr>;
    auto declaration() -> std::shared_ptr<Stmt>;
    auto statement() -> std::shared_ptr<Stmt>;
    auto print_statement() -> std::shared_ptr<Stmt>;
    auto var_declaration() -> std::shared_ptr<Stmt>;
    auto while_statement() -> std::shared_ptr<Stmt>;
    auto for_statement() -> std::shared_ptr<Stmt>;
    auto function(std::string kind) -> std::shared_ptr<Function>;
    auto return_statement() -> std::shared_ptr<Stmt>;
    auto call() -> std::shared_ptr<Expr>;
    auto finish_call(const std::shared_ptr<Expr>& callee) -> std::shared_ptr<Expr>;
    auto and_expression() -> std::shared_ptr<Expr>;
    auto or_expression() -> std::shared_ptr<Expr>;
    auto expression_statement() -> std::shared_ptr<Stmt>;
    auto if_statement() -> std::shared_ptr<Stmt>;
    auto block() -> std::vector<std::shared_ptr<Stmt>>;
    auto assignment() -> std::shared_ptr<Expr>;
    auto equality() -> std::shared_ptr<Expr>;
    auto comparison() -> std::shared_ptr<Expr>;
    auto term() -> std::shared_ptr<Expr>;
    auto factor() -> std::shared_ptr<Expr>;
    auto unary() -> std::shared_ptr<Expr>;
    auto primary() -> std::shared_ptr<Expr>;
    template <class... T>
    auto match(T... type) -> bool;
    auto check(token_type type) -> bool;
    auto is_at_end() -> bool;
    auto peek() -> Token;
    auto previous() -> Token;
    static auto error(const Token& token, const std::string& msg) -> ParseError;
    auto advance() -> Token;
    auto consume(token_type type, std::string msg) -> Token;
    auto synchronize() -> void;

    const std::vector<Token>& tokens;
    unsigned int current = 0;
};
