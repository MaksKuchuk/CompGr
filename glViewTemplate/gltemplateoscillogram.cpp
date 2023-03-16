#include "gltemplateoscillogram.h"
#include "../ui_gltemplateoscillogram.h"
#include "../glViewType/glOscillogram.hpp"
#include <QLabel>
#include <QVBoxLayout>
#include "../Handler/AnalysisWindowHandler.hpp"

glTemplateOscillogram::glTemplateOscillogram(QWidget *parent, Graph2DData *data) :
    QWidget(parent),
    ui(new Ui::glTemplateOscillogram),
    data(data)
{
    ui->setupUi(this);

    gView = new glOscillogram(this, data);

    QLabel *label = new QLabel(this);
    label->setText(QString::fromUtf8(data->name));

    QFont font = label->font();
    font.setPixelSize(24);
    label->setFont(font);

    label->setAlignment(Qt::AlignCenter);
    label->setFixedHeight(18);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 5, 0, 5);

    layout->addWidget(gView);
    layout->setSpacing(3);
    layout->addWidget(label);
    this->setLayout(layout);
}

void glTemplateOscillogram::mousePressEvent(QMouseEvent *event) {
    AnalysisWindowHandler::getInstance()->setLocalRef(this);
}

glTemplateOscillogram::~glTemplateOscillogram()
{
    delete ui;
}
