#define CATCH_CONFIG_MAIN
#include "include/catch2/catch_amalgamated.hpp"
#include "longnum.hpp"


typedef LongNumber BigNum;

TEST_CASE("Plus operation works correctly, when both positive and no fractional part", "[arithmetic]") {
    // given
    const LongNumber num1("10");
    const LongNumber num2("10");

    // when
    const LongNumber result = num1 + num2;

    // then
    const LongNumber expected("20");
    REQUIRE(result == expected);
}
/*
TEST_CASE("Plus operation works correctly, when both positive and no integer part", "[arithmetic]") {
    // given
    const LongNumber num1("0.1");
    const LongNumber num2("0.1");

    // when
    const LongNumber result = num1 + num2;

    // then
    const LongNumber expected("0.2");
    REQUIRE(result == expected);
}

TEST_CASE("Plus operation works correctly, when both positive", "[arithmetic]") {
    // given
    const LongNumber num1("10.5");
    const LongNumber num2("10.75");

    // when
    const LongNumber result = num1 + num2;

    // then
    const LongNumber expected("21.25");
    REQUIRE(result == expected);
}

TEST_CASE("Plus operation works correctly, when one negative", "[arithmetic]") {
    // given
    const LongNumber num1("-10.15");
    const LongNumber num2("10.75");

    // when
    const LongNumber result = num1 + num2;

    // then
    const LongNumber expected("0");
    REQUIRE(result == expected);
}

TEST_CASE("Plus operation works correctly, when one negative and number is really long", "[arithmetic]") {
    // given
    const LongNumber num1(
        "-10000000000000000000000000000000000000000000000000000000000000000000001"
        ".5"
    );
    const LongNumber num2(
        "10000000000000000000000000000000000000000000000000000000000000000000000"
        ".5"
    );

    // when
    const LongNumber result = num1 + num2;

    // then
    const LongNumber expected("-1");
    REQUIRE(result == expected);
}

TEST_CASE("Minus operation works correctly", "[arithmetic]") {
    // given
    const LongNumber num1(
        "15.84375"
    );
    const LongNumber num2(
        "11.8125"
    );

    // when
    const LongNumber result_positive = num1 - num2;

    // then
    const LongNumber expected_positive("4.03125");
    CHECK(result_positive == expected_positive);

    // when
    const LongNumber result_negative = num2 - num1;

    // then
    const LongNumber expected_negative("-4.03125");
    REQUIRE(result_negative == expected_negative);
}

TEST_CASE("Multiply operation works correctly, when both positive", "[arithmetic]") {
    // given
    const LongNumber num1(
        "15.84375"
    );
    const LongNumber num2(
        "11.8125"
    );

    // when
    const LongNumber result = num1 * num2;

    // then
    const LongNumber expected("187.154296875");
    REQUIRE(result == expected);
}

TEST_CASE("Multiply operation works correctly, when one negative", "[arithmetic]") {
    // given
    const LongNumber num1(
        "-15.84375"
    );
    const LongNumber num2(
        "11.8125"
    );

    // when
    const LongNumber result = num1 * num2;

    // then
    const LongNumber expected("-187.154296875");
    REQUIRE(result == expected);
}

TEST_CASE("Multiply operation works correctly, when both negative", "[arithmetic]") {
    // given
    const LongNumber num1(
        "-15.84375"
    );
    const LongNumber num2(
        "-11.8125"
    );

    // when
    const LongNumber result = num1 * num2;

    // then
    const LongNumber expected("187.154296875");
    REQUIRE(result == expected);
}
/*
TEST_CASE("Divide operation works correctly, when both positive", "[arithmetic]") {
    // given
    const LongNumber num1(
        "2.25"
    );
    const LongNumber num2(
        "2.25"
    );

    // when
    const LongNumber result = num1 / num2;

    // then
    const LongNumber expected("1");
    REQUIRE(result == expected);
}

TEST_CASE("Divide operation works correctly, when one negative", "[arithmetic]") {
    // given
    const LongNumber num1(
        "-2.25"
    );
    const LongNumber num2(
        "2.25"
    );

    // when
    const LongNumber result = num1 / num2;

    // then
    const LongNumber expected("-1");
    REQUIRE(result == expected);
}

TEST_CASE("Divide operation works correctly, when both negative", "[arithmetic]") {
    // given
    const LongNumber num1(
        "-2.25"
    );
    const LongNumber num2(
        "-2.25"
    );

    // when
    const LongNumber result = num1 / num2;

    // then
    const LongNumber expected("1");
    REQUIRE(result == expected);
}

TEST_CASE("Greater operation works correctly, when both positive", "[arithmetic]") {
    // given
    const LongNumber num1(
        "10.01"
    );
    const LongNumber num2(
        "11.02"
    );

    // when
    const bool result_false = num1 > num2;

    // then
    CHECK_FALSE(result_false);

    // when
    const bool result_true = num1 < num2;

    // then
    REQUIRE(result_true);
}

TEST_CASE("Greater operation works correctly, when one negative", "[arithmetic]") {
    // given
    const LongNumber num1(
        "-10.01"
    );
    const LongNumber num2(
        "11.02"
    );

    // when
    const bool result_false = num1 > num2;

    // then
    CHECK_FALSE(result_false);

    // when
    const bool result_true = num1 < num2;

    // then
    REQUIRE(result_true);
}

TEST_CASE("Greater operation works correctly, when both negative", "[arithmetic]") {
    // given
    const LongNumber num1(
        "-11.01"
    );
    const LongNumber num2(
        "-10.02"
    );

    // when
    const bool result_false = num1 > num2;

    // then
    CHECK_FALSE(result_false);

    // when
    const bool result_true = num1 < num2;

    // then
    REQUIRE(result_true);
}

LongNumber calculate_pi(int precision) {
    LongNumber pi("3.0");
    LongNumber epsilon = LongNumber("0.1");
    for (int i = 0; i < precision; ++i) {
        epsilon = epsilon * LongNumber("0.1"); // epsilon = 10^{-precision}
    }

    LongNumber sign("1.0");
    LongNumber denominator("2.0"); // Начинаем с 2*3*4
    int step = 0;
    LongNumber term;

    do {
        term = LongNumber("4.0") / (denominator * (denominator + LongNumber("1.0")) * (denominator + LongNumber("2.0")));
        term = term * sign;

        pi = pi + term;

        sign = sign * LongNumber("-1.0");
        denominator = denominator + LongNumber("2.0");
        step++;
    } while (term > epsilon || term < epsilon * LongNumber("-1.0"));

    return pi;
}
    

TEST_CASE("Calculate pi", "[benchmark]") {
    BENCHMARK("Fibonacci 25") {
        return calculate_pi(100);
    };
}
    
*/