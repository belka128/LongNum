#ifndef EXPRESSION_H
#define EXPRESSION_H
#include <string>
#include <memory>
#include <map>
#include <cmath>
#include <sstream>
#include <stdexcept>
#include <complex>

struct Invalid_Operation : public std::exception {
    const char* what() const noexcept {
        return "Invalid operation";
    }
};

template<typename T>
class Expression {
public:
    virtual ~Expression() = default;
    virtual T compute(const std::map<std::string, T>& vars) const = 0;
    virtual std::string toStr() const = 0;
    virtual std::unique_ptr<Expression<T>> derivative(const std::string& var) const = 0;
    virtual std::unique_ptr<Expression<T>> replace(const std::string& var, const T& val) const = 0;
    
    virtual std::unique_ptr<Expression<T>> copy() const = 0;
    virtual bool isConst() const = 0;
    
    Expression() = default;
    Expression(const Expression&) = delete;
    Expression(Expression&&) = delete;
    Expression& operator=(const Expression&) = delete;
    Expression& operator=(Expression&&) = delete;
};

template<typename T>
class ConstantExpr : public Expression<T> {
    T val;
public:
    explicit ConstantExpr(T v) : val(v) {}

    T compute(const std::map<std::string, T>&) const override {
        return val;
    }

    std::string toStr() const override {
        std::ostringstream oss;
        if constexpr (std::is_same_v<T, double>) {
            if (val == std::floor(val)) {
                oss << "(" << static_cast<long long>(val) << ")";
            } else {
                oss << "(" << val << ")";
            }
        } else {
            oss << "(" << val.real() << "+" << val.imag() << "i)";
        }
        return oss.str();
    }

    std::unique_ptr<Expression<T>> derivative(const std::string&) const override {
        return std::make_unique<ConstantExpr<T>>(0);
    }

    std::unique_ptr<Expression<T>> replace(const std::string&, const T&) const override {
        return std::make_unique<ConstantExpr<T>>(val);
    }

    std::unique_ptr<Expression<T>> copy() const override {
        return std::make_unique<ConstantExpr<T>>(val);
    }

    bool isConst() const override { return true; }
};

template<typename T>
class VariableExpr : public Expression<T> {
    std::string varName;
public:
    explicit VariableExpr(const std::string& name) : varName(name) {}

    T compute(const std::map<std::string, T>& vars) const override {
        return vars.at(varName);
    }

    std::string toStr() const override {
        return varName;
    }

    std::unique_ptr<Expression<T>> derivative(const std::string& var) const override {
        return std::make_unique<ConstantExpr<T>>(varName == var ? 1 : 0);
    }

    std::unique_ptr<Expression<T>> replace(const std::string& var, const T& val) const override {
        if (varName == var) {
            return std::make_unique<ConstantExpr<T>>(val);
        }
        return std::make_unique<VariableExpr<T>>(varName);
    }

    std::unique_ptr<Expression<T>> copy() const override {
        return std::make_unique<VariableExpr<T>>(varName);
    }

    bool isConst() const override { return false; }
};

template<typename T>
class BinaryOpExpr : public Expression<T> {
    char operation;
    std::unique_ptr<Expression<T>> lhs;
    std::unique_ptr<Expression<T>> rhs;

public:
    BinaryOpExpr(char op, std::unique_ptr<Expression<T>> left, 
                std::unique_ptr<Expression<T>> right)
        : operation(op), lhs(std::move(left)), rhs(std::move(right)) {}

    T compute(const std::map<std::string, T>& vars) const override {
        const T leftVal = lhs->compute(vars);
        const T rightVal = rhs->compute(vars);
        
        switch (operation) {
            case '+': return leftVal + rightVal;
            case '-': return leftVal - rightVal;
            case '*': return leftVal * rightVal;
            case '/': {
                if constexpr (std::is_same_v<T, double>) {
                    if (rightVal == 0) throw Invalid_Operation();
                } else if (rightVal == T(0)) {
                    throw Invalid_Operation();
                }
                return leftVal / rightVal;
            }
            case '^': return std::pow(leftVal, rightVal);
            default: throw Invalid_Operation();
        }
    }

    std::string toStr() const override {
        return "(" + lhs->toStr() + " " + operation + " " + rhs->toStr() + ")";
    }

    std::unique_ptr<Expression<T>> derivative(const std::string& var) const override {
        auto leftDeriv = lhs->derivative(var);
        auto rightDeriv = rhs->derivative(var);

        switch (operation) {
            case '+':
            case '-':
                return std::make_unique<BinaryOpExpr<T>>(
                    operation, std::move(leftDeriv), std::move(rightDeriv));

            case '*': {
                auto term1 = std::make_unique<BinaryOpExpr<T>>('*', std::move(leftDeriv), rhs->copy());
                auto term2 = std::make_unique<BinaryOpExpr<T>>('*', lhs->copy(), std::move(rightDeriv));
                return std::make_unique<BinaryOpExpr<T>>('+', std::move(term1), std::move(term2));
            }

            case '/': {
                auto numerator = std::make_unique<BinaryOpExpr<T>>(
                    '-',
                    std::make_unique<BinaryOpExpr<T>>('*', std::move(leftDeriv), rhs->copy()),
                    std::make_unique<BinaryOpExpr<T>>('*', lhs->copy(), std::move(rightDeriv))
                );
                auto denominator = std::make_unique<BinaryOpExpr<T>>('*', rhs->copy(), rhs->copy());
                return std::make_unique<BinaryOpExpr<T>>('/', std::move(numerator), std::move(denominator));
            }

            case '^':
                if (rhs->isConst()) {
                    const T power = rhs->compute({});
                    auto newExpr = std::make_unique<BinaryOpExpr<T>>(
                        '^',
                        lhs->copy(),
                        std::make_unique<ConstantExpr<T>>(power - T(1))
                    );
                    auto coeff = std::make_unique<ConstantExpr<T>>(power);
                    auto term = std::make_unique<BinaryOpExpr<T>>('*', std::move(coeff), std::move(newExpr));
                    auto diff = lhs->derivative(var);
                    
                    if (diff->isConst() && diff->compute({}) == T(1)) {
                        return term;
                    }
                    return std::make_unique<BinaryOpExpr<T>>('*', std::move(term), std::move(diff));
                }
                throw std::runtime_error("Non-constant exponents not supported");

            default:
                throw Invalid_Operation();
        }
    }

    std::unique_ptr<Expression<T>> replace(const std::string& var, const T& val) const override {
        return std::make_unique<BinaryOpExpr<T>>(
            operation,
            lhs->replace(var, val),
            rhs->replace(var, val)
        );
    }

    std::unique_ptr<Expression<T>> copy() const override {
        return std::make_unique<BinaryOpExpr<T>>(operation, lhs->copy(), rhs->copy());
    }

    bool isConst() const override {
        return lhs->isConst() && rhs->isConst();
    }
};

template<typename T>
class FunctionExpr : public Expression<T> {
    std::string func;
    std::unique_ptr<Expression<T>> arg;

public:
    FunctionExpr(const std::string& fn, std::unique_ptr<Expression<T>> a)
        : func(fn), arg(std::move(a)) {}

    T compute(const std::map<std::string, T>& vars) const override {
        const T val = arg->compute(vars);
        
        if (func == "sin") return std::sin(val);
        if (func == "cos") return std::cos(val);
        if (func == "ln") {
            if constexpr (std::is_same_v<T, double>) {
                if (val <= 0) throw Invalid_Operation();
            } else if (val.real() <= 0 && val.imag() == 0) {
                throw Invalid_Operation();
            }
            return std::log(val);
        }
        if (func == "exp") return std::exp(val);
        
        throw Invalid_Operation();
    }

    std::string toStr() const override {
        return func + "(" + arg->toStr() + ")";
    }

    std::unique_ptr<Expression<T>> derivative(const std::string& var) const override {
        auto argDeriv = arg->derivative(var);

        if (func == "sin") {
            auto derived = std::make_unique<FunctionExpr<T>>("cos", arg->copy());
            return createDerivativeTerm(std::move(derived), std::move(argDeriv));
        }
        if (func == "cos") {
            auto derived = std::make_unique<BinaryOpExpr<T>>(
                '*',
                std::make_unique<ConstantExpr<T>>(-1),
                std::make_unique<FunctionExpr<T>>("sin", arg->copy())
            );
            return createDerivativeTerm(std::move(derived), std::move(argDeriv));
        }
        if (func == "ln") {
            return std::make_unique<BinaryOpExpr<T>>('/', std::move(argDeriv), arg->copy());
        }
        if (func == "exp") {
            auto derived = std::make_unique<FunctionExpr<T>>("exp", arg->copy());
            return createDerivativeTerm(std::move(derived), std::move(argDeriv));
        }
        
        throw Invalid_Operation();
    }

private:
    std::unique_ptr<Expression<T>> createDerivativeTerm(
        std::unique_ptr<Expression<T>> derivedFn,
        std::unique_ptr<Expression<T>> innerDeriv
    ) const {
        if (innerDeriv->isConst() && innerDeriv->compute({}) == T(1)) {
            return derivedFn;
        }
        return std::make_unique<BinaryOpExpr<T>>('*', std::move(derivedFn), std::move(innerDeriv));
    }

public:
    std::unique_ptr<Expression<T>> replace(const std::string& var, const T& val) const override {
        return std::make_unique<FunctionExpr<T>>(func, arg->replace(var, val));
    }

    std::unique_ptr<Expression<T>> copy() const override {
        return std::make_unique<FunctionExpr<T>>(func, arg->copy());
    }

    bool isConst() const override {
        return arg->isConst();
    }
};
#endif