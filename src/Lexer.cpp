#include "Lexer.h"
#include "Mint.h"

auto Lexer::scan_tokens() -> std::vector<Token> {
    while(!is_at_end()) {
        start = current;
        scan_token();
    }

    Token token(token_type::MINT_EOF, "", std::string(""), line);
    tokens.push_back(token);

    return tokens;
}

auto Lexer::is_at_end() -> bool {
    return current >= source.size();
}

auto Lexer::scan_token() -> void {
    auto c = advance();

    switch (c) {
        case '(': add_token(token_type::LEFT_PAREN); break;
        case ')': add_token(token_type::RIGHT_PAREN); break;
        case '{': add_token(token_type::LEFT_BRACE); break;
        case '}': add_token(token_type::RIGHT_BRACE); break;
        case ',': add_token(token_type::COMMA); break;
        case '.': add_token(token_type::DOT); break;
        case '-': add_token(token_type::MINUS); break;
        case '+': add_token(token_type::PLUS); break;
        case ';': add_token(token_type::SEMICOLON); break;
        case '*': add_token(token_type::STAR); break;
        case '^': add_token(token_type::XOR); break;
        case '~': add_token(token_type::NOT); break;
        case '%': add_token(token_type::MODULO); break;
        case '&': add_token(match('&') ? token_type::AND : token_type::BIT_AND); break;
        case '|': add_token(match('|') ? token_type::OR : token_type::BIT_OR); break;
        case '!': add_token(match('=') ? token_type::BANG_EQUAL : token_type::BANG); break;
        case '=': add_token(match('=') ? token_type::EQUAL_EQUAL : token_type::EQUAL); break;
        case '<': {
            if(match('<')) add_token(token_type::LEFT_SHIFT);
            else if(match('=')) add_token(token_type::LESS_EQUAL);
            else add_token(token_type::LESS);
        }
        break;
        case '>': {
            if(match('>')) add_token(token_type::RIGHT_SHIFT);
            else if(match('=')) add_token(token_type::GREATER_EQUAL);
            else add_token(token_type::GREATER);
        }
        break;
        case '/': {
                if(match('/'))
                    while(peek() != '\n' && !is_at_end())
                        advance();
                else add_token(token_type::SLASH);
            }
            break;
        case ' ':
        case '\r':
        case '\t': break;
        case '\n': line++; break;
        case '"': parse_string(); break;
        default: {
            if(is_digit(c)) parse_number();
            else if(is_alpha(c)) identifier();
            else Mint::error(line, "Unexpected character.");
        }
    }
}

auto Lexer::advance() -> char {
    return source.at(current++);
}

auto Lexer::add_token(token_type type) -> void {
    add_token(type, std::string(""));
}

auto Lexer::add_token(token_type type, const std::any& literal) -> void {
    auto text = source.substr(start, current - start);
    Token token(type, text, literal, line);
    tokens.push_back(token);
}

auto Lexer::match(char expected) -> bool {
    if (is_at_end()) return false;
    if (source.at(current) != expected) return false;
    current++;

    return true;
}

auto Lexer::peek() -> char {
    if(is_at_end()) return '\0';

    return source.at(current);
}

auto Lexer::parse_string() -> void {
    while(peek() != '"' && !is_at_end()) {
        if(peek() == '\n') line++;
        advance();
    }

    if(is_at_end()) {
        Mint::error(line, "Unterminated string.");
        return;
    }

    // Advance until closing quote
    advance();
    // Remove quotes from string
    auto value = source.substr(start+1, current - start - 2);
    add_token(token_type::STRING, value);
}


auto Lexer::parse_number() -> void {
    while(isdigit(peek())) advance();

    if(peek() == '.' && is_digit(peek_next())) {
        advance();
        while(is_digit(peek())) advance();
    }

    auto number = std::stod(source.substr(start, current - start));
    add_token(token_type::NUMBER, number);
}

constexpr auto Lexer::is_digit(const char c) -> bool {
    return c >= '0' && c <= '9';
}

auto Lexer::peek_next() -> char {
    if(current - 1 >= source.size()) return '\0';

    return source.at(current + 1);
}

auto Lexer::identifier() -> void {
    while(is_alphanumeric(peek())) advance();
    auto text = source.substr(start, current - start);
    auto found = keywords.find(text);
    token_type type;

    if(found != keywords.end()) type = found->second;
    else type = token_type::IDENTIFIER;

    add_token(type);
}


constexpr auto Lexer::is_alpha(const char c) -> bool {
    return (c >= 'a' && c <= 'z')
        || (c >= 'A' && c <= 'Z')
        || c == '_';
}

constexpr auto Lexer::is_alphanumeric(const char c) -> bool {
    return is_alpha(c) || is_digit(c);
}