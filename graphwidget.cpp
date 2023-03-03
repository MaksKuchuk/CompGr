#include "graphwidget.h"
#include "ui_graphwidget.h"
#include "glview.h"

#include <QVBoxLayout>

GraphWidget::GraphWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphWidget)
{
    ui->setupUi(this);
    glView *gView = new glView(this);
    glView *gView2 = new glView(this);
    glView *gView3 = new glView(this);

    QVBoxLayout *layout = new QVBoxLayout();

    layout->addWidget(gView);
    layout->addWidget(gView2);
    layout->addWidget(gView3);

    this->setLayout(layout);

}

GraphWidget::~GraphWidget()
{
    delete ui;
}

