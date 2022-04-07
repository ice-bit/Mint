/*
 * Mint interpreter
 * Developed by Marco Cetica (c) 2022
 * Released under GPLv3
 *
 */
#pragma once
#include <any>
#include <vector>
#include <string>

class Interpreter;
class MintCallable {
public:
    virtual unsigned short arity() = 0;
    virtual std::any call(Interpreter& interpreter, std::vector<std::any> arguments) = 0;
    virtual std::string to_string() = 0;
    virtual ~MintCallable() = default;
};
