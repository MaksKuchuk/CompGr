#include "aboutwidget.h"
#include "ui_aboutwidget.h"

AboutWidget::AboutWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutWidget)
{
    ui->setupUi(this);
    ui->textBrowser->setGeometry(0, 0, 350, 100);
}

AboutWidget::~AboutWidget()
{
    delete ui;
}
