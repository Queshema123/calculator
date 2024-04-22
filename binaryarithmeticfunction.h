#ifndef BINARYARITHMETICFUNCTION_H
#define BINARYARITHMETICFUNCTION_H
#include "arithmeticfunction.h"
#include <QString>
#include <functional>

template<typename T>
class BinaryArithmeticFunction : public ArithmeticFunction<T>
{
    std::function<T(T, T)> func;
public:
    BinaryArithmeticFunction(int priority, const QString & view, std::function<T(T, T)> f) : ArithmeticFunction<T>{priority, view}, func{ f } { }
    T operator()(T left, T right) const override  { return func(left, right); }
    virtual ~BinaryArithmeticFunction() = default;
};

#endif // BINARYARITHMETICFUNCTION_H
