#include "analyzewidget.h"
#include "ui_analyzewidget.h"
#include "Handler/AnalysisWindowHandler.hpp"
#include <QVBoxLayout>

AnalyzeWidget::AnalyzeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnalyzeWidget),
    layout(new QVBoxLayout())
{
    ui->setupUi(this);

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    this->setLayout(layout);
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

