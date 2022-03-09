//
// Created by marco on 3/9/22.
//

#ifndef MINT_INTERPRETER_H
#define MINT_INTERPRETER_H


#include "Expr.h"
#include "Stmt.h"

class Interpreter : public ExprVisitor, public StmtVisitor {
public:
    void interpret(const std::vector<std::shared_ptr<Stmt>>& statements);
    // Expr abstract class
    std::any visit_binary_expr(std::shared_ptr<Binary> expr) override;
    std::any visit_grouping_expr(std::shared_ptr<Grouping> expr) override;
    std::any visit_literal_expr(std::shared_ptr<Literal> expr) override;
    std::any visit_unary_expr(std::shared_ptr<Unary> expr) override;
    std::any visit_variable_expr(std::shared_ptr<Variable> expr) override;
    // Stmt abstract class
    std::any visit_block_stmt(std::shared_ptr<Block> stmt) override;
    std::any visit_expression_stmt(std::shared_ptr<Expression> stmt) override;
    std::any visit_print_stmt(std::shared_ptr<Print> stmt) override;
    std::any visit_var_stmt(std::shared_ptr<Var> stmt) override;
private:
    std::any evaluate(const std::shared_ptr<Expr>& expr);
    void execute(const std::shared_ptr<Stmt>& stmt);
    static void check_number_operand(const Token& op, const std::any& operand);
    static void check_number_operands(const Token& op, const std::any& left, const std::any& right);
    static bool is_truthy(const std::any& object);
    static bool is_equal(const std::any& a, const std::any& b);
    static std::string stringify(const std::any& object);
};


#endif //MINT_INTERPRETER_H
