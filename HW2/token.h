#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include <variant>
#include <complex>

using Complex = std::complex<double>;

enum class TokenType {
    Number,
    Variable,
    Function,
    Operator,
    LeftParen,
    RightParen,
    EndOfInput
};

using TokenValue = std::variant<double, std::complex<double>, std::string>;

struct Token {
    TokenType type;
    TokenValue value;
};

#endif