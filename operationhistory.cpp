#include "operationhistory.h"
#include "resultpanel.h"
#include <QPushButton>
#include <QObject>
#include <QVBoxLayout>

OperationHistory::OperationHistory(QWidget *parent)
    : QWidget{parent}, m_left_expr{""}, m_right_expr{""}, m_result{""}, m_curr_expr_ptr{nullptr}, m_history_list{new QListWidget}
{
    m_curr_expr_ptr = &m_left_expr;
    QVBoxLayout* layout = new QVBoxLayout;
    this->setLayout(layout);
    this->setStyleSheet("background-color: #333333; color: white; font: 20px; margin: 2px; padding: 3px");
    layout->insertWidget(0, m_history_list, 2);
    QPushButton* reset_history_btn = new QPushButton("Reset");
    reset_history_btn->setStyleSheet("max-width: 50px; max-height: 50px");
    layout->insertWidget(1, reset_history_btn);
    QObject::connect(reset_history_btn, &QPushButton::clicked,     m_history_list, &QListWidget::clear);
    QObject::connect(reset_history_btn, &QPushButton::clicked,     this,           &OperationHistory::reset);
    QObject::connect(m_history_list,    &QListWidget::itemClicked, this,           &OperationHistory::getRecordResult);
}

void OperationHistory::getRecordResult(QListWidgetItem* item)
{
    emit recordClicked(item->data(Qt::UserRole).toString());
}

void OperationHistory::addRecordByExpr(const QString& record)
{
    QListWidgetItem* item = new QListWidgetItem(record);
    item->setData( Qt::UserRole, record.mid( record.indexOf('=') + 2 ));
    m_history_list->addItem( item );
    reset();
}

void OperationHistory::addRecord()
{
    if(m_left_expr.length() > 0)
        addRecordByExpr(m_left_expr + " = " + m_result);
}

void OperationHistory::addUnaryOperationResult(const QString& expression, const QString& result)
{
    if(m_curr_expr_ptr->indexOf(')') == -1)
        *m_curr_expr_ptr = expression;
    else
    {
        m_curr_expr_ptr->insert(m_curr_expr_ptr->lastIndexOf('(')+1, expression.mid(0, expression.indexOf('(')+1) ); // +1 для пропуска самой скобки
        m_curr_expr_ptr->append(')'); // Поскольку не парсится ')' с expression
    }
    m_result = result;
}

void OperationHistory::addBinaryOperation(const QString& left_number, const QString& operation, const QString& right_number, const QString& result)
{
    if(m_left_expr.length() == 0)
        m_left_expr = left_number;

    if(m_right_expr.length() == 0)
        m_right_expr = right_number;

    addRecordByExpr(m_left_expr + " " + operation + " " + m_right_expr + " = " + result);
}

void OperationHistory::reset()
{
    m_left_expr = "";
    m_right_expr = "";
    m_result = "";
    m_curr_expr_ptr = &m_left_expr;
}

void OperationHistory::changeCurrentExpression()
{
    m_curr_expr_ptr = &m_right_expr;
}

void OperationHistory::engine(ResultPanel::State st)
{
    switch (st) {
    case ResultPanel::State::ClearLastDigit:
        m_curr_expr_ptr->clear();
        break;
    case ResultPanel::State::BinaryOperation:
        changeCurrentExpression();
        break;
    case ResultPanel::State::Clear:
        reset();
        break;
    case ResultPanel::State::Calculate:
        addRecord();
        break;
    default:
        break;
    }
}
