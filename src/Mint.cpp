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

// Default error state
bool Mint::had_error = false;
bool Mint::had_runtime_error = false;

void Mint::run_file(const std::string &filepath) {
    std::fstream source_file(filepath, std::ios::in | std::ios::binary);
    std::stringstream buf;

    buf << source_file.rdbuf();
    run(buf.str());

    if(had_error) exit(65);
}

void Mint::run(std::string source) {
    auto lexer(new Lexer(std::move(source)));
    auto tokens = lexer->scan_tokens();
    Parser parser{tokens};
    Interpreter interpreter{};
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();

    if(had_error) return;

    interpreter.interpret(statements);
}

void Mint::run_prompt() {
    std::string user_input;

    while(true) {
        std::cout << "λ> ";
        std::getline(std::cin, user_input);
        run(user_input);
        had_error = false;
    }
}

void Mint::error(unsigned int line, const std::string &msg) {
    report(line, "", msg);
}

void Mint::report(unsigned int line, const std::string &pos, const std::string& reason) {
    std::cerr << "[Line " << line << "] Error " << pos << ": " << reason << std::endl;
    had_error = true;
}

void Mint::error(const Token& token, const std::string& msg) {
    if(token.type == token_type::MINT_EOF)
        report(token.line, " at end", msg);
    else
        report(token.line, " at '" + token.lexeme + "'", msg);
}

void Mint::runtime_error(const RuntimeError &err) {
    std::cerr << "[Line " << err.token.line << "] " << err.what() << std::endl;
    had_runtime_error = true;
}