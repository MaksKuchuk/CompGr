#include "analyzewidget.h"
#include "ui_analyzewidget.h"
#include "Handler/AnalysisWindowHandler.hpp"

AnalyzeWidget::AnalyzeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnalyzeWidget)
{
    ui->setupUi(this);
}

AnalyzeWidget::~AnalyzeWidget()
{
    delete ui;
}

void AnalyzeWidget::closeEvent(QCloseEvent *event)
{
    AnalysisWindowHandler::getInstance()->destroyWidget();

}

