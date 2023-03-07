#include "graphshow.h"
#include "ui_graphshow.h"

GraphShow::GraphShow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphShow)
{
    ui->setupUi(this);
}

GraphShow::~GraphShow()
{
    delete ui;
}
