#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>
#include <sstream> //перевод в строку
#include <iomanip> //точный вывод
#include <stdbool.h>

int IsTheyEqual(std::vector<char> a, std::vector<char> b);
std::vector<char> BinaryInterpritation(std::string num, int accuracy, int &countIntegers);
std::vector<char> AddBinaryVectors(const std::vector<char>& a, const std::vector<char>& b);

class LongNumber {
    private:
        std::vector<char> BinaryRepresentation; //двочиный вектор
        int CountIntegers; //скок двоичных цифр целой части
        int sign; //Знак: 1 - отрицательное, 0 - положительные
        friend void print_LN(const LongNumber& num);
        friend LongNumber shiftLeft(const LongNumber &x, int shiftCount);
    public:
        LongNumber();//Если ничего не введут
        LongNumber(const std::string &num); //Если передается онли строка
        LongNumber(const std::string &num, int accuracy); //Если передается строка + точность
        LongNumber(const std::vector<char> &num, int kolvoIntegers, int sign);
        LongNumber(const LongNumber& other) = default;

        LongNumber operator + (const LongNumber& other) const;
        LongNumber operator - (const LongNumber& other) const;
        LongNumber operator * (const LongNumber& other) const;
        LongNumber operator / (const LongNumber& other) const;
        LongNumber operator = (const LongNumber& other);
        bool operator == (const LongNumber& other) const;
        bool operator != (const LongNumber& other) const;
        bool operator<(const LongNumber& other) const;
        bool operator>(const LongNumber& other) const;
        LongNumber operator-() const;
        ~LongNumber() = default;
};

void print_LN(const LongNumber& num);
LongNumber operator"" _longnum (long double num);