#include "analyzewidget.h"
#include "ui_analyzewidget.h"
#include "Handler/AnalysisWindowHandler.hpp"
#include <QVBoxLayout>
#include <QEvent>
#include "mainwindow.h"
#include <QScrollBar>

#include <iostream>

AnalyzeWidget::AnalyzeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnalyzeWidget),
    layout(new QVBoxLayout())
{
    ui->setupUi(this);

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    this->setLayout(layout);

    QScrollBar* scr = new QScrollBar(Qt::Horizontal, this);
    //QLabel* counter = new QLabel();

    //layout->addWidget(counter);
    layout->addWidget(scr);

    connect(scr, &QScrollBar::valueChanged,
            AnalysisWindowHandler::getInstance(), &AnalysisWindowHandler::scrollBarHasChanged);

//    MainWindow::instance->installEventFilter(this);
}

AnalyzeWidget::~AnalyzeWidget()
{
    delete ui;
    delete layout;
}


void AnalyzeWidget::closeEvent(QCloseEvent *event)
{
    AnalysisWindowHandler::getInstance()->destroyWidget();
}

