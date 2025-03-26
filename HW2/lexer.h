#ifndef LEXER_H
#define LEXER_H
#include <string>
#include <vector>
#include <complex>
#include "token.h"

class Lexer {
public:
    Lexer(const std::string& input); //конструктор
    std::vector<Token> tokenize();

private:
    std::string input;
    size_t position;

    char currentChar(); //нынешний символ
    char consumeChar(); //символ + некст позиция
    void skipWhitespace();
    Token parseNumber();       //разбивает числа
    Token parseIdentifier();   // Переменные и функции
    Token parseOperator();     // + - * / ^
};

#endif