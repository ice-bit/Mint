//
// Created by marco on 3/9/22.
//

#ifndef MINT_INTERPRETER_H
#define MINT_INTERPRETER_H


#include "Expr.h"

class Interpreter : public ExprVisitor {
public:
    void interpret(const std::shared_ptr<Expr>& expr);
    std::any visit_binary_expr(std::shared_ptr<Binary> expr) override;
    std::any visit_grouping_expr(std::shared_ptr<Grouping> expr) override;
    std::any visit_literal_expr(std::shared_ptr<Literal> expr) override;
    std::any visit_unary_expr(std::shared_ptr<Unary> expr) override;
private:
    std::any evaluate(const std::shared_ptr<Expr>& expr);
    static void check_number_operand(const Token& op, const std::any& operand);
    static void check_number_operands(const Token& op, const std::any& left, const std::any& right);
    static bool is_truthy(const std::any& object);
    static bool is_equal(const std::any& a, const std::any& b);
    static std::string stringify(const std::any& object);
};


#endif //MINT_INTERPRETER_H
