//
// Created by marco on 3/9/22.
//

#ifndef MINT_STMT_H
#define MINT_STMT_H

#include <any>
#include <memory>
#include <utility>
#include <vector>
#include "Expr.h"

struct Block;
struct Expression;
struct If;
struct Print;
struct Var;
struct While;

class StmtVisitor {
public:
    virtual std::any visit_block_stmt(std::shared_ptr<Block> stmt) = 0;
    virtual std::any visit_expression_stmt(std::shared_ptr<Expression> stmt) = 0;
    virtual std::any visit_if_stmt(std::shared_ptr<If> stmt) = 0;
    virtual std::any visit_print_stmt(std::shared_ptr<Print> stmt) = 0;
    virtual std::any visit_var_stmt(std::shared_ptr<Var> stmt) = 0;
    virtual std::any visit_while_stmt(std::shared_ptr<While> stmt) = 0;
    virtual ~StmtVisitor() = default;
};

class Stmt {
public:
    virtual std::any accept(StmtVisitor& visitor) = 0;
};

struct Block : Stmt, public std::enable_shared_from_this<Block> {
    explicit Block(std::vector<std::shared_ptr<Stmt>> statements) : statements(std::move(statements)) {};

    std::any accept(StmtVisitor& visitor) override {
        return visitor.visit_block_stmt(shared_from_this());
    }

    const std::vector<std::shared_ptr<Stmt>> statements;
};

struct Expression : Stmt, public std::enable_shared_from_this<Expression> {
    explicit Expression(std::shared_ptr<Expr> expression) : expression(std::move(expression)) {};

    std::any accept(StmtVisitor& visitor) override {
        return visitor.visit_expression_stmt(shared_from_this());
    }

    const std::shared_ptr<Expr> expression;
};

struct If : Stmt, public std::enable_shared_from_this<If> {
    If(std::shared_ptr<Expr>  condition, std::shared_ptr<Stmt>  then_branch, std::shared_ptr<Stmt>  else_branch)
        : condition(std::move(condition)), then_branch(std::move(then_branch)), else_branch(std::move(else_branch)) {};

    std::any accept(StmtVisitor& visitor) override {
        return visitor.visit_if_stmt(shared_from_this());
    }

    const std::shared_ptr<Expr> condition;
    const std::shared_ptr<Stmt> then_branch;
    const std::shared_ptr<Stmt> else_branch;
};

struct Print : Stmt, public std::enable_shared_from_this<Print> {
    explicit Print(std::shared_ptr<Expr> expression) : expression(std::move(expression)) {};

    std::any accept(StmtVisitor& visitor) override {
        return visitor.visit_print_stmt(shared_from_this());
    }

    const std::shared_ptr<Expr> expression;
};

struct Var : Stmt, public std::enable_shared_from_this<Var> {
    Var(Token name, std::shared_ptr<Expr> initializer)
        : name(std::move(name)), initializer(std::move(initializer)) {};

    std::any accept(StmtVisitor& visitor) override {
        return visitor.visit_var_stmt(shared_from_this());
    }

    const Token name;
    const std::shared_ptr<Expr> initializer;
};

struct While : Stmt, public std::enable_shared_from_this<While> {
    While(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body)
        : condition(std::move(condition)), body(std::move(body)) {};

    std::any accept(StmtVisitor& visitor) override {
        return visitor.visit_while_stmt(shared_from_this());
    }

    std::shared_ptr<Expr> condition;
    std::shared_ptr<Stmt> body;
};

#endif //MINT_STMT_H
