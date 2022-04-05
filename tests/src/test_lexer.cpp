//
// Created by Marco Cetica on 04/04/22.
//
#include "gtest/gtest.h"
#include "Lexer.h"

TEST(LexerTest, TestScanTokens) {
    Lexer lx("let str = \"Hello, World\";");
    auto actual = lx.scan_tokens();

    const std::vector<Token> expected {
        Token(token_type::LET, "let", std::string(""), 1),
        Token(token_type::IDENTIFIER, "str", std::string(""), 1),
        Token(token_type::EQUAL, "=", std::string(""), 1),
        Token(token_type::STRING, "\"Hello, World\"", std::string("Hello, World"), 1),
        Token(token_type::SEMICOLON, ";", std::string(""), 1),
        Token(token_type::MINT_EOF, "", std::string(""), 1),

    };

    ASSERT_EQ(actual.size(), expected.size());
    for(auto i = 0UL; i < expected.size(); ++i) {
        ASSERT_EQ(expected[i].type, actual[i].type);
        ASSERT_EQ(expected[i].lexeme, actual[i].lexeme);
        ASSERT_EQ(expected[i].line, actual[i].line);
        ASSERT_EQ(
                std::any_cast<std::string>(expected[i].literal),
                std::any_cast<std::string>(actual[i].literal));
    }
}