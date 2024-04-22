#ifndef OPERATIONHISTORY_H
#define OPERATIONHISTORY_H
#include "resultpanel.h"

#include <QWidget>
#include <QListWidget>

class OperationHistory : public QWidget
{
    Q_OBJECT
    QString m_left_expr, m_right_expr, m_result;
    QString* m_curr_expr_ptr;
    QListWidget* m_history_list;

    void addRecordByExpr(const QString& record);
    void changeCurrentExpression();
    void addRecord();
    void reset();
    void getRecordResult(QListWidgetItem* item);
public:
    explicit OperationHistory(QWidget *parent = nullptr);
public slots:
    void engine(ResultPanel::State st);
    void addUnaryOperationResult(const QString& expression, const QString& result);
    void addBinaryOperation(const QString& left_number, const QString& operation, const QString& right_number, const QString& result);
signals:
    void recordClicked(const QString& result);
};

#endif // OPERATIONHISTORY_H
