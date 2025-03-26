#include "lexer.h"
#include "token.h"
#include <cctype>
#include <sstream>
#include <stdexcept>

Lexer::Lexer(const std::string& input) : input(input), position(0) {}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (position < input.length()) {
        skipWhitespace();
        if (position >= input.length()) break;

        char current = currentChar();
        if (isdigit(current) || current == '.') {
            tokens.push_back(parseNumber());
        } else if (isalpha(current)) {
            tokens.push_back(parseIdentifier());
        } else if (current == '+' || current == '-' || current == '*' || current == '/' || current == '^') {
            tokens.push_back(parseOperator());
        } else if (current == '(' || current == ')') {
            tokens.push_back({
                current == '(' ? TokenType::LeftParen : TokenType::RightParen,
                std::string(1, consumeChar())
            });
        } else if (current == 'i' && (position == input.length() - 1 || !isalpha(input[position + 1]))) {
            tokens.push_back({TokenType::Number, Complex(0, 1)});
            consumeChar();
        }
    }
    tokens.push_back({TokenType::EndOfInput, ""}); // конец ввода
    return tokens;
}

Token Lexer::parseNumber() { // Разбор чисел
    std::string numStr;
    bool hasDecimal = false;

    while (position < input.length()) {
        char c = currentChar();
        if (isdigit(c)) {
            numStr += consumeChar();
        } else if (c == '.') {
            if (hasDecimal) break;
            hasDecimal = true;
            numStr += consumeChar();
        } else {
            break;
        }
    }

    // комплы
    if (position < input.length() && currentChar() == 'i') {
        consumeChar(); // Пропускаем 'i'
        if (numStr.empty()) {
            return {TokenType::Number, Complex(0, 1)}; //только i
        }
        return {TokenType::Number, Complex(0, std::stod(numStr))};//число + i
    }
    return {TokenType::Number, std::stod(numStr)};//вещественное число
}


char Lexer::currentChar() { return input[position]; }
char Lexer::consumeChar() { return input[position++]; }
void Lexer::skipWhitespace() { while (position < input.length() && isspace(currentChar())) consumeChar(); }

Token Lexer::parseIdentifier() {
    std::string text;
    while (position < input.length() && isalpha(currentChar())) {
        text += consumeChar();
    }
    if (text == "sin" || text == "cos" || text == "ln" || text == "exp") {
        return {TokenType::Function, text};
    } else {
        return {TokenType::Variable, text};
    }
}

Token Lexer::parseOperator() {
    return {TokenType::Operator, std::string(1, consumeChar())};
}