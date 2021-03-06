/*
 * Mint interpreter
 * Developed by Marco Cetica (c) 2022
 * Released under GPLv3
 *
 */
#pragma once
#include "Stmt.h"
#include "Interpreter.h"

class Resolver : public ExprVisitor, public StmtVisitor {
public:
    explicit Resolver(Interpreter& interpreter) : interpreter(interpreter) {};
    void resolve(const std::vector<std::shared_ptr<Stmt>>& statements);
    // Stmt abstract class
    std::any visit_block_stmt(std::shared_ptr<Block> stmt) override;
    std::any visit_expression_stmt(std::shared_ptr<Expression> stmt) override;
    std::any visit_function_stmt(std::shared_ptr<Function> stmt) override;
    std::any visit_if_stmt(std::shared_ptr<If> stmt) override;
    std::any visit_print_stmt(std::shared_ptr<Print> stmt) override;
    std::any visit_return_stmt(std::shared_ptr<Return> stmt) override;
    std::any visit_variable_stmt(std::shared_ptr<Var> stmt) override;
    std::any visit_while_stmt(std::shared_ptr<While> stmt) override;
    // Expr abstract class
    std::any visit_assign_expr(std::shared_ptr<Assign> expr) override;
    std::any visit_binary_expr(std::shared_ptr<Binary> expr) override;
    std::any visit_call_expr(std::shared_ptr<Call> expr) override;
    std::any visit_grouping_expr(std::shared_ptr<Grouping> expr) override;
    std::any visit_literal_expr(std::shared_ptr<Literal> expr) override;
    std::any visit_logical_expr(std::shared_ptr<Logical> expr) override;
    std::any visit_unary_expr(std::shared_ptr<Unary> expr) override;
    std::any visit_variable_expr(std::shared_ptr<Variable> expr) override;

private:
    enum class function_type {
        NONE,
        FUNCTION
    };
    auto resolve(const std::shared_ptr<Stmt>& stmt) -> void;
    auto resolve(const std::shared_ptr<Expr>& expr) -> void;
    auto resolve_function(const std::shared_ptr<Function>& function, function_type type) -> void;
    auto begin_scope() -> void;
    auto end_scope() -> void;
    auto declare(const Token& name) -> void;
    auto define(const Token& name) -> void;
    auto resolve_local(const std::shared_ptr<Expr>& expr, const Token& name) -> void;

    Interpreter& interpreter;
    std::vector<std::map<std::string, bool>> scopes;
    function_type current_fun = function_type::NONE;
};

