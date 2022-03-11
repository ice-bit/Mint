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
    std::any get(const Token& name);
    void assign(const Token& name, std::any value);
    void define(const std::string& name, std::any value);
    void assign_at(unsigned int distance, const Token& name, std::any value);
    std::shared_ptr<Environment> ancestor(unsigned int distance);
    std::any get_at(unsigned int distance, const std::string& name);
private:
    std::shared_ptr<Environment> enclosing;
    std::map<std::string, std::any> values;
};

#endif //MINT_ENVIRONMENT_H
