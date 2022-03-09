//
// Created by marco on 3/9/22.
//

#ifndef MINT_INTERPRETER_H
#define MINT_INTERPRETER_H


#include "Expr.h"

class Interpreter : public Visitor {
public:
    void interpreter(std::shared_ptr<Expr> expr);
    std::any visitBinaryExpr(std::shared_ptr<Binary> expr);
    std::any visitGroupingExpr(std::shared_ptr<Grouping> expr);
    std::any visitLiteralExpr(std::shared_ptr<Literal> expr);
    std::any visitUnaryExpr(std::shared_ptr<Unary> expr);
private:
    std::any evaluate(std::shared_ptr<Expr> expr);
    void check_number_operand(const Token& op, const std::any& operand);
    void check_number_operands(const Token& op, const std::any& left, const std::any& right);
    bool is_truthy(const std::any& object);
    bool is_equal(const std::any& a, const std::any& b);
    std::string stringify(const std::any& object);
};


#endif //MINT_INTERPRETER_H
