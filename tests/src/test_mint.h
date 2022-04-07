/*
 * Mint interpreter
 * Developed by Marco Cetica (c) 2022
 * Released under GPLv3
 *
 */
#pragma once
#include "Mint.h"
#include "gtest/gtest.h"
#include <filesystem>

class MintTest : public ::testing::Test {
protected:
    auto SetUp() -> void override;
    auto TearDown() -> void override;

    std::streambuf *err_buf;
    std::streambuf *std_buf;
    std::ostringstream err_stream;
    std::ostringstream std_stream;
    const std::string file_name = std::filesystem::absolute("mint_test.js");
    const std::string fact_src =
            "function fact(n) {\n" \
            "if(n == 0) return 1;\n" \
            "return fact(n-1) * n;\n"\
            "}\n"\
            "print fact(5); // 120\n";
};