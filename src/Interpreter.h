/*
 * Mint interpreter
 * Developed by Marco Cetica (c) 2022
 * Released under GPLv3
 *
 */
#pragma once
#include "Expr.h"
#include "Stmt.h"
#include "Environment.h"
#include "MintCallable.h"

class Interpreter : public ExprVisitor, public StmtVisitor {
    friend class MintFunction;
public:
    Interpreter();
    auto interpret(const std::vector<std::shared_ptr<Stmt>>& statements) -> void;
    auto resolve(const std::shared_ptr<Expr>& expr, int depth) -> void;
    // Expr abstract class
    std::any visit_binary_expr(std::shared_ptr<Binary> expr) override;
    std::any visit_grouping_expr(std::shared_ptr<Grouping> expr) override;
    std::any visit_literal_expr(std::shared_ptr<Literal> expr) override;
    std::any visit_logical_expr(std::shared_ptr<Logical> expr) override;
    std::any visit_unary_expr(std::shared_ptr<Unary> expr) override;
    std::any visit_variable_expr(std::shared_ptr<Variable> expr) override;
    std::any visit_assign_expr(std::shared_ptr<Assign> expr) override;
    std::any visit_call_expr(std::shared_ptr<Call> expr) override;
    // Stmt abstract class
    std::any visit_block_stmt(std::shared_ptr<Block> stmt) override;
    std::any visit_expression_stmt(std::shared_ptr<Expression> stmt) override;
    std::any visit_print_stmt(std::shared_ptr<Print> stmt) override;
    std::any visit_variable_stmt(std::shared_ptr<Var> stmt) override;
    std::any visit_if_stmt(std::shared_ptr<If> stmt) override;
    std::any visit_while_stmt(std::shared_ptr<While> stmt) override;
    std::any visit_function_stmt(std::shared_ptr<Function> stmt) override;
    std::any visit_return_stmt(std::shared_ptr<Return> stmt) override;

    std::shared_ptr<Environment> globals{new Environment};
private:
    auto evaluate(const std::shared_ptr<Expr>& expr);
    auto lookup_variable(const Token& name, const std::shared_ptr<Expr>& expr);
    auto execute(const std::shared_ptr<Stmt>& stmt) -> void;
    auto execute_block(const std::vector<std::shared_ptr<Stmt>>& statements, std::shared_ptr<Environment> env) -> void;
    static auto check_number_operand(const Token& op, const std::any& operand) -> void;
    static auto check_number_operands(const Token& op, const std::any& left, const std::any& right) -> void;
    static auto is_truthy(const std::any& object) -> bool;
    static auto is_equal(const std::any& a, const std::any& b) -> bool;
    static auto stringify(const std::any& object) -> std::string;

    std::shared_ptr<Environment> environment = globals;
    std::map<std::shared_ptr<Expr>, int> locals;
};
