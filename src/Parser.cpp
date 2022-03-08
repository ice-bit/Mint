//
// Created by marco on 3/8/22.
//

#include <cassert>
#include "Parser.h"
#include "Mint.h"

std::shared_ptr<Expr> Parser::parse() {
    try {
        return expression();
    } catch(const ParseError& err) {
        return nullptr;
    }
}

std::shared_ptr<Expr> Parser::expression() {
    return equality();
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
        Token op = previous();
        auto right = term();
        expr = std::make_shared<Binary>(expr, std::move(op), right);
    }

    return expr;
}

std::shared_ptr<Expr> Parser::term() {
    auto expr = factor();

    while(match(token_type::MINUS, token_type::PLUS)) {
        Token op = previous();
        auto right = factor();
        expr = std::make_shared<Binary>(expr, std::move(op), right);
    }

    return expr;
}

std::shared_ptr<Expr> Parser::factor() {
    auto expr = unary();

    while(match(token_type::SLASH, token_type::STAR)) {
        Token op = previous();
        auto right = unary();
        expr = std::make_shared<Binary>(expr, std::move(op), right);
    }

    return expr;
}

std::shared_ptr<Expr> Parser::unary() {
    if(match(token_type::BANG, token_type::MINUS)) {
        Token op = previous();
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

Token Parser::consume(token_type type, const std::string_view msg) {
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

Parser::ParseError Parser::error(const Token& token, std::string_view msg) {
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