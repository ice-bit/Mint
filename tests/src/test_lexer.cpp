//
// Created by Marco Cetica on 04/04/22.
//
#include "gtest/gtest.h"
#include "Lexer.h"

TEST(LexerTest, TestScanTokens) {
    Lexer lx("let str = \"Hello, World\";");
    auto tokens = lx.scan_tokens();
    std::vector<std::string> expected {
        "let", "str", "="
    };

    for(auto i = tokens.begin(); i != tokens.end(); i++)
        
}