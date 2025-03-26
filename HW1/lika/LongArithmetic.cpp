#include "LongArithmetic.hpp"
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

std::vector<char> IntegerToBin(const std::string num) {
    std::vector<char> result;
    std::vector<int> integerPart;
    if (num.find('.') != std::string::npos) {
        if (num[0] == '-') {
            for (int i = 1; i < num.substr(0, num.find('.')).length(); i++) {
                integerPart.push_back(num.substr(0, num.find('.'))[i] - '0');
            }
        } else {
            for (int i = 0; i < num.substr(0, num.find('.')).length(); i++) {
                integerPart.push_back(num.substr(0, num.find('.'))[i] - '0');
            }
        }
    } else {
        if (num[0] == '-') {
            for (int i = 1; i < num.length(); i++) {
                integerPart.push_back(num[i] - '0');
            }
        } else {
            for (int i = 0; i < num.length(); i++) {
                integerPart.push_back(num[i] - '0');
            }
        }
    }
    int currentEl;
    int reminder;
    while (!integerPart.empty()) {
        reminder = 0;
        for (int i = 0; i < integerPart.size(); i++) {
            currentEl = reminder * 10 + integerPart[i];
            reminder = currentEl % 2;
            integerPart[i] = currentEl / 2;
        }
        result.push_back((char)reminder);
        while (integerPart[0] == 0) {
            integerPart.erase(integerPart.begin());
        }
    }
    result.push_back('1');
    std::reverse(result.begin(), result.end());
    return result;
}

std::vector<char> FractionalToBin(const std::string num, int accuracy) {
    std::string stringFractional;
    std::vector<char> result;
    std::vector<int> fractionalPart;
    if (num.find('.') != std::string::npos) {
        stringFractional = num.substr(num.find('.') + 1);
    } else {
        for (int i = 0; i < accuracy; i++) {
            result.push_back('0');
        }
        return result;
    }
    for (int i = 0; i < stringFractional.length(); i++) {
        fractionalPart.push_back(stringFractional[i] - '0');
    }
    std::reverse(fractionalPart.begin(), fractionalPart.end());
    int reminder;
    int currentEl;
    while (!fractionalPart.empty() || (accuracy <= result.size())) {
        reminder = 0;
        for (int j = 0; j < fractionalPart.size(); j++) {
            currentEl = (fractionalPart[j] * 2 + reminder) % 10;
            reminder = (fractionalPart[j] * 2 + reminder) / 10;
            fractionalPart[j] = currentEl;
        }
        while (fractionalPart[0] == 0) {
            fractionalPart.erase(fractionalPart.begin());
        }
        result.push_back((char)reminder);
    }
    while (accuracy > result.size()) {
        result.push_back('0');
    }
    return result;
}

LongNum::LongNum() : binIntegerPart(std::vector<char>()), binFractionalPart(std::vector<char>()), accuracy(0), sign(false) {};

LongNum::LongNum(const std::string &num)
:   binIntegerPart(IntegerToBin(num)),
    binFractionalPart(std::vector<char>()),
    accuracy(0),
    sign(false) { //положительное
        if (num.find('.') != std::string::npos) {
            accuracy = num.length() - num.find('.');
            binFractionalPart = FractionalToBin(num, accuracy);
        }
        if(num[0] == '-') {
            sign = true; //отрицательное
        }
    };

LongNum::LongNum(const std::string &num, int accuracy)
:    binIntegerPart(IntegerToBin(num)),
     binFractionalPart(FractionalToBin(num, accuracy)),
     accuracy(accuracy),
     sign(false) { //положительное
        if(num[0] == '-') {
          sign = true; //отрицательное
        }
     };

LongNum::~LongNum() {}

LongNum::LongNum(const LongNum &other)
:    binIntegerPart(other.binIntegerPart),
     binFractionalPart(other.binFractionalPart),
     accuracy(other.accuracy),
     sign(other.sign) {}

LongNum &LongNum::operator=(const LongNum &other) {
    binIntegerPart = other.binIntegerPart;
    binFractionalPart = other.binFractionalPart;
    accuracy = other.accuracy;
    sign = other.sign;
    return *this;
}

LongNum LongNum::operator+(const LongNum &other) const{
    LongNum result;
    if (sign && other.sign) {
        result.sign = true;
    } else if (sign) {
        result.sign = false;
        result.accuracy = accuracy;
        result.binIntegerPart = binIntegerPart;
        result.binFractionalPart = binFractionalPart;
        return other - result;
    } else if (other.sign) {
        result.sign = false;
        result.accuracy = other.accuracy;
        result.binIntegerPart = other.binIntegerPart;
        result.binFractionalPart = other.binFractionalPart;
        return *this - result;
    } else {
        result.sign = false;
    }
    if (accuracy < other.accuracy) {
        result.accuracy = other.accuracy;
    } else {
        result.accuracy = accuracy;
    }
    std::vector<int> fractionalA;
    std::vector<int> fractionalB;
    for (int i = 0; i < accuracy; i++) {
        fractionalA.push_back(binFractionalPart[i] - '0');
    }
    for (int i = 0; i < other.accuracy; i++) {
        fractionalB.push_back(other.binIntegerPart[i] - '0');
    }
    while (fractionalA.size() < result.accuracy) {
        fractionalA.push_back(0);
    }
    while (fractionalB.size() < result.accuracy) {
        fractionalB.push_back(0);
    }
    std::reverse(fractionalA.begin(), fractionalA.end());
    std::reverse(fractionalB.begin(), fractionalB.end());
    int reminder = 0;
    for (int i = 0; i < result.accuracy; i++) {
        reminder = (fractionalA[i] + fractionalB[i] + reminder) / 2;
        if (((fractionalA[i] + fractionalB[i] + reminder) % 2) == 0) {
            result.binIntegerPart.push_back('0');
        } else {
            result.binIntegerPart.push_back('1');
        }
    }
    std::reverse(result.binFractionalPart.begin(), result.binFractionalPart.end());
    std::vector<int> integerA;
    std::vector<int> integerB;
    for (int i = 0; i < binIntegerPart.size(); i++) {
        integerA.push_back(binIntegerPart[i] - '0');
    }
    for (int i = 0; i < other.binIntegerPart.size(); i++) {
        integerB.push_back(other.binIntegerPart[i] - '0');
    }
    std::reverse(integerA.begin(), integerA.end());
    std::reverse(integerB.begin(), integerB.end());
    while (integerA.size() < std::max(integerB.size(), integerA.size())) {
        integerA.push_back(0);
    }
    while (integerB.size() < std::max(integerA.size(), integerB.size())) {
        integerB.push_back(0);
    }
    for (int i = 0; i < std::max(integerA.size(), integerB.size()); i++) {
        reminder = (integerA[i] + integerB[i] + reminder) / 2;
        if (((integerA[i] + integerB[i] + reminder) % 2) == 0) {
            result.binIntegerPart.push_back('0');
        } else {
            result.binIntegerPart.push_back('1');
        }
    }
    if (reminder != 0) {
        result.binIntegerPart.push_back('1');
    }
    std::reverse(result.binIntegerPart.begin(), result.binIntegerPart.end());
    return result;
}

LongNum LongNum::operator-(const LongNum &other) const{
    if (sign && other.sign) {
        LongNum temp = *this;
        LongNum next = other;
        temp.sign = false;
        next.sign = false;
        return next - temp;
    } else if (sign) {
        LongNum next = other;
        next.sign = true;
        return *this + next;
    } else if (other.sign) {
        return *this + other;
    }
    LongNum result;
    result.accuracy = std::max(accuracy, other.accuracy);
    if (*this > other) {
        result.sign = false;
        std::vector<int> integerA;
        std::vector<int> integerB;
        for (int i = 0; i < binIntegerPart.size(); i++) {
            integerA.push_back(binIntegerPart[i]);
        }
        for (int i = 0; i < other.binIntegerPart.size(); i++) {
            integerB.push_back(other.binIntegerPart[i]);
        }
        std::vector<int> fractionalA;
        std::vector<int> fractionalB;
        for (int i = 0; i < binFractionalPart.size(); i++) {
            fractionalA.push_back(binFractionalPart[i]);
        }
        for (int i = 0; i < other.binFractionalPart.size(); i++) {
            fractionalB.push_back(other.binFractionalPart[i]);
        }
        std::reverse(integerA.begin(), integerA.end());
        std::reverse(integerB.begin(), integerB.end());
        while (integerA.size() < std::max(integerA.size(), integerB.size())) {
            integerA.push_back(0);
        }
        while (integerB.size() < std::max(integerA.size(), integerB.size())) {
            integerB.push_back(0);
        }
        while (fractionalA.size() < result.accuracy) {
            fractionalA.push_back(0);
        }
        while (fractionalB.size() < result.accuracy) {
            fractionalB.push_back(0);
        }
        std::reverse(fractionalA.begin(), fractionalA.end());
        std::reverse(fractionalB.begin(), fractionalB.end());
        int reminder = 0;
        for (int i = 0; i < result.accuracy; i++) {
            if (reminder == 0) {
                if (fractionalA[i] >= fractionalB[i]) {
                    result.binFractionalPart.push_back((char) (fractionalA[i] - fractionalB[i]));
                    reminder = 0;
                } else {
                    result.binFractionalPart.push_back((char) (2 + fractionalA[i] - fractionalB[i]));
                    reminder = 1;
                }
            } else {
                if (fractionalA[i] - 1 >= fractionalB[i]) {
                    result.binFractionalPart.push_back((char) (fractionalA[i] - 1 - fractionalB[i]));
                    reminder = 0;
                } else {
                    result.binFractionalPart.push_back((char) (1 + fractionalA[i] - fractionalB[i]));
                    reminder = 1;
                }
            }
        }
        for (int i = 0; i < integerA.size(); i++) {
            if (reminder == 0) {
                if (integerA[i] >= integerB[i]) {
                    result.binIntegerPart.push_back((char) (integerA[i] - integerB[i]));
                    reminder = 0;
                } else {
                    result.binIntegerPart.push_back((char) (2 + integerA[i] - integerB[i]));
                    reminder = 1;
                }
            } else {
                if (integerA[i] - 1 >= integerB[i]) {
                    result.binIntegerPart.push_back((char) (integerA[i] - 1 - integerB[i]));
                    reminder = 0;
                } else {
                    result.binIntegerPart.push_back((char) (1 + integerA[i] - integerB[i]));
                    reminder = 1;
                }
            }
        }
        std::reverse(result.binIntegerPart.begin(), result.binIntegerPart.end());
        std::reverse(result.binFractionalPart.begin(), result.binFractionalPart.end());
        return result;
    } else if (*this < other) {
        result.sign = true;
        std::vector<int> integerA;
        std::vector<int> integerB;
        for (int i = 0; i < other.binIntegerPart.size(); i++) {
            integerA.push_back(other.binIntegerPart[i]);
        }
        for (int i = 0; i < binIntegerPart.size(); i++) {
            integerB.push_back(other.binIntegerPart[i]);
        }
        std::vector<int> fractionalA;
        std::vector<int> fractionalB;
        for (int i = 0; i < other.binFractionalPart.size(); i++) {
            integerA.push_back(other.binFractionalPart[i]);
        }
        for (int i = 0; i < binFractionalPart.size(); i++) {
            fractionalB.push_back(binFractionalPart[i]);
        }
        std::reverse(integerA.begin(), integerA.end());
        std::reverse(integerB.begin(), integerB.end());
        while (integerA.size() < std::max(integerA.size(), integerB.size())) {
            integerA.push_back(0);
        }
        while (integerB.size() < std::max(integerA.size(), integerB.size())) {
            integerB.push_back(0);
        }
        while (fractionalA.size() < result.accuracy) {
            fractionalA.push_back(0);
        }
        while (fractionalB.size() < result.accuracy) {
            fractionalB.push_back(0);
        }
        std::reverse(fractionalA.begin(), fractionalA.end());
        std::reverse(fractionalB.begin(), fractionalB.end());
        int reminder = 0;
        for (int i = 0; i < result.accuracy; i++) {
            if (reminder == 0) {
                if (fractionalA[i] >= fractionalB[i]) {
                    result.binFractionalPart.push_back((char) (fractionalA[i] - fractionalB[i]));
                    reminder = 0;
                } else {
                    result.binFractionalPart.push_back((char) (2 + fractionalA[i] - fractionalB[i]));
                    reminder = 1;
                }
            } else {
                if (fractionalA[i] - 1 >= fractionalB[i]) {
                    result.binFractionalPart.push_back((char) (fractionalA[i] - 1 - fractionalB[i]));
                    reminder = 0;
                } else {
                    result.binFractionalPart.push_back((char) (1 + fractionalA[i] - fractionalB[i]));
                    reminder = 1;
                }
            }
        }
        for (int i = 0; i < integerA.size(); i++) {
            if (reminder == 0) {
                if (integerA[i] >= integerB[i]) {
                    result.binIntegerPart.push_back((char) (integerA[i] - integerB[i]));
                    reminder = 0;
                } else {
                    result.binIntegerPart.push_back((char) (2 + integerA[i] - integerB[i]));
                    reminder = 1;
                }
            } else {
                if (integerA[i] - 1 >= integerB[i]) {
                    result.binIntegerPart.push_back((char) (integerA[i] - 1 - integerB[i]));
                    reminder = 0;
                } else {
                    result.binIntegerPart.push_back((char) (1 + integerA[i] - integerB[i]));
                    reminder = 1;
                }
            }
        }
        std::reverse(result.binIntegerPart.begin(), result.binIntegerPart.end());
        std::reverse(result.binFractionalPart.begin(), result.binFractionalPart.end());
        return result;
    }
    result.sign = false;
    result.binIntegerPart.push_back('0');
    for (int i = 0; i < result.accuracy; i++) {
        result.binFractionalPart.push_back('0');
    }
    return result;
}

LongNum LongNum::operator*(const LongNum &other) const {
    LongNum result;
    result.sign = sign ^ other.sign;
    result.accuracy = std::max(accuracy, other.accuracy);
    int numsAfterDot = accuracy + other.accuracy;
    std::vector<int> A, B;
    for (int i = 0; i < binIntegerPart.size(); i++) {
        A.push_back(binIntegerPart[i]);
    }
    for (int i = 0; i < binFractionalPart.size(); i++) {
        A.push_back(binFractionalPart[i]);
    }
    for (int i = 0; i < other.binIntegerPart.size(); i++) {
        B.push_back(other.binIntegerPart[i]);
    }
    for (int i = 0; i < other.binFractionalPart.size(); i++) {
        B.push_back(other.binFractionalPart[i]);
    }
    std::reverse(A.begin(), A.end());
    std::reverse(B.begin(), B.end());
    std::vector<int> C, D;
    int reminder = 0;
    for (int i = 0; i < A.size(); i++) {
        C.push_back((A[i] * B[0] + reminder)%2);
        reminder = (A[i] * B[0] + reminder)/2;
    }
    for (int i = 1; i < B.size(); i++) {
        reminder = 0;
        for (int j = 0; j < i; j++) {
            D.push_back(0);
        }
        for (int j = 0; j < A.size(); j++) {
            D.push_back((A[j] * B[i] + reminder)%2);
            reminder = (A[j] * B[i] + reminder)/2;
        }
        if (C.size() < D.size()) {
            C.push_back(0);
        }
        if (D.size() < C.size()) {
            D.push_back(0);
        }
        reminder = 0;
        int helpEl;
        for (int j = 0; j < C.size(); j++) {
            helpEl = (C[j] + D[j] + reminder) % 2;
            reminder = (C[j] + D[j]) / 2;
            C[j] = helpEl;
        }
        if (reminder != 0) {
            C.push_back(reminder);
        }
        D.clear();
    }
    for (int i = 0; i < numsAfterDot; i++) {
        result.binFractionalPart.push_back((char) (C[i]));
    }
    std::reverse(result.binFractionalPart.begin(), result.binFractionalPart.end());
    while (result.binFractionalPart.size() > result.accuracy) {
        result.binFractionalPart.pop_back();
    }
    while (result.binFractionalPart.size() < result.accuracy) {
        result.binFractionalPart.push_back(0);
    }
    for (int i = numsAfterDot; i < C.size(); i++) {
        result.binIntegerPart.push_back((char) (C[i]));
    }
    std::reverse(result.binIntegerPart.begin(), result.binIntegerPart.end());
    return result;
}

bool LongNum::operator==(const LongNum &other) const{
    if ((binIntegerPart.size() == other.binIntegerPart.size()) && (binFractionalPart.size() == other.binFractionalPart.size())) {
        for (int i = 0; i < binIntegerPart.size(); i++) {
            if (binIntegerPart[i] != other.binIntegerPart[i]) {
                return false;
            }
        }
        for (int i = 0; i < binFractionalPart.size(); i++) {
            if (binFractionalPart[i] != other.binFractionalPart[i]) {
                return false;
            }
        }
        return true;
    } else {
        return false;
    }
}

bool LongNum::operator!=(const LongNum &other) const {
    return !(*this == other);
}

bool LongNum::operator<(const LongNum &other) const {
    if (binIntegerPart.size() < other.binIntegerPart.size()) {
        return true;
    } if (binIntegerPart.size() > other.binIntegerPart.size()) {
        return false;
    }
    for (int i = 0; i < binIntegerPart.size(); i++) {
        if (binIntegerPart[i] < other.binIntegerPart[i]) {
            return true;
        }
        if (binIntegerPart[i] > other.binIntegerPart[i]) {
            return false;
        }
    }
    for (int i = 0; i < binFractionalPart.size(); i++) {
        if (binFractionalPart[i] < other.binFractionalPart[i]) {
            return true;
        }
        if (binFractionalPart[i] > other.binFractionalPart[i]) {
            return false;
        }
    }
    return false;
}

bool LongNum::operator>(const LongNum &other) const {
    return (!(*this < other) && !(*this == other));
}

int main(){
    LongNum a("123.2", 3);
}