//
// Created by marco on 3/9/22.
//

#include <iostream>
#include <utility>
#include "Interpreter.h"
#include "RuntimeError.h"
#include "Mint.h"

void Interpreter::interpret(const std::shared_ptr<Expr>& expr) {
    try {
        std::any value = evaluate(expr);
        std::cout << stringify(value) << std::endl;
    } catch(const RuntimeError& err) {
        Mint::runtime_error(err);
    }
}

std::any Interpreter::evaluate(const std::shared_ptr<Expr>& expr) {
    return expr->accept(*this);
}

std::any Interpreter::visit_binary_expr(std::shared_ptr<Binary> expr)  {
    std::any left = evaluate(expr->left);
    std::any right = evaluate(expr->right);

    switch (expr->op.type) {
        case token_type::BANG_EQUAL: return !is_equal(left, right);
        case token_type::EQUAL_EQUAL: return is_equal(left, right);
        case token_type::GREATER:
            check_number_operands(expr->op, left, right);
            return std::any_cast<double>(left) > std::any_cast<double>(right);
        case token_type::GREATER_EQUAL:
            check_number_operands(expr->op, left, right);
            return std::any_cast<double>(left) >= std::any_cast<double>(right);
        case token_type::LESS:
            check_number_operands(expr->op, left, right);
            return std::any_cast<double>(left) < std::any_cast<double>(right);
        case token_type::LESS_EQUAL:
            check_number_operands(expr->op, left, right);
            return std::any_cast<double>(left) <= std::any_cast<double>(right);
        case token_type::MINUS:
            check_number_operands(expr->op, left, right);
            return std::any_cast<double>(left) - std::any_cast<double>(right);
        case token_type::PLUS:
            if(left.type() == typeid(double) && right.type() == typeid(double))
                return std::any_cast<double>(left) + std::any_cast<double>(right);
            if(left.type() == typeid(std::string) && right.type() == typeid(std::string))
                return std::any_cast<std::string>(left) + std::any_cast<std::string>(right);

            throw RuntimeError(expr->op, "Operands must be two numbers or two strings.");
        case token_type::SLASH:
            check_number_operands(expr->op, left, right);
            return std::any_cast<double>(left) / std::any_cast<double>(right);
        case token_type::STAR:
            check_number_operands(expr->op, left, right);
            return std::any_cast<double>(left) * std::any_cast<double>(right);
    }

    return {};
}


std::any Interpreter::visit_grouping_expr(std::shared_ptr<Grouping> expr) {
    return evaluate(expr->expr);
}

std::any Interpreter::visit_literal_expr(std::shared_ptr<Literal> expr) {
    return expr->value;
}

std::any Interpreter::visit_unary_expr(std::shared_ptr<Unary> expr) {
    std::any right = evaluate(expr->right);

    switch (expr->op.type) {
        case token_type::BANG: return !is_truthy(right);
        case token_type::MINUS:
            check_number_operand(expr->op, right);
            return -std::any_cast<double>(right);
    }

    return {};
}

void Interpreter::check_number_operand(const Token &op, const std::any &operand) {
    if(operand.type() == typeid(double)) return;
    throw RuntimeError(op, "Operand must be a number.");
}

void Interpreter::check_number_operands(const Token &op, const std::any &left, const std::any &right) {
    if(left.type() == typeid(double) && right.type() == typeid(double)) return;

    throw RuntimeError(op, "Operands must be numbers.");
}

bool Interpreter::is_truthy(const std::any &object) {
    if(object.type() == typeid(nullptr)) return false;
    if(object.type() == typeid(bool))
        return std::any_cast<bool>(object);

    return true;
}

bool Interpreter::is_equal(const std::any &a, const std::any &b) {
    if(a.type() == typeid(nullptr) && b.type() == typeid(nullptr)) return true;
    if(a.type() == typeid(nullptr)) return false;
    if(a.type() == typeid(std::string) && b.type() == typeid(std::string))
        return std::any_cast<std::string>(a) == std::any_cast<std::string>(b);
    if(a.type() == typeid(double) && b.type() == typeid(double))
        return std::any_cast<double>(a) == std::any_cast<double>(b);
    if(a.type() == typeid(bool) && b.type() == typeid(bool))
        return std::any_cast<bool>(a) == std::any_cast<bool>(b);

    return false;
}

std::string Interpreter::stringify(const std::any &object) {
    if(object.type() == typeid(nullptr)) return "nil";
    if(object.type() == typeid(double)) {
        std::string text = std::to_string(std::any_cast<double>(object));
        if(text[text.length() - 2] == '.' && text[text.length() - 1] == '0')
            text = text.substr(0, text.length() - 2);

        return text;
    }

    if(object.type() == typeid(std::string))
        return std::any_cast<std::string>(object);
    if(object.type() == typeid(bool))
        return std::any_cast<bool>(object) ? "true" : "false";

    return "Error in 'stringify': object type not supported.";
}
