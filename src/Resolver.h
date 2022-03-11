//
// Created by marco on 3/11/22.
//

#ifndef MINT_RESOLVER_H
#define MINT_RESOLVER_H


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
    Interpreter& interpreter;
    std::vector<std::map<std::string, bool>> scopes;
    enum class function_type {
        NONE,
        FUNCTION
    };
    function_type current_fun = function_type::NONE;
};


#endif //MINT_RESOLVER_H
