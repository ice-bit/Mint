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

auto Parser::expression() -> std::shared_ptr<Expr> {
    return assignment();
}

auto Parser::declaration() -> std::shared_ptr<Stmt> {
    try {
        if(match(token_type::FN)) return function("function");
        if(match(token_type::LET)) return var_declaration();
        return statement();
    } catch(ParseError& err) {
        synchronize();
        return nullptr;
    }
}

auto Parser::statement() -> std::shared_ptr<Stmt> {
    if(match(token_type::FOR)) return for_statement();
    if(match(token_type::IF)) return if_statement();
    if(match(token_type::PRINT)) return print_statement();
    if(match(token_type::RETURN)) return return_statement();
    if(match(token_type::WHILE)) return while_statement();
    if(match(token_type::LEFT_BRACE)) return std::make_shared<Block>(block());

    return expression_statement();
}

auto Parser::for_statement() -> std::shared_ptr<Stmt> {
    consume(token_type::LEFT_PAREN, "Expect '(' after for");

    std::shared_ptr<Stmt> initializer;
    if(match(token_type::SEMICOLON)) initializer = nullptr;
    else if(match(token_type::LET)) initializer = var_declaration();
    else initializer = expression_statement();

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

auto Parser::function(std::string kind) -> std::shared_ptr<Function> {
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


auto Parser::return_statement() -> std::shared_ptr<Stmt> {
    Token keyword = previous();
    std::shared_ptr<Expr> value = nullptr;
    if(!check(token_type::SEMICOLON)) value = expression();

    consume(token_type::SEMICOLON, "Expect ';' after return value.");

    return std::make_shared<Return>(keyword, value);
}

auto Parser::call() -> std::shared_ptr<Expr> {
    std::shared_ptr<Expr> expr = primary();

    while(true) {
        if(match(token_type::LEFT_PAREN)) expr = finish_call(expr);
        else break;
    }

    return expr;
}

auto Parser::finish_call(const std::shared_ptr<Expr>& callee) -> std::shared_ptr<Expr> {
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
auto Parser::and_expression() -> std::shared_ptr<Expr> {
    std::shared_ptr<Expr> expr = equality();

    while(match(token_type::AND)) {
        Token op = previous();
        std::shared_ptr<Expr> right = equality();
        expr = std::make_shared<Logical>(expr, std::move(op), right);
    }

    return expr;
}
auto Parser::or_expression() -> std::shared_ptr<Expr> {
    std::shared_ptr<Expr> expr = and_expression();

    while(match(token_type::OR)) {
        Token op = previous();
        std::shared_ptr<Expr> right = and_expression();
        expr = std::make_shared<Logical>(expr, std::move(op), right);
    }

    return expr;
}

auto Parser::expression_statement() -> std::shared_ptr<Stmt> {
    std::shared_ptr<Expr> expr = expression();
    consume(token_type::SEMICOLON, "Expect ';' after expression.");

    return std::make_shared<Expression>(expr);
}

auto Parser::if_statement() -> std::shared_ptr<Stmt> {
    consume(token_type::LEFT_PAREN, "Expect '(' after if.");
    std::shared_ptr<Expr> cond = expression();
    consume(token_type::RIGHT_PAREN, "Expect ')' after if condition.");

    std::shared_ptr<Stmt> then_branch = statement();
    std::shared_ptr<Stmt> else_branch = nullptr;
    if(match(token_type::ELSE)) else_branch = statement();

    return std::make_shared<If>(cond, then_branch, else_branch);
}

auto Parser::while_statement() -> std::shared_ptr<Stmt> {
    consume(token_type::LEFT_PAREN, "Expect '(' after while.");
    std::shared_ptr<Expr> cond = expression();
    consume(token_type::RIGHT_PAREN, "Expect ')' after condition.");
    std::shared_ptr<Stmt> body = statement();

    return std::make_shared<While>(cond, body);
}

auto Parser::print_statement() -> std::shared_ptr<Stmt> {
    auto value = expression();
    consume(token_type::SEMICOLON, "Expect ';' after value.");

    return std::make_shared<Print>(value);
}

auto Parser::var_declaration() -> std::shared_ptr<Stmt> {
    auto name = consume(token_type::IDENTIFIER, "Expect variable name.");

    std::shared_ptr<Expr> initializer = nullptr;
    if(match(token_type::EQUAL))
        initializer = expression();

    consume(token_type::SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_shared<Var>(std::move(name), initializer);
}

auto Parser::block() -> std::vector<std::shared_ptr<Stmt>> {
    std::vector<std::shared_ptr<Stmt>> statements;

    while(!check(token_type::RIGHT_BRACE) && !is_at_end())
        statements.push_back(declaration());

    consume(token_type::RIGHT_BRACE, "Expect '}' after block.");

    return statements;
}


auto Parser::assignment() -> std::shared_ptr<Expr> {
    auto expr = or_expression();

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

auto Parser::equality() -> std::shared_ptr<Expr> {
    auto expr = comparison();

    while(match(token_type::BANG_EQUAL, token_type::EQUAL_EQUAL)) {
        auto op = previous();
        auto right = comparison();
        expr = std::make_shared<Binary>(expr, std::move(op), right);
    }

    return expr;
}

auto Parser::comparison() -> std::shared_ptr<Expr> {
    auto expr = term();

    while(match(token_type::GREATER, token_type::GREATER_EQUAL, token_type::LESS, token_type::LESS_EQUAL)) {
        auto op = previous();
        auto right = term();
        expr = std::make_shared<Binary>(expr, std::move(op), right);
    }

    return expr;
}

auto Parser::term() -> std::shared_ptr<Expr> {
    auto expr = factor();

    while(match(token_type::MINUS, token_type::PLUS)) {
        auto op = previous();
        auto right = factor();
        expr = std::make_shared<Binary>(expr, std::move(op), right);
    }

    return expr;
}

auto Parser::factor() -> std::shared_ptr<Expr> {
    auto expr = unary();

    while(match(token_type::SLASH,
                token_type::STAR,
                token_type::MODULO,
                token_type::BIT_AND,
                token_type::BIT_OR,
                token_type::XOR,
                token_type::LEFT_SHIFT,
                token_type::RIGHT_SHIFT)) {
        Token op = previous();
        auto right = unary();
        expr = std::make_shared<Binary>(expr, std::move(op), right);
    }

    return expr;
}

auto Parser::unary() -> std::shared_ptr<Expr> {
    if(match(token_type::BANG, token_type::MINUS, token_type::NOT)) {
        auto op = previous();
        auto right = unary();
        return std::make_shared<Unary>(std::move(op), right);
    }

    return call();
}

auto Parser::primary() -> std::shared_ptr<Expr>  {
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
auto Parser::match(T... type) -> bool {
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

auto Parser::check(token_type type) -> bool {
    if(is_at_end()) return false;

    return peek().type == type;
}


auto Parser::advance() -> Token {
    if(!is_at_end()) ++current;

    return previous();
}

auto Parser::is_at_end() -> bool{
    return peek().type == token_type::MINT_EOF;
}

auto Parser::peek() -> Token {
    return tokens.at(current);
}

auto Parser::previous() -> Token {
    return tokens.at(current - 1);
}

auto Parser::error(const Token &token, const std::string& msg) -> Parser::ParseError {
   Mint::error(token, msg);

   return ParseError{""};
}

auto Parser::synchronize() -> void {
    advance();

    while(!is_at_end()) {
        if(previous().type == token_type::SEMICOLON) return;

        switch (peek().type) {
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