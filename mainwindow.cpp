#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QObject>

void MainWindow::addBtn(const QString& object_name, QGridLayout* btn_layout, const QString& text_view, int row, int col, const QString& stylesheet)
{
    QPushButton* btn = new QPushButton(text_view);
    btn->setObjectName(object_name);
    btn->setStyleSheet(stylesheet);
    btn_layout->addWidget(btn, row, col);
    QObject::connect(btn, &QPushButton::clicked, ptr_panel.get(), &ResultPanel::processBtnClick);
}

void MainWindow::addBtns(QGridLayout* btns_layout, const QString& style)
{
    addBtn("PercentBtn", btns_layout, "%", 0, 0, style);
    addBtn("ClearLastDigitBtn", btns_layout, "CE", 0, 1, style);
    addBtn("ClearBtn", btns_layout, "C", 0, 2, style);
    addBtn("EraseSymbolBtn", btns_layout, "<-", 0, 3, style);

    addBtn("DenominatorBtn", btns_layout, "1/x", 1, 0, style);
    addBtn("SquareBtn", btns_layout, "x^2", 1, 1, style);
    addBtn("SqrtBtn", btns_layout, "sqrt", 1, 2, style);
    addBtn("DivisionBtn", btns_layout, "/", 1, 3, style);

    addBtn("SevenBtn", btns_layout, "7", 2, 0, style);
    addBtn("EightBtn", btns_layout, "8", 2, 1, style);
    addBtn("NineBtn", btns_layout, "9", 2, 2, style);
    addBtn("MulBtn", btns_layout, "*", 2, 3, style);

    addBtn("FourBtn", btns_layout, "4", 3, 0, style);
    addBtn("FiveBtn", btns_layout, "5", 3, 1, style);
    addBtn("SixBtn", btns_layout, "6", 3, 2, style);
    addBtn("SubBtn", btns_layout, "-", 3, 3, style);

    addBtn("OneBtn", btns_layout, "1", 4, 0, style);
    addBtn("TwoBtn", btns_layout, "2", 4, 1, style);
    addBtn("ThreeBtn", btns_layout, "3", 4, 2, style);
    addBtn("SumBtn", btns_layout, "+", 4, 3, style);

    addBtn("SignBtn", btns_layout, "+/-", 5, 0, style);
    addBtn("ZeroBtn", btns_layout, "0", 5, 1, style);
    addBtn("PointBtn", btns_layout, ".", 5, 2, style);
    addBtn("CalculateBtn", btns_layout, "=", 5, 3, style);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ptr_panel{new ResultPanel}, ptr_history_wdgt{new OperationHistory}, calc_panel{new QLabel("0.0")}
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QHBoxLayout* main_layout = new QHBoxLayout;
    centralWidget()->setLayout(main_layout);

    QVBoxLayout* calc_v_layout = new QVBoxLayout;
    QWidget* calc_widget = new QWidget;
    calc_widget->setLayout(calc_v_layout);
    main_layout->insertWidget(0, calc_widget, 1);
    main_layout->insertWidget(1, ptr_history_wdgt.get(), 1);

    calc_v_layout->addWidget(calc_panel);
    calc_panel->setStyleSheet("color: white; font: 40px");
    this->setStyleSheet("background-color: #1a1a1a");
    QWidget* btns = new QWidget;
    QGridLayout* btns_layout = new QGridLayout;
    btns->setLayout(btns_layout);
    btns_layout->setContentsMargins(QMargins(0, 0, 0, 0));
    btns_layout->setSpacing(0);
    calc_v_layout->addWidget(btns);

    QString style{"background-color: #333333; color: white; font: 20px; margin: 2px; padding: 3px; min-height: 30px"};

    addBtns(btns_layout, style);

    QObject::connect(ptr_panel.get(),         &ResultPanel::getResultExpr,                this,                    &MainWindow::setCalcPanelText);
    QObject::connect(ptr_history_wdgt.get(),  &OperationHistory::recordClicked,             ptr_panel.get(),       &ResultPanel::setDataFromHistory);
    QObject::connect(ptr_panel.get(),         &ResultPanel::getState,                     ptr_history_wdgt.get(),  &OperationHistory::engine);
    QObject::connect(ptr_panel.get(),         &ResultPanel::getUnaryOperationExpression,  ptr_history_wdgt.get(),  &OperationHistory::addUnaryOperationResult);
    QObject::connect(ptr_panel.get(),         &ResultPanel::getBinaryOperationExpression, ptr_history_wdgt.get(),  &OperationHistory::addBinaryOperation);
}

MainWindow::~MainWindow()
{
    delete ui;
}
