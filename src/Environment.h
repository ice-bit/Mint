//
// Created by marco on 3/9/22.
//

#ifndef MINT_ENVIRONMENT_H
#define MINT_ENVIRONMENT_H

#include <memory>
#include <any>
#include <map>
#include "Token.h"

class Environment : public std::enable_shared_from_this<Environment> {
friend class Interpreter;
public:
    Environment() : enclosing(nullptr) {};
    explicit Environment(std::shared_ptr<Environment> enclosing) : enclosing(std::move(enclosing)) {};
    auto get(const Token& name) -> std::any;
    auto assign(const Token& name, std::any value) -> void;
    auto define(const std::string& name, std::any value) -> void;
    auto assign_at(unsigned int distance, const Token& name, std::any value) -> void;
    auto ancestor(unsigned int distance) -> std::shared_ptr<Environment>;
    auto get_at(unsigned int distance, const std::string& name) -> std::any;
private:
    std::shared_ptr<Environment> enclosing;
    std::map<std::string, std::any> values;
};

#endif //MINT_ENVIRONMENT_H
