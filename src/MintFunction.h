/*
 * Mint interpreter
 * Developed by Marco Cetica (c) 2022
 * Released under GPLv3
 *
 */
#pragma once
#include <memory>
#include <utility>
#include "MintCallable.h"

class Environment;
struct Function;

class MintReturn {
public:
    const std::any value;
};

class MintFunction : public MintCallable {
public:
    MintFunction(std::shared_ptr<Function> declaration, std::shared_ptr<Environment> closure)
        : declaration(std::move(declaration)), closure(std::move(closure)) {};
    std::string to_string() override;
    unsigned short arity() override;
    std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override;
private:
    std::shared_ptr<Function> declaration;
    std::shared_ptr<Environment> closure;
};


