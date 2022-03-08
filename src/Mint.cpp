//
// Created by marco on 3/8/22.
//

#include <fstream>
#include <sstream>
#include <iostream>

#include "Mint.h"

// Default error state
bool Mint::had_error = false;

void Mint::run_file(const std::string &filepath) {
    std::fstream source_file(filepath, std::ios::in | std::ios::binary);
    std::stringstream buf;

    buf << source_file.rdbuf();
    run(buf.str());

    if(had_error) exit(65);
}

void Mint::run(std::string source) {
    // TODO: scanner, parser, etc.
    (void)source;
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

void Mint::report(unsigned int line, const std::string &pos, const std::string &reason) {
    std::cerr << "[Line " << line << "] Error " << pos << ": " << reason << std::endl;
    had_error = true;
}