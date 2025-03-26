#include <iostream>
#include <map>
#include <complex>
#include "lexer.h"
#include "parser.h"
#include "expression.h"

using Complex = std::complex<double>;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage:\n"
                  << "  Evaluation: " << argv[0] << " --eval <expression> [var=value ...]\n"
                  << "  Differentiation: " << argv[0] << " --diff <expression> --by <variable>\n"
                  << "  Complex mode: " << argv[0] << " --complex <expression> [var=re+imi ...]\n";
        return 1;
    }

    std::string command = argv[1];
    
    if (command == "--eval" || command == "--diff") {
        // Режим для double (старая логика)
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
            // Заменили evaluate() на compute()
            std::cout << expr->compute(vars) << std::endl;
        } else {
            std::string var = argv[4];
            // Заменили differentiate() на derivative()
            auto derivative = expr->derivative(var);
            // Заменили toString() на toStr()
            std::cout << derivative->toStr() << std::endl;
        }
    } else if (command == "--complex") {
        // Режим для комплексных чисел
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
            
            // Парсим комплексное число (формат "a+bi")
            size_t plusPos = valueStr.find('+');
            double re = std::stod(valueStr.substr(0, plusPos));
            double im = std::stod(valueStr.substr(plusPos + 1, valueStr.size() - plusPos - 2));
            vars[varName] = Complex(re, im);
        }

        Complex result = expr->compute(vars);  // Заменили evaluate()
        std::cout << result.real() << "+" << result.imag() << "i" << std::endl;
    } else {
        std::cerr << "Unknown command\n";
        return 1;
    }

    return 0;
}