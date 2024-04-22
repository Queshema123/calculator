#ifndef RESULTPANEL_H
#define RESULTPANEL_H

#include <QObject>
#include <QLabel>
#include <QMap>
#include <QPushButton>
#include <QSharedPointer>
#include <QHash>
#include "unaryarithmeticfunction.h"
#include "binaryarithmeticfunction.h"
#include "arithmeticfunction.h"

class ResultPanel : public QObject
{
public:
    enum class State{Digit, BinaryOperation, UnaryOperation, Point, Clear, Erase, ClearLastDigit, Calculate,  Default};
private:
    Q_OBJECT

    typedef QSharedPointer<UnaryArithmeticFunction<double>> unary_func_ptr;
    static const QMap<QString, unary_func_ptr> unary_functions;

    typedef QSharedPointer<BinaryArithmeticFunction<double>> binary_func_ptr;
    static const QMap<QString, binary_func_ptr> binary_functions;

    static const QMap<QString, State> states_map;
    static const QMap<QString, QChar> operands;

    QString m_left_number, m_right_number;
    QString* m_cur_number;
    const ArithmeticFunction<double>* m_operation;

    void calculateUnaryOperation(const ArithmeticFunction<double>* operation);
    void calculateBinaryOperation(const ArithmeticFunction<double>* operation);
    void clear();
    void engine(QObject* btn, State state);
public:
    explicit ResultPanel(QObject* parent = nullptr);
    ~ResultPanel() = default;
signals:
    void getState(State st);
    void getResultExpr(const QString& result);
    void getBinaryOperationExpression(const QString& left_number, const QString& operation, const QString& right_number, const QString& result);
    void getUnaryOperationExpression(const QString& expression, const QString& result);
public slots:
    void processBtnClick();
    void setDataFromHistory(const QString& result);
};
#endif // RESULTPANEL_H
