/*
 * Mint interpreter
 * Developed by Marco Cetica (c) 2022
 * Released under GPLv3
 *
 */
#include "Environment.h"
#include "RuntimeError.h"

auto Environment::get(const Token &name) -> std::any {
    auto element = values.find(name.lexeme);
    if(element != values.end()) return element->second;

    if(enclosing != nullptr) return enclosing->get(name);

    throw RuntimeError(name, std::string("Undefined variable '" + name.lexeme + "'"));
}

auto Environment::assign(const Token &name, std::any value) -> void {
    auto element = values.find(name.lexeme);
    if(element != values.end()) {
        element->second = std::move(value);
        return;
    }

    if(enclosing != nullptr) {
        enclosing->assign(name, std::move(value));
        return;
    }

    throw RuntimeError(name, std::string("Undefined variable '" + name.lexeme + "'"));
}

auto Environment::define(const std::string &name, std::any value) -> void {
    values[name] = std::move(value);
}

auto Environment::ancestor(const unsigned int distance) -> std::shared_ptr<Environment> {
    auto env = shared_from_this();

    for(size_t i = 0; i < distance; i++)
        env = env->enclosing;

    return env;
}

auto Environment::get_at(const unsigned int distance, const std::string& name) -> std::any {
    return ancestor(distance)->values[name];
}

auto Environment::assign_at(unsigned int distance, const Token& name, std::any value) -> void {
    ancestor(distance)->values[name.lexeme] = std::move(value);
}
