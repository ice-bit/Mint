//
// Created by marco on 3/8/22.
//

#pragma once
#include <any>
#include <memory>
#include <utility>
#include <vector>
#include "Token.h"

struct Binary;
struct Call;
struct Grouping;
struct Literal;
struct Logical;
struct Unary;
struct Variable;
struct Assign;

class ExprVisitor {
public:
    [[nodiscard]] virtual std::any visit_binary_expr(std::shared_ptr<Binary> expr) = 0;
    [[nodiscard]] virtual std::any visit_call_expr(std::shared_ptr<Call> expr) = 0;
    [[nodiscard]] virtual std::any visit_grouping_expr(std::shared_ptr<Grouping> expr) = 0;
    [[nodiscard]] virtual std::any visit_literal_expr(std::shared_ptr<Literal> expr) = 0;
    [[nodiscard]] virtual std::any visit_logical_expr(std::shared_ptr<Logical> expr) = 0;
    [[nodiscard]] virtual std::any visit_unary_expr(std::shared_ptr<Unary> expr) = 0;
    [[nodiscard]] virtual std::any visit_variable_expr(std::shared_ptr<Variable> expr) = 0;
    [[nodiscard]] virtual std::any visit_assign_expr(std::shared_ptr<Assign> expr) = 0;
    virtual ~ExprVisitor() = default;
};

class Expr {
public:
    virtual std::any accept(ExprVisitor& visitor) = 0;
    virtual ~Expr() = default;
};

struct Binary : Expr, public std::enable_shared_from_this<Binary> {
    Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
        : left(std::move(left)), op(std::move(op)), right(std::move(right)) {};

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visit_binary_expr(shared_from_this());
    }

    const std::shared_ptr<Expr> left;
    const Token op;
    const std::shared_ptr<Expr> right;
};

struct Grouping : Expr, public std::enable_shared_from_this<Grouping> {
    explicit Grouping(std::shared_ptr<Expr> expr)
        : expr(std::move(expr)) {};

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visit_grouping_expr(shared_from_this());
    }

    const std::shared_ptr<Expr> expr;
};

struct Literal : Expr, public std::enable_shared_from_this<Literal> {
    explicit Literal(std::any value) : value(std::move(value)) {};
    std::any accept(ExprVisitor& visitor) override {
        return visitor.visit_literal_expr(shared_from_this());
    }

    const std::any value;
};

struct Logical : Expr, public std::enable_shared_from_this<Logical> {
    Logical(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
        : left(std::move(left)), op(std::move(op)), right(std::move(right)) {};

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visit_logical_expr(shared_from_this());
    }

    const std::shared_ptr<Expr> left;
    const Token op;
    const std::shared_ptr<Expr> right;
};

struct Unary : Expr, public std::enable_shared_from_this<Unary> {
    Unary(Token op, std::shared_ptr<Expr> right) : op(std::move(op)), right(std::move(right)) {};
    std::any accept(ExprVisitor& visitor) override {
        return visitor.visit_unary_expr(shared_from_this());
    }

    const Token op;
    const std::shared_ptr<Expr> right;
};

struct Variable : Expr, public std::enable_shared_from_this<Variable> {
    explicit Variable(Token name) : name(std::move(name)) {};

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visit_variable_expr(shared_from_this());
    }

    const Token name;
};

struct Assign : Expr, public std::enable_shared_from_this<Assign> {
    Assign(Token name, std::shared_ptr<Expr> value)
        : name(std::move(name)), value(std::move(value)) {};

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visit_assign_expr(shared_from_this());
    }

    const Token name;
    const std::shared_ptr<Expr> value;
};

struct Call : Expr, public std::enable_shared_from_this<Call> {
    Call(std::shared_ptr<Expr>  callee, Token  paren, std::vector<std::shared_ptr<Expr>>  arguments)
        : callee(std::move(callee)), paren(std::move(paren)), arguments(std::move(arguments)) {};

    std::any accept(ExprVisitor& visitor) override {
        return visitor.visit_call_expr(shared_from_this());
    }

    const std::shared_ptr<Expr> callee;
    const Token paren;
    const std::vector<std::shared_ptr<Expr>> arguments;
};

