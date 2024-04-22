#ifndef UNARYARITHMETICFUNCTION_H
#define UNARYARITHMETICFUNCTION_H
#include "arithmeticfunction.h"
#include <functional>

template<typename T>
class UnaryArithmeticFunction : public ArithmeticFunction<T>
{
    std::function<T(T)> func;
public:
    UnaryArithmeticFunction(int priority, const QString& view, std::function<T(T)> f) : ArithmeticFunction<T>{priority, view} ,func{f} { }
    T operator()(T number, T unused = 0.0) const override { return func(number); }
    virtual ~UnaryArithmeticFunction() = default;
};

#endif // UNARYARITHMETICFUNCTION_H
