/*
 * Mint interpreter
 * Developed by Marco Cetica (c) 2022
 * Released under GPLv3
 *
 */
#pragma once
#include <stdexcept>
#include "Token.h"


class RuntimeError : public std::runtime_error {
public:
    RuntimeError(const Token& token, std::string_view msg)
        : std::runtime_error(msg.data()), token(token) {};

    const Token& token;
};


