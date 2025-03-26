#include <iostream>
#include <map>
#include <complex>
#include "lexer.h"
#include "parser.h"
#include "expression.h"

using Complex = std::complex<double>;

int main(int argc, char* argv[]) {
    std::string command = argv[1];
    
    if (command == "--eval" || command == "--diff") {
        // для double 
        std::string exprStr = argv[2];
        Lexer lexer(exprStr);
        Parser<double> parser(lexer.tokenize());
        auto expr = parser.parse();

        if (command == "--eval") {
            std::map<std::string, double> vars;
            for (int i = 3; i < argc; ++i) {
                std::string arg = argv[i];
                size_t eqPos = arg.find('=');
                vars[arg.substr(0, eqPos)] = std::stod(arg.substr(eqPos + 1));
            }
            std::cout << expr->compute(vars) << std::endl;
        } else {
            std::string var = argv[4];
            auto derivative = expr->derivative(var);
            std::cout << derivative->toStr() << std::endl;
        }
    } else if (command == "--complex") {
        // для комплов
        std::string exprStr = argv[2];
        Lexer lexer(exprStr);
        Parser<Complex> parser(lexer.tokenize());
        auto expr = parser.parse();

        std::map<std::string, Complex> vars;
        for (int i = 3; i < argc; ++i) {
            std::string arg = argv[i];
            size_t eqPos = arg.find('=');
            std::string varName = arg.substr(0, eqPos);
            std::string valueStr = arg.substr(eqPos + 1);
            
            // Парсим комплы
            size_t plusPos = valueStr.find('+');
            double re = std::stod(valueStr.substr(0, plusPos));
            double im = std::stod(valueStr.substr(plusPos + 1, valueStr.size() - plusPos - 2));
            vars[varName] = Complex(re, im);
        }

        Complex result = expr->compute(vars);
        std::cout << result.real() << "+" << result.imag() << "i" << std::endl;
    } else {
        return 1;
    }

    return 0;
}