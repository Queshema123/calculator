#ifndef ARITHMETICFUNCTION_H
#define ARITHMETICFUNCTION_H
#include <QString>

template<typename T>
class ArithmeticFunction
{
    int priority;
    QString view;
public:
    ArithmeticFunction(int priority, const QString& view) : priority{priority}, view{view} { }
    int getPriority() const { return priority; }
    const QString& getView() const { return view; }
    virtual ~ArithmeticFunction() = default;
    virtual T operator()(T l = 0, T r = 0) const = 0;
};
#endif // ARITHMETICFUNCTION_H
