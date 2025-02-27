#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>
#include <sstream> //перевод в строку
#include <iomanip> //точный вывод
#include <stdbool.h>

bool IsTheyEqual(std::vector<char> a, std::vector<char> b);
std::vector<char> BinaryInterpritation(std::string num, int accuracy, int &countIntegers);

class LongNumber {
    private:
        std::vector<char> BinaryRepresentation; //двочиный вектор
        int CountIntegers; //скок двоичных цифр целой части
        int sign; //Знак: 1 - отрицательное, 0 - положительные
        friend void print_LN(const LongNumber& num);
    
    public:
        LongNumber();//Если ничего не введут
        LongNumber(const std::string &num, int accuracy); //Если передается строка
        LongNumber(const std::vector<char> &num, int kolvoIntegers, int sign);

        LongNumber operator + (const LongNumber& other);
        LongNumber operator - (const LongNumber& other);
        LongNumber operator = (const LongNumber& other);
        bool operator == (const LongNumber& other) const;
};

void print_LN(const LongNumber& num);
LongNumber operator"" _longnum (long double num);