#include "Resolver.h"
#include "Mint.h"

#define UNUSED(x) (void)(x)

auto Resolver::resolve(const std::vector<std::shared_ptr<Stmt>> &statements) -> void {
    for(auto& stmt : statements) resolve(stmt);
}

auto Resolver::resolve(const std::shared_ptr<Stmt>& stmt) -> void {
    stmt->accept(*this);
}

auto Resolver::resolve(const std::shared_ptr<Expr>& expr) -> void {
    expr->accept(*this);
}

auto Resolver::resolve_function(const std::shared_ptr<Function>& function, function_type type) -> void {
    function_type enclosing_fun = current_fun;
    current_fun = type;

    begin_scope();
    for(const auto& par : function->params) {
        declare(par);
        define(par);
    }
    resolve(function->body);
    end_scope();
    current_fun = enclosing_fun;
}

auto Resolver::begin_scope() -> void {
    scopes.emplace_back();
}

auto Resolver::end_scope() -> void {
    scopes.pop_back();
}

auto Resolver::declare(const Token &name) -> void {
    if(scopes.empty()) return;

    auto& scope = scopes.back();
    if(scope.find(name.lexeme) != scope.end())
        Mint::error(name, "Already a variable with this name in this scope.");

    scope[name.lexeme] = false;
}

auto Resolver::define(const Token &name) -> void {
    if(scopes.empty()) return;
    scopes.back()[name.lexeme] = true;
}

auto Resolver::resolve_local(const std::shared_ptr<Expr>& expr, const Token &name) -> void {
    for(auto i = (signed)scopes.size()-1; i >= 0; i--) {
        if(scopes[i].find(name.lexeme) != scopes[i].end()) {
            interpreter.resolve(expr, (signed)(scopes.size() - 1 - i));
            return;
        }
    }
}

std::any Resolver::visit_block_stmt(std::shared_ptr<Block> stmt) {
    begin_scope();
    resolve(stmt->statements);
    end_scope();

    return {};
}

std::any Resolver::visit_expression_stmt(std::shared_ptr<Expression> stmt) {
    resolve(stmt->expression);

    return {};
}

std::any Resolver::visit_function_stmt(std::shared_ptr<Function> stmt) {
    declare(stmt->name);
    define(stmt->name);

    resolve_function(stmt, function_type::FUNCTION);

    return {};
}

std::any Resolver::visit_if_stmt(std::shared_ptr<If> stmt) {
    resolve(stmt->condition);
    resolve(stmt->then_branch);

    if(stmt->else_branch != nullptr) resolve(stmt->else_branch);

    return {};
}

std::any Resolver::visit_print_stmt(std::shared_ptr<Print> stmt) {
    resolve(stmt->expression);

    return {};
}

std::any Resolver::visit_return_stmt(std::shared_ptr<Return> stmt) {
    if(current_fun == function_type::NONE)
        Mint::error(stmt->keyword, "Can't return from top-level code.");

    if(stmt->value != nullptr) resolve(stmt->value);

    return {};
}

std::any Resolver::visit_variable_stmt(std::shared_ptr<Var> stmt) {
    declare(stmt->name);

    if(stmt->initializer != nullptr) resolve(stmt->initializer);
    define(stmt->name);

    return {};
}

std::any Resolver::visit_while_stmt(std::shared_ptr<While> stmt) {
    resolve(stmt->condition);
    resolve(stmt->body);

    return {};
}

std::any Resolver::visit_assign_expr(std::shared_ptr<Assign> expr) {
    resolve(expr->value);
    resolve_local(expr, expr->name);

    return {};
}

std::any Resolver::visit_binary_expr(std::shared_ptr<Binary> expr) {
    resolve(expr->left);
    resolve(expr->right);

    return {};
}

std::any Resolver::visit_call_expr(std::shared_ptr<Call> expr) {
    resolve(expr->callee);

    for(const auto& arg : expr->arguments) resolve(arg);

    return {};
}

std::any Resolver::visit_grouping_expr(std::shared_ptr<Grouping> expr) {
    resolve(expr->expr);

    return {};
}

std::any Resolver::visit_literal_expr(std::shared_ptr<Literal> expr) {
    UNUSED(expr);
    return {};
}

std::any Resolver::visit_logical_expr(std::shared_ptr<Logical> expr) {
    resolve(expr->left);
    resolve(expr->right);

    return {};
}

std::any Resolver::visit_unary_expr(std::shared_ptr<Unary> expr) {
    resolve(expr->right);

    return {};
}

std::any Resolver::visit_variable_expr(std::shared_ptr<Variable> expr) {
    if(!scopes.empty()) {
        auto& scope = scopes.back();
        auto elem = scope.find(expr->name.lexeme);
        if(elem != scope.end() && !elem->second)
            Mint::error(expr->name, "Can't read local variable in its own initializer.");
    }

    resolve_local(expr, expr->name);

    return {};
}