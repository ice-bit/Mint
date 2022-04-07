#include <fstream>
#include "test_mint.h"
#include "Token.h"

auto MintTest::SetUp() -> void {
    // Capture error output
    err_buf = std::cerr.rdbuf();
    std::cerr.rdbuf(err_stream.rdbuf());
    // Capture standard output
    std_buf = std::cout.rdbuf();
    std::cout.rdbuf(std_stream.rdbuf());
}

auto MintTest::TearDown() -> void {
    std::cerr.rdbuf(err_buf);
    std::cout.rdbuf(std_buf);
    // reset had_error and had_runtime_error flags
    Mint::had_error = false;
    Mint::had_runtime_error = false;
}

TEST_F(MintTest, TestError) {
    Mint::error(1, "This is a test.");

    auto expected = "[Line 1] Error: This is a test.";
    auto actual = err_stream.str();
    // Remove trailing new line
    actual.erase(actual.length()-1);

    ASSERT_EQ(actual, expected);
}

TEST_F(MintTest, TestErrorWithEOFToken) {
    auto eof_tk = Token(token_type::MINT_EOF, "", std::string(""), 1);
    Mint::error(eof_tk, "This is a test.");

    auto expected = "[Line 1] Error at end: This is a test.";
    auto actual = err_stream.str();
    // Remove trailing new line
    actual.erase(actual.length()-1);

    ASSERT_EQ(actual, expected);
}

TEST_F(MintTest, TestErrorWithCommonToken) {
    auto tk = Token(token_type::LET, "let", std::string(""), 1);
    Mint::error(tk, "This is a test.");

    auto expected = "[Line 1] Error at 'let': This is a test.";
    auto actual = err_stream.str();
    // Remove trailing new line
    actual.erase(actual.length()-1);

    ASSERT_EQ(actual, expected);
}

TEST_F(MintTest, TestSourceFileEvaluation) {
    // Add source code to test file
    std::ofstream file(file_name, std::ios::trunc | std::ios::out | std::ios::binary);
    file << fact_src;
    file.close();
    // Run test file
    auto res = Mint::run_file(file_name);

    ASSERT_EQ(res, 0);

    auto expected = "120.000000";
    auto actual = std_stream.str();
    // Remove trailing new line
    actual.erase(actual.length()-1);

    ASSERT_EQ(actual, expected);
}