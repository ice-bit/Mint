//
// Created by marco on 3/8/22.
//

#pragma once
#include <string>
#include "Token.h"
#include "RuntimeError.h"
#include "Interpreter.h"

class Mint {
public:
    static auto run_file(const std::string& filepath) -> void;
    static auto error(unsigned int line, const std::string& msg) -> void;
    static auto error(const Token& token, const std::string& msg) -> void;
    static auto runtime_error(const RuntimeError& err) -> void;
    static auto run_prompt() -> void;
private:
    static auto run(std::string source) -> void;
    static auto report(unsigned int line, const std::string& pos, const std::string& reason) -> void;
    static bool had_error;
    static bool had_runtime_error;
};

