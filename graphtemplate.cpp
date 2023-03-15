#include "graphtemplate.h"
#include "ui_graphtemplate.h"
#include "glview.h"
#include "Parser/ParseData.hpp"
#include "Handler/AnalysisWindowHandler.hpp"
#include "Transformation/TransformToOscillogram.hpp"

#include <QVBoxLayout>
#include <QLabel>

GraphTemplate::GraphTemplate(QWidget *parent, ParseData* pData, long long ind) :
    QWidget(parent),
    ui(new Ui::GraphTemplate),
    pData(pData),
    ind(ind)
{
    ui->setupUi(this);

    glView *gView = new glView(this,
                               pData->getAmountOfSamples(),
                               pData->getChannel(ind),
                               pData->maxVal(ind),
                               pData->minVal(ind));
    gView->setFixedHeight(60);
    gView->setFixedWidth(300);

    QLabel *label = new QLabel(this);
    label->setText(QString::fromUtf8(pData->getChannelName(ind).c_str()));

    QFont font = label->font();
    font.setPixelSize(24);
    label->setFont(font);

    label->setAlignment(Qt::AlignCenter);
    label->setFixedHeight(18);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    layout->addWidget(gView);
    layout->setSpacing(3);
    layout->addWidget(label);
    this->setLayout(layout);
}

void GraphTemplate::mousePressEvent(QMouseEvent *event) {
    AnalysisWindowHandler::getInstance()->analyze2DBy(
                TransformToOscillogram::transform(pData, ind), glType::Oscillogram);

}

GraphTemplate::~GraphTemplate()
{
    delete ui;
}
