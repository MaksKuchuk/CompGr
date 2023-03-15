#include "gltemplateoscillogram.h"
#include "../ui_gltemplateoscillogram.h"

glTemplateOscillogram::glTemplateOscillogram(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::glTemplateOscillogram)
{
    ui->setupUi(this);
}

glTemplateOscillogram::~glTemplateOscillogram()
{
    delete ui;
}
