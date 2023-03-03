#include "graphwidget.h"
#include "ui_graphwidget.h"
#include "graphtemplate.h"

#include <QVBoxLayout>

GraphWidget::GraphWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphWidget)
{
    ui->setupUi(this);
    GraphTemplate *gTemp = new GraphTemplate("name1", this);
    GraphTemplate *gTemp1 = new GraphTemplate("name2", this);
    GraphTemplate *gTemp2 = new GraphTemplate("name3", this);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    layout->addWidget(gTemp);
    layout->setSpacing(20);
    layout->addWidget(gTemp1);
    layout->setSpacing(20);
    layout->addWidget(gTemp2);

    this->setLayout(layout);

}

GraphWidget::~GraphWidget()
{
    delete ui;
}

