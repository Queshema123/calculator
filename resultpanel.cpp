#include "resultpanel.h"
#include "mainwindow.h"
#include <QObject>
#include <algorithm>
#include <stdexcept>
#include <cmath>


const QMap<QString, ResultPanel::unary_func_ptr> ResultPanel::unary_functions =
{
    {"SqrtBtn",        unary_func_ptr(new UnaryArithmeticFunction<double>(5, "sqrt",  [](double n) { return (n < 0.0)? throw std::logic_error("Sqrt of negate number") : sqrt(n); } )) },
    {"SquareBtn",      unary_func_ptr(new UnaryArithmeticFunction<double>(5, "sqr",   [](double n) { return n*n; } )) },
    {"DenominatorBtn", unary_func_ptr(new UnaryArithmeticFunction<double>(5, "denom", [](double n) { return (n == 0.0)? throw std::logic_error("Division by zero") : 1/n;} )) },
    {"PercentBtn",     unary_func_ptr(new UnaryArithmeticFunction<double>(5, "%",     [](double n) { return n * 0.01;} )) },
    {"SignBtn",        unary_func_ptr(new UnaryArithmeticFunction<double>(5, "~",     [](double n) { return -n;} )) }
};

const QMap<QString, ResultPanel::binary_func_ptr> ResultPanel::binary_functions =
{
    { "SumBtn",      binary_func_ptr(new BinaryArithmeticFunction<double>(1, "+", [](double l, double r){return l+r;})) },
    { "SubBtn",      binary_func_ptr(new BinaryArithmeticFunction<double>(1, "-", [](double l, double r){return l-r;})) },
    { "MulBtn",      binary_func_ptr(new BinaryArithmeticFunction<double>(2, "*", [](double l, double r){return l*r;})) },
    { "DivisionBtn", binary_func_ptr(new BinaryArithmeticFunction<double>(2, "/", [](double l, double r){ if(r == 0) throw std::logic_error("Division by zero"); return l/r;} )) },
};

const QMap<QString, ResultPanel::State> ResultPanel::states_map =
{
    {"PercentBtn",        ResultPanel::State::UnaryOperation},
    {"DenominatorBtn",    ResultPanel::State::UnaryOperation},
    {"SquareBtn",         ResultPanel::State::UnaryOperation},
    {"SqrtBtn",           ResultPanel::State::UnaryOperation},
    {"SignBtn",           ResultPanel::State::UnaryOperation},

    {"DivisionBtn",       ResultPanel::State::BinaryOperation},
    {"MulBtn",            ResultPanel::State::BinaryOperation},
    {"SubBtn",            ResultPanel::State::BinaryOperation},
    {"SumBtn",            ResultPanel::State::BinaryOperation},

    {"ZeroBtn",           ResultPanel::State::Digit},
    {"OneBtn",            ResultPanel::State::Digit},
    {"TwoBtn",            ResultPanel::State::Digit},
    {"ThreeBtn",          ResultPanel::State::Digit},
    {"FourBtn",           ResultPanel::State::Digit},
    {"FiveBtn",           ResultPanel::State::Digit},
    {"SixBtn",            ResultPanel::State::Digit},
    {"SevenBtn",          ResultPanel::State::Digit},
    {"EightBtn",          ResultPanel::State::Digit},
    {"NineBtn",           ResultPanel::State::Digit},

    {"ClearLastDigitBtn", ResultPanel::State::ClearLastDigit},
    {"ClearBtn",          ResultPanel::State::Clear},
    {"EraseSymbolBtn",    ResultPanel::State::Erase},
    {"CalculateBtn",      ResultPanel::State::Calculate},
    {"PointBtn",          ResultPanel::State::Point},
};

const QMap<QString, QChar> ResultPanel::operands =
{
    {"ZeroBtn",  '0'},
    {"OneBtn",   '1'},
    {"TwoBtn",   '2'},
    {"ThreeBtn", '3'},
    {"FourBtn",  '4'},
    {"FiveBtn",  '5'},
    {"SixBtn",   '6'},
    {"SevenBtn", '7'},
    {"EightBtn", '8'},
    {"NineBtn",  '9'},
    {"PointBtn", '.'}
};

ResultPanel::ResultPanel(QObject* parent) : QObject{parent}, m_left_number{"0.0"}, m_right_number{""}, m_operation{nullptr}
{
    m_cur_number = &m_left_number;
}

void ResultPanel::calculateUnaryOperation(const ArithmeticFunction<double>* operation)
{
    if(!operation)
        return;
    double result{ (*operation)(m_cur_number->toDouble()) };
    emit getUnaryOperationExpression(operation->getView() + "(" + (*m_cur_number) + ")", QString::number(result) );
    *m_cur_number = QString::number(result);
}

void ResultPanel::calculateBinaryOperation(const ArithmeticFunction<double>* operation)
{
    if(!m_operation)
        return;
    if(m_right_number.length() > 0)
    {
        double left_number { m_left_number.toDouble() };
        double right_number{ m_right_number.toDouble() };
        double result{ (*operation)(left_number, right_number) };

        emit getBinaryOperationExpression(m_left_number, operation->getView(), m_right_number, QString::number(result));

        m_left_number = QString::number(result);
        m_right_number.clear();
        m_operation = nullptr;
        m_cur_number = &m_left_number;
    }
}

void ResultPanel::clear()
{
    m_operation = nullptr;
    m_left_number = "0.0";
    m_right_number.clear();
    m_cur_number = &m_left_number;
}

void ResultPanel::engine(QObject* btn, State state)
{
    switch (state) {
    case State::BinaryOperation:
        calculateBinaryOperation(m_operation);
        m_operation = binary_functions[btn->objectName()].get() ;
        m_cur_number = &m_right_number;
        break;
    case State::UnaryOperation:
        calculateUnaryOperation( unary_functions[btn->objectName()].get() );
        break;
    case State::Digit:
    case State::Point:
        m_cur_number->push_back( operands[btn->objectName()] );
        break;
    case State::ClearLastDigit:
        m_cur_number->clear();
        break;
    case State::Clear:
        clear();
        break;
    case State::Erase:
        m_cur_number->chop(1);
        break;
    case State::Calculate:
        calculateBinaryOperation(m_operation);
        break;
    default:
        break;
    }
    emit getState(state);
}

void ResultPanel::processBtnClick()
{
    try
    {
        State state = states_map[QObject::sender()->objectName()];
        engine(QObject::sender(), state);
        QString result{m_left_number + " "};
        result += (m_operation) ? m_operation->getView(): "";
        result += " " + m_right_number;
        emit getResultExpr(result);
    }
    catch(std::exception& ex)
    {
        emit getResultExpr(ex.what());
    }
}

void ResultPanel::setDataFromHistory(const QString& result)
{
    clear();
    m_left_number = result;
    emit getResultExpr(m_left_number);
}
