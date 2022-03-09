//
// Created by marco on 3/8/22.
//

#include <cassert>
#include "Parser.h"
#include "Mint.h"
#include "Token.h"

std::vector<std::shared_ptr<Stmt>> Parser::parse() {
    std::vector<std::shared_ptr<Stmt>> statements;

    while(!is_at_end())
        statements.push_back(declaration());
}

std::shared_ptr<Expr> Parser::expression() {
    return assignment();
}

std::shared_ptr<Stmt> Parser::declaration() {
    try {
        if(match(token_type::LET)) return var_declaration();

        return statement();
    } catch(const ParseError& err) {
        synchronize();
        return nullptr;
    }
}

std::shared_ptr<Stmt> Parser::statement() {
    if(match(token_type::PRINT)) return print_statement();
    if(match(token_type::LEFT_BRACE)) return std::make_shared<Block>(block());

    return expr_statement();
}

std::shared_ptr<Stmt> Parser::print_statement() {
    auto value = expression();
    consume(token_type::SEMICOLON, "Expect ';' after value.");

    return std::make_shared<Print>(value);
}

std::shared_ptr<Stmt> Parser::var_declaration() {
    Token name = consume(token_type::IDENTIFIER, "Expect variable name.");
    std::shared_ptr<Expr> initializer = nullptr;

    if(match(token_type::EQUAL)) initializer = expression();

    consume(token_type::SEMICOLON, "Expect ';' after variable declaration.");

    return std::make_shared<Var>(std::move(name), initializer);
}

std::shared_ptr<Stmt> Parser::expr_statement() {
    auto expr = expression();
    consume(token_type::SEMICOLON, "Expect ';' after expression.");

    return std::make_shared<Expression>(expr);
}

std::vector<std::shared_ptr<Stmt>> Parser::block() {
    std::vector<std::shared_ptr<Stmt>> statements;

    while(!check(token_type::RIGHT_BRACE) && !is_at_end())
        statements.push_back(declaration());

    consume(token_type::RIGHT_BRACE, "Expect '}' after block.");

    return statements;
}

std::shared_ptr<Expr> Parser::assignment() {
    auto expr = equality();

    if(match(token_type::EQUAL)) {
        Token equals = previous();
        auto value = assignment();

        if(auto * e = dynamic_cast<Variable*>(expr.get())) {
            Token name = e->name;
            return std::make_shared<Assign>(std::move(name), value);
        }

        Mint::error(equals, "Invalid assignment target.");
    }

    return expr;
}

std::shared_ptr<Expr> Parser::equality() {
    auto expr = comparison();

    while(match(token_type::BANG_EQUAL, token_type::EQUAL_EQUAL)) {
        Token op = previous();
        auto right = comparison();
        expr = std::make_shared<Binary>(expr, std::move(op), right);
    }

    return expr;
}

std::shared_ptr<Expr> Parser::comparison() {
    auto expr = term();

    while(match(token_type::GREATER, token_type::GREATER_EQUAL, token_type::LESS, token_type::LESS_EQUAL)) {
        auto op = previous();
        auto right = term();
        expr = std::make_shared<Binary>(expr, std::move(op), right);
    }

    return expr;
}

std::shared_ptr<Expr> Parser::term() {
    auto expr = factor();

    while(match(token_type::MINUS, token_type::PLUS)) {
        auto op = previous();
        auto right = factor();
        expr = std::make_shared<Binary>(expr, std::move(op), right);
    }

    return expr;
}

std::shared_ptr<Expr> Parser::factor() {
    auto expr = unary();

    while(match(token_type::SLASH, token_type::STAR)) {
        auto op = previous();
        auto right = unary();
        expr = std::make_shared<Binary>(expr, std::move(op), right);
    }

    return expr;
}

std::shared_ptr<Expr> Parser::unary() {
    if(match(token_type::BANG, token_type::MINUS)) {
        auto op = previous();
        auto right = unary();
        return std::make_shared<Unary>(std::move(op), right);
    }

    return primary();
}

std::shared_ptr<Expr> Parser::primary() {
    if(match(token_type::FALSE)) return std::make_shared<Literal>(false);
    if(match(token_type::TRUE)) return std::make_shared<Literal>(true);
    if(match(token_type::NIL)) return std::make_shared<Literal>(nullptr);

    if(match(token_type::NUMBER, token_type::STRING))
        return std::make_shared<Literal>(previous().literal);

    if(match(token_type::LEFT_PAREN)) {
        std::shared_ptr<Expr> expr = expression();
        consume(token_type::RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_shared<Grouping>(expr);
    }

    throw error(peek(), "Expect expression.");
}

template <class... T>
bool Parser::match(T... type) {
    assert((... && std::is_same_v<T, token_type>));

    if((... || check(type))) {
        advance();
        return true;
    }

    return false;
}

Token Parser::consume(token_type type, const std::string& msg) {
    if(check(type)) return advance();

    throw error(peek(), msg);
}

bool Parser::check(token_type type) {
    if(is_at_end()) return false;

    return peek().type == type;
}

Token Parser::advance() {
    if(!is_at_end()) ++current;

    return previous();
}

bool Parser::is_at_end() {
    return peek().type == token_type::MINT_EOF;
}

Token Parser::peek() {
    return tokens.at(current);
}

Token Parser::previous() {
    return tokens.at(current - 1);
}

Parser::ParseError Parser::error(const Token& token, const std::string& msg) {
    Mint::error(token, msg);
}

void Parser::synchronize() {
    advance();

    while(!is_at_end()) {
        if(previous().type == token_type::SEMICOLON) return;

        switch (peek().type) {
            case token_type::CLASS:
            case token_type::FN:
            case token_type::LET:
            case token_type::FOR:
            case token_type::IF:
            case token_type::WHILE:
            case token_type::PRINT:
            case token_type::RETURN: break;
        }
        advance();
    }
}