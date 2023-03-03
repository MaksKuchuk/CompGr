#include "graphtemplate.h"
#include "ui_graphtemplate.h"

GraphTemplate::GraphTemplate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphTemplate)
{
    ui->setupUi(this);
}

GraphTemplate::~GraphTemplate()
{
    delete ui;
}
