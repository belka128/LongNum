#ifndef LONG_ARITHMETIC_HPP
#define LONG_ARITHMETIC_HPP

#include <iostream>
#include <vector>
#include <string>

class LongNum {
private:
      std::vector<char> binIntegerPart;
      std::vector<char> binFractionalPart;
      int accuracy;
      bool sign;
public:
      LongNum();
      LongNum(const std::string &num);
      LongNum(const std::string &num, int accuracy);
      ~LongNum();
      LongNum(const LongNum &other); //конструктор копирования
      LongNum &operator=(const LongNum &other); //оператор присваивания
      LongNum operator+(const LongNum &other) const;
      LongNum operator-(const LongNum &other) const;
      LongNum operator*(const LongNum &other) const;
      LongNum operator/(const LongNum &other) const;
      bool operator==(const LongNum &other) const;
      bool operator!=(const LongNum &other) const;
      bool operator<(const LongNum &other) const;
      bool operator>(const LongNum &other) const;
};
#endif