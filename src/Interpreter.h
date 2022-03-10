//
// Created by marco on 3/9/22.
//

#ifndef MINT_INTERPRETER_H
#define MINT_INTERPRETER_H


#include "Expr.h"
#include "Stmt.h"
#include "Environment.h"
#include "MintCallable.h"

class NativeClock : public MintCallable {
public:
    unsigned short arity() override;
    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override;
    std::string to_string() override;
};

class Interpreter : public ExprVisitor, public StmtVisitor {
friend class MintFunction;
public:
    Interpreter();
    void interpret(const std::vector<std::shared_ptr<Stmt>>& statements);
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
    std::any visit_var_stmt(std::shared_ptr<Var> stmt) override;
    std::any visit_if_stmt(std::shared_ptr<If> stmt) override;
    std::any visit_while_stmt(std::shared_ptr<While> stmt) override;
    std::any visit_function_stmt(std::shared_ptr<Function> stmt) override;
    std::any visit_return_stmt(std::shared_ptr<Return> stmt) override;

    std::shared_ptr<Environment> globals{new Environment};
private:
    std::any evaluate(const std::shared_ptr<Expr>& expr);
    void execute(const std::shared_ptr<Stmt>& stmt);
    void execute_block(const std::vector<std::shared_ptr<Stmt>>& statements, std::shared_ptr<Environment> env);
    static void check_number_operand(const Token& op, const std::any& operand);
    static void check_number_operands(const Token& op, const std::any& left, const std::any& right);
    static bool is_truthy(const std::any& object);
    static bool is_equal(const std::any& a, const std::any& b);
    static std::string stringify(const std::any& object);

    std::shared_ptr<Environment> environment{new Environment};
};


#endif //MINT_INTERPRETER_H
