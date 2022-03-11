//
// Created by marco on 3/9/22.
//

#include <iostream>
#include <utility>
#include <chrono>
#include "Interpreter.h"
#include "RuntimeError.h"
#include "Mint.h"
#include "Environment.h"
#include "MintFunction.h"

#define UNUSED(x) (void)(x)

Interpreter::Interpreter() {
    globals->define("clock", std::shared_ptr<NativeClock>{});
}

void Interpreter::interpret(const std::vector<std::shared_ptr<Stmt>>& statements) {
    try {
        for(const auto &statement : statements)
            execute(statement);
    } catch(const RuntimeError& err) {
        Mint::runtime_error(err);
    }
}

void Interpreter::resolve(const std::shared_ptr<Expr>& expr, const int depth) {
    locals[expr] = depth;
}

void Interpreter::execute_block(const std::vector<std::shared_ptr<Stmt>>& statements,
                                std::shared_ptr<Environment> env) {
    std::shared_ptr<Environment> previous = this->environment;
    try {
        this->environment = std::move(env);

        for(const auto& statement : statements)
            execute(statement);
    } catch(...) {
        this->environment = previous;
        throw;
    }

    this->environment = previous;
}

void Interpreter::execute(const std::shared_ptr<Stmt>& stmt) {
    stmt->accept(*this);
}

std::any Interpreter::evaluate(const std::shared_ptr<Expr>& expr) {
    return expr->accept(*this);
}

std::any Interpreter::lookup_variable(const Token& name, const std::shared_ptr<Expr>& expr) {
    auto elem = locals.find(expr);
    if(elem != locals.end()) {
        auto distance = elem->second;
        return environment->get_at(distance, name.lexeme);
    } else return globals->get(name);
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
        default: break;
    }

    return {};
}

std::any Interpreter::visit_grouping_expr(std::shared_ptr<Grouping> expr) {
    return evaluate(expr->expr);
}

std::any Interpreter::visit_literal_expr(std::shared_ptr<Literal> expr) {
    return expr->value;
}

std::any Interpreter::visit_logical_expr(std::shared_ptr<Logical> expr) {
    auto left = evaluate(expr->left);

    if(expr->op.type == token_type::OR) {
        if (is_truthy(left)) return left;
    }
    else {
        if (!is_truthy(left)) return left;
    }

    return evaluate(expr->right);
}

std::any Interpreter::visit_unary_expr(std::shared_ptr<Unary> expr) {
    std::any right = evaluate(expr->right);

    switch (expr->op.type) {
        case token_type::BANG: return !is_truthy(right);
        case token_type::MINUS:
            check_number_operand(expr->op, right);
            return -std::any_cast<double>(right);
        default: break;
    }

    return {};
}

std::any Interpreter::visit_variable_expr(std::shared_ptr<Variable> expr) {
    return lookup_variable(expr->name, expr);
}

std::any Interpreter::visit_assign_expr(std::shared_ptr<Assign> expr) {
    auto value = evaluate(expr->value);
    auto elem = locals.find(expr);

    if(elem != locals.end()) {
        auto distance = elem->second;
        environment->assign_at(distance, expr->name, value);
    } else globals->assign(expr->name, value);

    return value;
}

std::any Interpreter::visit_call_expr(std::shared_ptr<Call> expr) {
    auto callee = evaluate(expr->callee);

    std::vector<std::any> arguments;
    for(auto &argument : expr->arguments)
        arguments.push_back(evaluate(argument));

    std::shared_ptr<MintCallable> function;

    if(callee.type() == typeid(std::shared_ptr<MintFunction>))
        function = std::any_cast<std::shared_ptr<MintFunction>>(callee);
    else
        throw RuntimeError(expr->paren, "Can only call functions and classes.");

    if(arguments.size() != function->arity()) {
        throw RuntimeError(expr->paren, "Expected " +
            std::to_string(function->arity()) + " arguments but got " +
            std::to_string(arguments.size()) + ".");
    }

    return function->call(*this, std::move(arguments));
}

std::any Interpreter::visit_block_stmt(std::shared_ptr<Block> stmt) {
    execute_block(stmt->statements, std::make_shared<Environment>(environment));

    return {};
}

std::any Interpreter::visit_expression_stmt(std::shared_ptr<Expression> stmt) {
    evaluate(stmt->expression);

    return {};
}

std::any Interpreter::visit_print_stmt(std::shared_ptr<Print> stmt) {
    std::any value = evaluate(stmt->expression);
    std::cout << stringify(value) << std::endl;

    return {};
}

std::any Interpreter::visit_variable_stmt(std::shared_ptr<Var> stmt) {
    std::any value = nullptr;
    if(stmt->initializer != nullptr)
        value = evaluate(stmt->initializer);

    environment->define(stmt->name.lexeme, std::move(value));

    return {};
}

std::any Interpreter::visit_if_stmt(std::shared_ptr<If> stmt) {
    if(is_truthy(evaluate(stmt->condition)))
        execute(stmt->then_branch);
    else if(stmt->else_branch != nullptr)
        execute(stmt->else_branch);

    return {};
}

std::any Interpreter::visit_while_stmt(std::shared_ptr<While> stmt) {
    while(is_truthy(evaluate(stmt->condition)))
        execute(stmt->body);

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
    if(object.type() == typeid(std::shared_ptr<MintFunction>))
        return std::any_cast<std::shared_ptr<MintFunction>>(object)->to_string();

    return "Error in 'stringify': object type not supported.";
}

std::any Interpreter::visit_function_stmt(std::shared_ptr<Function> stmt) {
    auto function = std::make_shared<MintFunction>(stmt, environment);
    environment->define(stmt->name.lexeme, function);

    return {};
}

std::any Interpreter::visit_return_stmt(std::shared_ptr<Return> stmt) {
    std::any value = nullptr;

    if(stmt->value != nullptr) value = evaluate(stmt->value);

    throw MintReturn{value};
}

unsigned short NativeClock::arity() {
    return 0;
}

std::any NativeClock::call(Interpreter &interpreter, std::vector<std::any> arguments) {
    auto ticks = std::chrono::system_clock::now().time_since_epoch();

    UNUSED(interpreter);
    UNUSED(arguments);

    return std::chrono::duration<double>(ticks).count() / 1000.0;
}

std::string NativeClock::to_string() {
    return "<native fn>";
}