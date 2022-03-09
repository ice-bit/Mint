//
// Created by marco on 3/8/22.
//

#ifndef MINT_MINT_H
#define MINT_MINT_H

#include <string>
#include "Token.h"
#include "RuntimeError.h"

class Mint {
public:
    static void run_file(const std::string& filepath);
    static void error(unsigned int line, const std::string& msg);
    static void error(const Token& token, std::string_view msg);
    static void runtime_error(const RuntimeError& err);
    [[noreturn]] static void run_prompt();
private:
    static void run(std::string source);
    static void report(unsigned int line, const std::string& pos, std::string_view reason);
    static bool had_error;
    static bool had_runtime_error;
};

#endif //MINT_MINT_H
