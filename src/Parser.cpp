//
// Created by marco on 3/9/22.
//

#include "Parser.h"
#include "Mint.h"

#include <cassert>

std::vector<std::shared_ptr<Stmt>> Parser::parse() {
    std::vector<std::shared_ptr<Stmt>> statements;

    while(!is_at_end())
        statements.push_back(declaration());

    return statements;
}

std::shared_ptr<Expr> Parser::expression() {
    return assignment();
}

std::shared_ptr<Stmt> Parser::declaration() {
    try {
        if(match(token_type::FN)) return function("function");
        if(match(token_type::LET)) return var_declaration();
        return statement();
    } catch(ParseError& err) {
        synchronize();
        return nullptr;
    }
}

std::shared_ptr<Stmt> Parser::statement() {
    if(match(token_type::FOR)) return for_statement();
    if(match(token_type::IF)) return if_statement();
    if(match(token_type::PRINT)) return print_statement();
    if(match(token_type::RETURN)) return return_statement();
    if(match(token_type::WHILE)) return while_statement();
    if(match(token_type::LEFT_BRACE)) return std::make_shared<Block>(block());

    return expr_statement();
}

std::shared_ptr<Stmt> Parser::for_statement() {
    consume(token_type::LEFT_PAREN, "Expect '(' after for");

    std::shared_ptr<Stmt> initializer;
    if(match(token_type::SEMICOLON)) initializer = nullptr;
    else if(match(token_type::LET)) initializer = var_declaration();
    else initializer = expr_statement();

    std::shared_ptr<Expr> cond = nullptr;
    if(!check(token_type::SEMICOLON)) cond = expression();
    consume(token_type::SEMICOLON, "Expect ';' after loop condition.");

    std::shared_ptr<Expr> increment = nullptr;
    if(!check(token_type::RIGHT_PAREN)) increment = expression();
    consume(token_type::RIGHT_PAREN, "Expect ')' after for clauses.");
    std::shared_ptr<Stmt> body = statement();

    if(increment != nullptr) {
        body = std::make_shared<Block>(
                std::vector<std::shared_ptr<Stmt>>{
                        body,
                        std::make_shared<Expression>(increment)});
    }

    if(cond == nullptr)
        cond = std::make_shared<Literal>(true);
    body = std::make_shared<While>(cond, body);

    if(initializer != nullptr)
        body = std::make_shared<Block>(
                std::vector<std::shared_ptr<Stmt>>{initializer, body});

    return body;
}

std::shared_ptr<Function> Parser::function(std::string kind) {
    Token name = consume(token_type::IDENTIFIER, "Expect " + kind + " name.");
    consume(token_type::LEFT_PAREN, "Expect '(' after " + kind + " name.");
    std::vector<Token> parameters;
    if(!check(token_type::RIGHT_PAREN)) {
        do {
            if(parameters.size() >= 255)
                error(peek(), "Can't have more than 255 parameters.");

            parameters.push_back(consume(token_type::IDENTIFIER, "Expect parameter name."));
        } while(match(token_type::COMMA));
    }
    consume(token_type::RIGHT_PAREN, "Expect ')' after parameters.");
    consume(token_type::LEFT_BRACE, "Expect '{' before " + kind + " body.");
    std::vector<std::shared_ptr<Stmt>> body = block();

    return std::make_shared<Function>(std::move(name), std::move(parameters), std::move(body));
}


std::shared_ptr<Stmt> Parser::return_statement() {
    Token keyword = previous();
    std::shared_ptr<Expr> value = nullptr;
    if(!check(token_type::SEMICOLON)) value = expression();

    consume(token_type::SEMICOLON, "Expect ';' after return value.");

    return std::make_shared<Return>(keyword, value);
}

std::shared_ptr<Expr> Parser::call() {
    std::shared_ptr<Expr> expr = primary();

    while(true) {
        if(match(token_type::LEFT_PAREN)) expr = finish_call(expr);
        else break;
    }

    return expr;
}

std::shared_ptr<Expr> Parser::finish_call(const std::shared_ptr<Expr>& callee) {
    std::vector<std::shared_ptr<Expr>> arguments;
    if(!check(token_type::RIGHT_PAREN)) {
        do {
            if(arguments.size() >= 255)
                error(peek(), "Can't have more than 255 arguments.");
            arguments.push_back(expression());
        } while(match(token_type::COMMA));
    }

    Token paren = consume(token_type::RIGHT_PAREN, "Expect ')' after arguments.");

    return std::make_shared<Call>(callee, std::move(paren), std::move(arguments));
}
std::shared_ptr<Expr> Parser::and_expression() {
    std::shared_ptr<Expr> expr = equality();

    while(match(token_type::AND)) {
        Token op = previous();
        std::shared_ptr<Expr> right = equality();
        expr = std::make_shared<Logical>(expr, std::move(op), right);
    }

    return expr;
}
std::shared_ptr<Expr> Parser::or_expression() {
    std::shared_ptr<Expr> expr = and_expression();

    while(match(token_type::OR)) {
        Token op = previous();
        std::shared_ptr<Expr> right = and_expression();
        expr = std::make_shared<Logical>(expr, std::move(op), right);
    }

    return expr;
}

std::shared_ptr<Stmt> Parser::expression_statement() {
    std::shared_ptr<Expr> expr = expression();
    consume(token_type::SEMICOLON, "Expect ';' after expression.");

    return std::make_shared<Expression>(expr);
}

std::shared_ptr<Stmt> Parser::if_statement() {
    consume(token_type::LEFT_PAREN, "Expect '(' after if.");
    std::shared_ptr<Expr> cond = expression();
    consume(token_type::RIGHT_PAREN, "Expect ')' after if condition.");

    std::shared_ptr<Stmt> then_branch = statement();
    std::shared_ptr<Stmt> else_branch = nullptr;
    if(match(token_type::ELSE)) else_branch = statement();

    return std::make_shared<If>(cond, then_branch, else_branch);
}

std::shared_ptr<Stmt> Parser::while_statement() {
    consume(token_type::LEFT_PAREN, "Expect '(' after while.");
    std::shared_ptr<Expr> cond = expression();
    consume(token_type::RIGHT_PAREN, "Expect ')' after condition.");
    std::shared_ptr<Stmt> body = statement();

    return std::make_shared<While>(cond, body);
}

std::shared_ptr<Stmt> Parser::print_statement() {
    auto value = expression();
    consume(token_type::SEMICOLON, "Expect ';' after value.");

    return std::make_shared<Print>(value);
}

std::shared_ptr<Stmt> Parser::var_declaration() {
    auto name = consume(token_type::IDENTIFIER, "Expect variable name.");

    std::shared_ptr<Expr> initializer = nullptr;
    if(match(token_type::EQUAL))
        initializer = expression();

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
        auto equals = previous();
        auto value = assignment();

        if(auto* e = dynamic_cast<Variable*>(expr.get())) {
            auto name = e->name;
            return std::make_shared<Assign>(std::move(name), value);
        }

        error(equals, "Invalid assignment target.");
    }

    return expr;
}

std::shared_ptr<Expr> Parser::equality() {
    auto expr = comparison();

    while(match(token_type::BANG_EQUAL, token_type::EQUAL_EQUAL)) {
        auto op = previous();
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
        Token op = previous();
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

    if(match(token_type::IDENTIFIER))
        return std::make_shared<Variable>(previous());

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

Token Parser::consume(token_type type, std::string msg) {
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

Parser::ParseError Parser::error(const Token &token, const std::string& msg) {
   Mint::error(token, msg);

   return ParseError{""};
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
            case token_type::RETURN: return;
            default: break;
        }
        advance();
    }
}