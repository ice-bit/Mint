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
