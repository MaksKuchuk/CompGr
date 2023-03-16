#include "graph2ddatatemplate.h"
#include "ui_graph2ddatatemplate.h"

Graph2DDataTemplate::Graph2DDataTemplate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Graph2DDataTemplate)
{
    ui->setupUi(this);
}

Graph2DDataTemplate::~Graph2DDataTemplate()
{
    delete ui;
}
