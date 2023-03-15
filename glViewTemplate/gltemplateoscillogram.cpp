#include "gltemplateoscillogram.h"
#include "../ui_gltemplateoscillogram.h"

glTemplateOscillogram::glTemplateOscillogram(QWidget *parent, Graph2DData *data) :
    QWidget(parent),
    ui(new Ui::glTemplateOscillogram),
    data(data)
{
    ui->setupUi(this);
}

glTemplateOscillogram::~glTemplateOscillogram()
{
    delete ui;
}
