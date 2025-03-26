#define CATCH_CONFIG_MAIN
#include "catch.h"
#include <complex>
#include "expression.h"
#include "lexer.h"
#include "parser.h"

using Complex = std::complex<double>;

// ==================== Числовые узлы ====================
TEST_CASE("5.2 == 5.2") {
    auto num = std::make_unique<ConstantExpr<double>>(5.2);
    REQUIRE(num->compute({}) == Approx(5.2));
}

TEST_CASE("-1.8 == -1.8") {
    auto num = std::make_unique<ConstantExpr<double>>(-1.8);
    REQUIRE(num->compute({}) == Approx(-1.8));
}

// ==================== Переменные ====================
TEST_CASE("y=3.0 → 3.0") {
    auto var = std::make_unique<VariableExpr<double>>("y");
    REQUIRE(var->compute({{"y", 3.0}}) == Approx(3.0));
}

TEST_CASE("z=-2.1 → -2.1") {
    auto var = std::make_unique<VariableExpr<double>>("z");
    REQUIRE(var->compute({{"z", -2.1}}) == Approx(-2.1));
}

// ==================== Арифметические операции ====================
TEST_CASE("1.5 + 2.5 == 4.0") {
    auto expr = std::make_unique<BinaryOpExpr<double>>('+',
        std::make_unique<ConstantExpr<double>>(1.5),
        std::make_unique<ConstantExpr<double>>(2.5));
    REQUIRE(expr->compute({}) == Approx(4.0));
}

TEST_CASE("-3 + 5 == 2") {
    auto expr = std::make_unique<BinaryOpExpr<double>>('+',
        std::make_unique<ConstantExpr<double>>(-3),
        std::make_unique<ConstantExpr<double>>(5));
    REQUIRE(expr->compute({}) == Approx(2));
}

TEST_CASE("10 / 2 == 5") {
    auto expr = std::make_unique<BinaryOpExpr<double>>('/',
        std::make_unique<ConstantExpr<double>>(10),
        std::make_unique<ConstantExpr<double>>(2));
    REQUIRE(expr->compute({}) == Approx(5));
}

TEST_CASE("2.5 * 4 == 10.0") {
    auto expr = std::make_unique<BinaryOpExpr<double>>('*',
        std::make_unique<ConstantExpr<double>>(2.5),
        std::make_unique<ConstantExpr<double>>(4));
    REQUIRE(expr->compute({}) == Approx(10.0));
}

TEST_CASE("(-3 * -2) - 1 == 5") {
    auto expr = std::make_unique<BinaryOpExpr<double>>('*',
        std::make_unique<ConstantExpr<double>>(-3),
        std::make_unique<ConstantExpr<double>>(-2));
    REQUIRE(expr->compute({}) == Approx(6));
}

// ==================== Математические функции ====================
TEST_CASE("sin(π/2) ≈ 1") {
    auto expr = std::make_unique<FunctionExpr<double>>("sin",
        std::make_unique<ConstantExpr<double>>(3.1415926/2));
    REQUIRE(expr->compute({}) == Approx(1).margin(0.0001));
}

TEST_CASE("sin(π) ≈ 0") {
    auto expr = std::make_unique<FunctionExpr<double>>("sin",
        std::make_unique<ConstantExpr<double>>(3.1415926));
    REQUIRE(expr->compute({}) == Approx(0).margin(0.0001));
}

TEST_CASE("exp(0) == 1") {
    auto expr = std::make_unique<FunctionExpr<double>>("exp",
        std::make_unique<ConstantExpr<double>>(0));
    REQUIRE(expr->compute({}) == Approx(1));
}

TEST_CASE("exp(-1) ≈ 0.3679") {
    auto expr = std::make_unique<FunctionExpr<double>>("exp",
        std::make_unique<ConstantExpr<double>>(-1));
    REQUIRE(expr->compute({}) == Approx(0.3679).margin(0.0001));
}

// ==================== Комплексные числа ====================
TEST_CASE("(2+3i) + (1+1i) == (3+4i)") {
    auto expr = std::make_unique<BinaryOpExpr<Complex>>('+',
        std::make_unique<ConstantExpr<Complex>>(Complex(2,3)),
        std::make_unique<ConstantExpr<Complex>>(Complex(1,1)));
    REQUIRE(expr->compute({}) == Complex(3,4));
}

TEST_CASE("(0+0i) + (1.5+2.5i) == (1.5+2.5i)") {
    auto expr = std::make_unique<BinaryOpExpr<Complex>>('+',
        std::make_unique<ConstantExpr<Complex>>(Complex(0,0)),
        std::make_unique<ConstantExpr<Complex>>(Complex(1.5,2.5)));
    REQUIRE(expr->compute({}) == Complex(1.5,2.5));
}

TEST_CASE("(1+2i) * (3+4i) == (-5+10i)") {
    auto expr = std::make_unique<BinaryOpExpr<Complex>>('*',
        std::make_unique<ConstantExpr<Complex>>(Complex(1,2)),
        std::make_unique<ConstantExpr<Complex>>(Complex(3,4)));
    REQUIRE(expr->compute({}) == Complex(-5,10));
}

TEST_CASE("(0+1i) * (0+1i) == -1") {
    auto expr = std::make_unique<BinaryOpExpr<Complex>>('*',
        std::make_unique<ConstantExpr<Complex>>(Complex(0,1)),
        std::make_unique<ConstantExpr<Complex>>(Complex(0,1)));
    REQUIRE(expr->compute({}) == Complex(-1,0));
}

// ==================== Парсинг выражений ====================
TEST_CASE("Parsing '3+4*5' == 23") {
    Lexer lexer("3+4*5");
    Parser<double> parser(lexer.tokenize());
    auto expr = parser.parse();
    REQUIRE(expr->compute({}) == Approx(23));
}

TEST_CASE("Parsing '2*(x+1)' with x=3 == 8") {
    Lexer lexer("2*(x+1)");
    Parser<double> parser(lexer.tokenize());
    auto expr = parser.parse();
    REQUIRE(expr->compute({{"x", 3}}) == Approx(8));
}

// ==================== Дифференцирование ====================
TEST_CASE("d/dx(x^3) at x=2 == 12") {
    auto expr = std::make_unique<BinaryOpExpr<double>>('^',
        std::make_unique<VariableExpr<double>>("x"),
        std::make_unique<ConstantExpr<double>>(3));
    auto deriv = expr->derivative("x");
    REQUIRE(deriv->toStr() == "((3) * (x ^ (2)))");
    REQUIRE(deriv->compute({{"x", 2}}) == Approx(12));
}

TEST_CASE("d/dx(cos(x)) at x=0 == 0") {
    auto expr = std::make_unique<FunctionExpr<double>>("cos",
        std::make_unique<VariableExpr<double>>("x"));
    auto deriv = expr->derivative("x");
    REQUIRE(deriv->toStr() == "((-1) * sin(x))");
    REQUIRE(deriv->compute({{"x", 0}}) == Approx(0).margin(0.0001));
}