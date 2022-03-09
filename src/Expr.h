//
// Created by marco on 3/8/22.
//

#ifndef MINT_EXPR_H
#define MINT_EXPR_H

#include <any>
#include <memory>
#include "Token.h"

struct Binary;
struct Grouping;
struct Literal;
struct Unary;

class Visitor {
public:
    [[nodiscard]] virtual std::any visit_binary_expr(std::shared_ptr<Binary> expr) = 0;
    [[nodiscard]] virtual std::any visit_grouping_expr(std::shared_ptr<Grouping> expr) = 0;
    [[nodiscard]] virtual std::any visit_literal_expr(std::shared_ptr<Literal> expr) = 0;
    [[nodiscard]] virtual std::any visit_unary_expr(std::shared_ptr<Unary> expr) = 0;
    virtual ~Visitor() = default;
};

class Expr {
public:
    virtual std::any accept(Visitor& visitor) = 0;
    virtual ~Expr() = default;
};

struct Binary : Expr, public std::enable_shared_from_this<Binary> {
    Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
        : left(std::move(left)), op(std::move(op)), right(std::move(right)) {};

    std::any accept(Visitor& visitor) override {
        return visitor.visit_binary_expr(shared_from_this());
    }

    const std::shared_ptr<Expr> left;
    const Token op;
    const std::shared_ptr<Expr> right;
};

struct Grouping : Expr, public std::enable_shared_from_this<Grouping> {
    explicit Grouping(std::shared_ptr<Expr> expr)
        : expr(std::move(expr)) {};

    std::any accept(Visitor& visitor) override {
        return visitor.visit_grouping_expr(shared_from_this());
    }

    const std::shared_ptr<Expr> expr;
};

struct Literal : Expr, public std::enable_shared_from_this<Literal> {
    explicit Literal(std::any value) : value(std::move(value)) {};
    std::any accept(Visitor& visitor) override {
        return visitor.visit_literal_expr(shared_from_this());
    }

    const std::any value;
};

struct Unary : Expr, public std::enable_shared_from_this<Unary> {
    Unary(Token op, std::shared_ptr<Expr> right) : op(std::move(op)), right(std::move(right)) {};
    std::any accept(Visitor& visitor) override {
        return visitor.visit_unary_expr(shared_from_this());
    }

    const Token op;
    const std::shared_ptr<Expr> right;
};

#endif //MINT_EXPR_H
