#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>
#include <sstream> //перевод в строку
#include <iomanip> //точный вывод
#include <stdbool.h>

bool IsTheyEqual(std::vector<char> a, std::vector<char> b){ //Проверка чисел на равенство(включая незначащие нули в конце)
    int i = 0;
    while (i < a.size() && i < b.size()){
        if (a[i] != b[i]){
            return false;
        }
        i++;
    }
    if (i < a.size()){
        while (i < a.size()){
            if (a[i] != 0){
                return false;
            }
            i++;
        }
    }else{
        while (i < b.size()){
            if (b[i] != 0){
                return false;
            }
            i++;
        }
    }
    return true;
}

std::vector<char> BinaryInterpritation(std::string num, int accuracy, int &countIntegers){
    countIntegers = 0;
    if (num[0] == '-'){
        num.erase(num.begin());
    }
    int len_num = num.size()-1;
    int integer = num.find('.');
    int precision = len_num - integer;
    int count = 0;
    std::vector<char> NewNum; //вектор хранящий num посимвольно в 10ой СС
    for (char c : num){ //записываем строку в наш вектор
        if (count == integer){
            count++;
            continue; // Пропускаем точку
        }
        NewNum.push_back(c - '0');
        count++;
    }

    std::vector<char> NumResult; //вектор хранящий num посимвольно в 2ой СС

    //--------------------------------- ЗАПИСЫВАЕМ ЦЕЛУЮ ЧАСТЬ В 2-ОЙ СС ---------------------------------
    int count_zero = 0;
    int previous_ost = 0;
    while (count_zero < integer){
        if ((count_zero + (NewNum[integer-1]==0) != integer) && (NewNum[integer-1] % 2 == 1)){ //В этом блоке добавляем 0 или 1 в разложение
            NumResult.push_back(1);
        }else{
            NumResult.push_back(0);
        }
        countIntegers++;
        previous_ost = 0;
        count_zero = 0;
        for (int i = 0; i < integer; i++){ //В этом блоке мы изменяем NewNum
            if ((NewNum[i] + previous_ost*10) == 0){
                count_zero++;
                previous_ost = 0;
                continue;
            }
            if (NewNum[i] % 2 == 0){ //делим на 2 четные числа
                NewNum[i] = (NewNum[i] + previous_ost*10)/2;
                previous_ost = 0;
            }else{ //делим на 2 нечетные числа
                NewNum[i] = (NewNum[i] + previous_ost*10)/2;
                previous_ost = 1;
            }
        }
    }
    while (countIntegers > 1 && NumResult[countIntegers-1] == 0){ //Убираем ведущие нули
        NumResult.pop_back();
        countIntegers--;
    }
    reverse(NumResult.begin(), NumResult.end()); // переворачиваем NumResult

    //------------------------- ЗАПИСЫВАЕМ ДРОБНУЮ ЧАСТЬ В 2-ОЙ СС -------------------------
    int kolvoPrecision = 0;
    previous_ost = 0;
    count_zero = 0;
    while (kolvoPrecision < accuracy){
        previous_ost = 0;
        for (int i = len_num-1; i >= integer; i--){
            if ((NewNum[i] * 2 + previous_ost) >= 10){
                NewNum[i] = (NewNum[i] * 2 + previous_ost)%10;
                previous_ost = 1;
            }else{
                NewNum[i] = (NewNum[i] * 2 + previous_ost)%10;
                previous_ost = 0;
            }
        }
        if (previous_ost == 1){
            kolvoPrecision++;
            NumResult.push_back(1);
        }else{
            kolvoPrecision++;
            NumResult.push_back(0);
        }
    }
    return NumResult;
}

class LongNumber {
private:
    std::vector<char> BinaryRepresentation; //двочиный вектор
    int CountIntegers; //скок двоичных цифр целой части
    int sign; //Знак: 1 - отрицательное, 0 - положительные
    friend void print_LN(const LongNumber& num);

public:
    LongNumber():BinaryRepresentation(std::vector<char>()), CountIntegers(0), sign(0) {};//Если ничего не введут
    LongNumber(const std::string &num, int accuracy) //Если передается строка
    :CountIntegers(0), //должно меняться в функции, а так хз
    BinaryRepresentation(std::vector<char>()),
    sign(0){    
        int c = 0;
        std::vector<char> bin = BinaryInterpritation(num, accuracy, c);  
        BinaryRepresentation = std::move(bin);
        CountIntegers = c;
        if (num[0] == '-'){
            sign = 1;
        }
    };
    LongNumber(const std::vector<char> &num, int kolvoIntegers, int sign) //Если передается вектор в двоичной (для + и -)
    :CountIntegers(kolvoIntegers),
    BinaryRepresentation(num),
    sign(sign){};

    LongNumber operator + (const LongNumber& other){
        int Resultsign = this->sign; // записываем знак результата
        int lenNum1 = this->BinaryRepresentation.size(); //общая длина в 2ой СС
        int lenNum2 = other.BinaryRepresentation.size();
        int lenPrecision1 = lenNum1 - this->CountIntegers; //кол-во цифр в дробной части
        int lenPrecision2 = lenNum2 - other.CountIntegers;
        std::vector<char> ResultInteger; //сумма целой части
        std::vector<char> ResultPrecision; //сумма дробной части
        int previous_ost = 0; //(используется в подсчете и целой и дробной части)
        if (this->sign == other.sign){ //одинаковые знаки
            std::vector<char> ResultInteger; //сумма целой части
            std::vector<char> ResultPrecision; //сумма дробной части
            //СЧИТАЕМ ДРОБНУЮ ЧАСТЬ
            int counter1 = 0; //инициализируем counter(ы)
            int counter2 = 0;
            
            int differencePrecisions = lenPrecision1 - lenPrecision2;
            if (differencePrecisions > 0){ //записываем хвост первого числа + записываем counter
                counter1 = lenNum1-differencePrecisions-1;
                counter2 = lenNum2-1;
                for (int i = lenNum1 - 1; differencePrecisions > 0; i--){
                    ResultPrecision.push_back(this->BinaryRepresentation[i]);
                    differencePrecisions--;
                }
            }else{
                counter1 = lenNum1-1;
                counter2 = lenNum2+differencePrecisions-1;
                for (int i = lenNum2 - 1; differencePrecisions < 0; i--){
                    ResultPrecision.push_back(other.BinaryRepresentation[i]);
                    differencePrecisions++; //записываем хвост второго числа + записываем counter
                }
            }
            while (counter1 >= this->CountIntegers){ //считаем оставшиеся слагаемые(сейчас у обоих чисел их одинаковое кол-во, поэтому нет условия на counter2)
                if (((this->BinaryRepresentation[counter1]) + (other.BinaryRepresentation[counter2]) + previous_ost) == 3){
                    previous_ost = 1;
                    ResultPrecision.push_back(1);
                }else if (((this->BinaryRepresentation[counter1]) + (other.BinaryRepresentation[counter2]) + previous_ost) == 2){
                    previous_ost = 1;
                    ResultPrecision.push_back(0);
                }else if (((this->BinaryRepresentation[counter1]) + (other.BinaryRepresentation[counter2]) + previous_ost) == 1){
                    previous_ost = 0;
                    ResultPrecision.push_back(1);
                }else if (((this->BinaryRepresentation[counter1]) + (other.BinaryRepresentation[counter2]) + previous_ost) == 0){
                    previous_ost = 0;
                    ResultPrecision.push_back(0);
                }
                counter1--;
                counter2--;
            }

            reverse(ResultPrecision.begin(), ResultPrecision.end()); // готовая дробная часть
            // СЧИТАЕМ ЦЕЛУЮ ЧАСТЬ
            counter1 = this->CountIntegers - 1;
            counter2 = other.CountIntegers - 1;
            int kolvoIntegers = 0;
            while (counter1 >= 0 && counter2 >= 0){
                if (((this->BinaryRepresentation[counter1]) + (other.BinaryRepresentation[counter2]) + previous_ost) == 3){
                    previous_ost = 1;
                    ResultInteger.push_back(1);
                }else if (((this->BinaryRepresentation[counter1]) + (other.BinaryRepresentation[counter2]) + previous_ost) == 2){
                    previous_ost = 1;
                    ResultInteger.push_back(0);
                }else if (((this->BinaryRepresentation[counter1]) + (other.BinaryRepresentation[counter2]) + previous_ost) == 1){
                    previous_ost = 0;
                    ResultInteger.push_back(1);
                }else if (((this->BinaryRepresentation[counter1]) + (other.BinaryRepresentation[counter2]) + previous_ost) == 0){
                    previous_ost = 0;
                    ResultInteger.push_back(0);
                }
                kolvoIntegers++;
                counter1--;
                counter2--;
            }
            if (counter1 >= 0){ //дописываем хвост первого числа
                for (int i = counter1; i >= 0; i--){
                    if (((this->BinaryRepresentation[i]) + previous_ost == 0) || ((this->BinaryRepresentation[i]) + previous_ost == 1)){
                        ResultInteger.push_back((this->BinaryRepresentation[i]) + previous_ost);
                        previous_ost = 0;
                    }else{
                        ResultInteger.push_back(0);
                        previous_ost = 1;
                    }
                    kolvoIntegers++;
                }
            }else if (counter2 >= 0){ //дописываем хвост второго числа
                for (int i = counter2; i >= 0; i--){
                    if (((other.BinaryRepresentation[i]) + previous_ost == 0) || ((other.BinaryRepresentation[i]) + previous_ost == 1)){
                        ResultInteger.push_back((other.BinaryRepresentation[i]) + previous_ost);
                        previous_ost = 0;
                    }else{
                        ResultInteger.push_back(0);
                        previous_ost = 1;
                    }
                    kolvoIntegers++;
                }
            }
            if (previous_ost == 1){
                ResultInteger.push_back(1);
                kolvoIntegers++;
            }
            reverse(ResultInteger.begin(), ResultInteger.end()); // готовая целая часть
            ResultInteger.insert(ResultInteger.end(), ResultPrecision.begin(), ResultPrecision.end()); //Готовый ответ
            
            return(LongNumber(ResultInteger, kolvoIntegers, this->sign)); //нужно вернуть новый класс
        }else{ //разные знаки у слагаемых (ВЫЧИТАНИЕ) -----------------------------------------------------------------------------
            std::vector<char> Result; //будующий ответ
            int WhoBigger = 1; //Определяем кто больше по модулю 0ое число или 1ое число
            if (this->CountIntegers > other.CountIntegers){
                WhoBigger = 0;
            }else if (this->CountIntegers == other.CountIntegers){
                
                int idx = 0;
                int flag = 0;
                while (idx < lenNum1 && idx < lenNum2){
                    if (this->BinaryRepresentation[idx] > other.BinaryRepresentation[idx]){
                        WhoBigger = 0;
                        flag = 1;
                        break;
                    }else if(this->BinaryRepresentation[idx] < other.BinaryRepresentation[idx]){
                        flag = 2;
                        break;
                    }
                    idx++;
                }
                if (flag == 0){
                    if (idx == lenNum2){
                        WhoBigger = 0;
                    }
                }else if(flag == 2){
                    WhoBigger = 1;
                }
            }
            
            if (WhoBigger == 0){ // 1ое > 2ое
                //СЧИТАЕМ ОБЕ ЧАСТИ
                int counter1 = 0; //инициализируем counter(ы)
                int counter2 = 0;
                int differencePrecisions = lenPrecision1 - lenPrecision2;
                if (differencePrecisions > 0){ //записываем хвост первого числа + записываем counter
                    counter1 = lenNum1-differencePrecisions-1;
                    counter2 = lenNum2-1;
                    for (int i = lenNum1 - 1; differencePrecisions > 0; i--){
                        Result.push_back(this->BinaryRepresentation[i]);
                        differencePrecisions--;
                    }
                }else{ //записываем хвост второго числа + записываем counter
                    counter1 = lenNum1-1;
                    counter2 = lenNum2+differencePrecisions-1;
                    previous_ost = 0;
                    for (int i = lenNum2 - 1; differencePrecisions < 0; i--){
                        if ((other.BinaryRepresentation[i]) == 1){
                            if (previous_ost == -1){
                                Result.push_back(0);
                            }else{
                                Result.push_back(1);
                                previous_ost = -1;
                            }
                        }else{
                            if (previous_ost == -1){
                                Result.push_back(1);
                            }else{
                                previous_ost = 0;
                                Result.push_back(0);
                            }
                        }
                        differencePrecisions++;
                    }
                }

                int kolvoIntegers = 0;
                while (counter2 >= 0){ //считаем оставшиеся цифры
                    if (((this->BinaryRepresentation[counter1]) - (other.BinaryRepresentation[counter2]) + previous_ost) == -2){
                        previous_ost = -1;
                        Result.push_back(0);
                    }else if (((this->BinaryRepresentation[counter1]) - (other.BinaryRepresentation[counter2]) + previous_ost) == -1){
                        previous_ost = -1;
                        Result.push_back(1);
                    }else if (((this->BinaryRepresentation[counter1]) - (other.BinaryRepresentation[counter2]) + previous_ost) == 0){
                        previous_ost = 0;
                        Result.push_back(0);
                    }else if (((this->BinaryRepresentation[counter1]) - (other.BinaryRepresentation[counter2]) + previous_ost) == 1){
                        previous_ost = 0;
                        Result.push_back(1);
                    }
                    if (counter1 < this->CountIntegers){
                        kolvoIntegers++;
                    }
                    counter1--;
                    counter2--;  
                }
                if (counter1 >= 0){ //дописываем хвост первого числа
                    for (int i = counter1; i >= 0; i--){
                        if (((this->BinaryRepresentation[i]) + previous_ost == 0) || ((this->BinaryRepresentation[i]) + previous_ost == 1)){
                            Result.push_back((this->BinaryRepresentation[i]) + previous_ost);
                            previous_ost = 0;
                        }else{
                            Result.push_back(0);
                            previous_ost = -1;
                        }
                        kolvoIntegers++;
                    }
                }
                
                while (kolvoIntegers > 1 && Result[Result.size()-1] == 0){ //Убираем ведущие нули
                    Result.pop_back();
                    kolvoIntegers--;
                }
                reverse(Result.begin(), Result.end()); // готовый ответ
                
                return(LongNumber(Result, kolvoIntegers, this->sign));
                
            }else{ // 1ое < 2ое
                LongNumber NotOther = other;  
                return NotOther + *this;
            }
        }
    }
    LongNumber operator - (const LongNumber& other){
        LongNumber NotOther = other;  
        NotOther.sign = !other.sign;  
        return *this + NotOther;  // Вызываем сложение
    }
    LongNumber operator = (const LongNumber& other) {
        if (this != &other) {
            BinaryRepresentation = other.BinaryRepresentation;
            sign = other.sign;
            CountIntegers = other.CountIntegers;
        }
        return *this;
    }
    bool operator == (const LongNumber& other) const {
        return (sign == other.sign &&
                CountIntegers == other.CountIntegers &&
                IsTheyEqual(BinaryRepresentation, other.BinaryRepresentation));
    }

};


void print_LN(const LongNumber& num) { //работает но не для мего больших чисел
    std::cout << "Двоичное представление: ";
    
    for (int i = 0; i < num.BinaryRepresentation.size(); i++) {
        if (i == num.CountIntegers){
             std::cout << "."; // Разделяем целую и дробную часть
        }
        std::cout << (int)num.BinaryRepresentation[i];
    }
    std::cout << std::endl;

    //Перевод целой части в 10ое
    long long integerPart = 0;
    for (int i = 0; i < num.CountIntegers; i++) {
        integerPart = integerPart * 2 + num.BinaryRepresentation[i];
    }

    //Перевод дробной части в десятичное
    double fractionalPart = 0.0;
    double power = 0.5;

    for (int i = num.CountIntegers; i < num.BinaryRepresentation.size(); i++) {
        fractionalPart += num.BinaryRepresentation[i] * power;
        power /= 2;
    }

    double result = integerPart + fractionalPart;
    if (num.sign == 1){ //Учитываем знак числа
        result = -result;
    }
    std::cout << "Число в 10-ичной системе: " << std::setprecision(10) << result << std::endl;
}

LongNumber operator"" _longnum (long double num) { //переводим в строку, точность = 100 || НЕ РАБОТАЕТ С ОТРИЦАТЕЛЬНЫМИ
    std::ostringstream oss;
    oss << num; // Записываем число в строку
    std::string str_num = oss.str(); // Получаем строку
    return(LongNumber(str_num, 100));
};

int main(){
    // LongNumber num2;
    LongNumber x = 1462.3456_longnum;
    LongNumber y("12.123", 50); //число, точность
    print_LN(x);
    print_LN(y);
    // print_LN(num);
    
    // print_LN(b);
    //LongNumber y = 1223.913_longnum;
    LongNumber c = y - x;
    print_LN(c);
    
    return 0;
}