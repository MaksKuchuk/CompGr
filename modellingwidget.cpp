#include "modellingwidget.h"
#include "ui_graphwidget.h"
#include "GraphGlData/Graph2DData.hpp"
#include <QVBoxLayout>
#include "mainwindow.h"
#include <QLineEdit>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include "glview.h"


#include "Parser/Parser.hpp"
#include "Parser/ParseData.hpp"

ModellingWidget::ModellingWidget(QWidget *parent)
: QWidget(parent),
  ui(new Ui::GraphWidget)
{
    ui->setupUi(this);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(layout);




    ParseData* pd = Parser::parse("C:\\samples\\a.txt");
    glView *gv = new glView(this, pd->getAmountOfSamples(), pd->channels[0], pd->maxVal(0), pd->minVal(0));

    gv->setFixedHeight(60);
    gv->setFixedWidth(300);

    layout->addWidget(gv);

    QDialogButtonBox *btn_box = new QDialogButtonBox(this);
    btn_box->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(btn_box);
}
