//
// Created by marco on 3/8/22.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>

#include "Mint.h"
#include "Lexer.h"
#include "Parser.h"
#include "Interpreter.h"
#include "MintFunction.h"
#include "Resolver.h"

// Default error state
bool Mint::had_error = false;
bool Mint::had_runtime_error = false;
Interpreter interpreter;

auto Mint::run_file(const std::string &filepath) -> void {
    std::fstream source_file(filepath, std::ios::in | std::ios::binary);
    if(source_file.fail()) {
        std::cerr << "Cannot open source file \"" << filepath << "\"." << std::endl;
        std::exit(1);
    }

    std::stringstream buf;
    uint8_t exit_code = 0;

    buf << source_file.rdbuf();
    run(buf.str());

    if(had_error) exit_code = 65;
    if(had_runtime_error) exit_code = 70;

    source_file.close();
    std::exit(exit_code);
}


auto Mint::run(std::string source) -> void {
    auto lexer(new Lexer(std::move(source)));
    auto tokens = lexer->scan_tokens();
    Parser parser{tokens};
    auto statements = parser.parse();
    // Catch syntax errors(managed by the parser)
    if(had_error) return;

    Resolver resolver(interpreter);
    resolver.resolve(statements);

    // Catch execution errors(managed by the resolver)
    if(had_error) return;

    interpreter.interpret(statements);
}

auto Mint::run_prompt() -> void {
    std::string user_input;

    while(true) {
        std::cout << "λ> ";
        std::getline(std::cin, user_input);
        run(user_input);
        had_error = false;
    }
}

auto Mint::error(unsigned int line, const std::string &msg) -> void {
    report(line, "", msg);
}

auto Mint::error(const Token& token, const std::string& msg) -> void {
    if(token.type == token_type::MINT_EOF)
        report(token.line, " at end", msg);
    else
        report(token.line, " at '" + token.lexeme + "'", msg);
}

auto Mint::report(unsigned int line, const std::string &pos, const std::string& reason) -> void {
    std::cerr << "[Line " << line << "] Error" << pos << ": " << reason << std::endl;
    had_error = true;
}

auto Mint::runtime_error(const RuntimeError &err) -> void {
    std::cerr << "[Line " << err.token.line << "] " << err.what() << std::endl;
    had_runtime_error = true;
}