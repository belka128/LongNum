#include "parser.h"
#include <stdexcept>

template<typename T>
Parser<T>::Parser(const std::vector<Token>& tokens) : tokens(tokens), position(0) {}

template<typename T>
std::unique_ptr<Expression<T>> Parser<T>::parse() {
    return parseExpression();
}

template<typename T>
const Token& Parser<T>::peek() {
    return tokens[position];
}

template<typename T>
const Token& Parser<T>::consume() {
    return tokens[position++];
}

template<typename T>
std::unique_ptr<Expression<T>> Parser<T>::parseExpression() {
    auto left = parseTerm();
    while (position < tokens.size() && peek().type == TokenType::Operator) {
        std::string op = std::get<std::string>(peek().value);
        if (op != "+" && op != "-") break;
        consume();
        auto right = parseTerm();
        left = std::make_unique<BinaryOpExpr<T>>(op[0], std::move(left), std::move(right));
    }
    return left;
}

template<typename T>
std::unique_ptr<Expression<T>> Parser<T>::parseTerm() {
    auto left = parseFactor();
    while (position < tokens.size() && peek().type == TokenType::Operator) {
        std::string op = std::get<std::string>(peek().value);
        if (op != "*" && op != "/") break;
        consume();
        auto right = parseFactor();
        left = std::make_unique<BinaryOpExpr<T>>(op[0], std::move(left), std::move(right));
    }
    return left;
}

template<typename T>
std::unique_ptr<Expression<T>> Parser<T>::parseFactor() {
    auto left = parsePrimary();
    while (position < tokens.size() && peek().type == TokenType::Operator) {
        std::string op = std::get<std::string>(peek().value);
        if (op != "^") break;
        consume();
        auto right = parsePrimary();
        left = std::make_unique<BinaryOpExpr<T>>(op[0], std::move(left), std::move(right));
    }
    return left;
}

template<typename T>
std::unique_ptr<Expression<T>> Parser<T>::parsePrimary() {
    const Token& token = consume();
    switch (token.type) {
        case TokenType::Number:
            if constexpr (std::is_same_v<T, double>) {
                return std::make_unique<ConstantExpr<T>>(std::get<double>(token.value));
            } else {
                return std::make_unique<ConstantExpr<T>>(std::get<Complex>(token.value));
            }
        case TokenType::Variable:
            return std::make_unique<VariableExpr<T>>(std::get<std::string>(token.value));
        case TokenType::Function: {
            auto arg = parsePrimary();
            return std::make_unique<FunctionExpr<T>>(std::get<std::string>(token.value), std::move(arg));
        }
        case TokenType::LeftParen: {
            auto expr = parseExpression();
            if (consume().type != TokenType::RightParen) {
                throw Invalid_Operation();
            }
            return expr;
        }
        default:
            throw Invalid_Operation();
    }
}

template class Parser<double>;
template class Parser<std::complex<double>>;