#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QScopedPointer>
#include "resultpanel.h"
#include "operationhistory.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QScopedPointer<ResultPanel> ptr_panel;
    QScopedPointer<OperationHistory> ptr_history_wdgt;
    QLabel* calc_panel;
    void addBtn(const QString& object_name, QGridLayout* btn_layout, const QString& text_view, int row, int col, const QString& stylesheet = "");
    void addBtns(QGridLayout* btns_layout, const QString& stylesheet);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void setCalcPanelText(const QString& txt) { calc_panel->setText(txt); }
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
