#define CATCH_CONFIG_MAIN
#include "catch2/catch_amalgamated.hpp"
#include "longnum.hpp"

TEST_CASE("Объявление чисел", "[LongNumber]") {
    LongNumber a("23.56", 30);
    REQUIRE(a == LongNumber("23.56", 30));

    LongNumber b = -45.78_longnum;
    REQUIRE(b == LongNumber("-45.78", 100));
}

TEST_CASE("Сложение чисел", "[LongNumber]") {
    LongNumber a("6.62", 100);
    LongNumber b = 5.3_longnum;
    LongNumber result1 = a + b;

    REQUIRE(result1 == LongNumber("11.92", 100));

    LongNumber c("12.2", 30);
    LongNumber d("-7.15", 30);
    LongNumber result2 = c + d;

    REQUIRE(result2 == LongNumber("5.05", 30));
}

TEST_CASE("Вычитание чисел", "[LongNumber]") {
    LongNumber a("135.78", 30);
    LongNumber b("47.4", 30);
    LongNumber result1 = a - b;

    REQUIRE(result1 == LongNumber("88.38", 30));

    LongNumber c("-12.4", 30);
    LongNumber d("-67.07", 30);
    LongNumber result2 = c - d;

    REQUIRE(result2 == LongNumber("54.67", 30));
}


TEST_CASE("Сравнение чисел", "[LongNumber]") {
    LongNumber a("99.99", 30);
    LongNumber b("99.99", 30);
    LongNumber c = -99.99_longnum;
    REQUIRE((a == b) == 1);
    REQUIRE((a != c) == 1);
    REQUIRE((b > c) == 1);
    REQUIRE((b > a) == 0);
}
