//
// Created by marco on 3/9/22.
//

#include "Environment.h"
#include "RuntimeError.h"

std::any Environment::get(const Token &name) {
    auto element = values.find(name.lexeme);
    if(element != values.end()) return element->second;

    if(enclosing != nullptr) return enclosing->get(name);

    throw RuntimeError(name, std::string("Undefined variable '" + name.lexeme + "'"));
}

void Environment::assign(const Token &name, std::any value) {
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

void Environment::define(const std::string &name, std::any value) {
    values[name] = std::move(value);
}

std::shared_ptr<Environment> Environment::ancestor(const unsigned int distance) {
    std::shared_ptr<Environment> env = shared_from_this();

    for(size_t i = 0; i < distance; i++)
        env = env->enclosing;

    return env;
}

std::any Environment::get_at(const unsigned int distance, const std::string& name) {
    return ancestor(distance)->values[name];
}

void Environment::assign_at(unsigned int distance, const Token& name, std::any value) {
    ancestor(distance)->values[name.lexeme] = std::move(value);
}
