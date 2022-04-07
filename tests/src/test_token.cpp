#include "Token.h"
#include "gtest/gtest.h"

TEST(TokenTest, TestCtor) {
    Token tk(token_type::LEFT_PAREN, "(", std::string("("), 0);

    ASSERT_EQ(tk.type, token_type::LEFT_PAREN);
    ASSERT_EQ(tk.lexeme, "(");
    ASSERT_EQ(std::any_cast<std::string>(tk.literal), std::string("("));
    ASSERT_EQ(tk.line, 0);
}

TEST(TokenTest, TestToString) {
    Token tk(token_type::LEFT_PAREN, "(", std::string("("), 0);
    auto expected = std::string( std::to_string((double)0.0) + " ( nil");
    auto actual = tk.to_string();

    ASSERT_EQ(expected, actual);
}