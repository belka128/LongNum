#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include <memory>
#include "token.h"
#include "expression.h"

template<typename T>
class Parser {
public:
    Parser(const std::vector<Token>& tokens); //конструктор
    std::unique_ptr<Expression<T>> parse();
private:
    std::vector<Token> tokens;
    size_t position;

    const Token& peek(); //текущий токен
    const Token& consume();//текущий токен и идем к некст
    std::unique_ptr<Expression<T>> parseExpression(); //+ -
    std::unique_ptr<Expression<T>> parseTerm();//* /
    std::unique_ptr<Expression<T>> parseFactor();//^
    std::unique_ptr<Expression<T>> parsePrimary();//крч остальное
};
#endif