#include "graphwidget.h"
#include "ui_graphwidget.h"
#include "graphtemplate.h"
#include "Parser/ParseData.hpp"

#include <QVBoxLayout>

GraphWidget::GraphWidget(QWidget *parent, ParseData* pData) :
    QWidget(parent),
    ui(new Ui::GraphWidget)
{
    ui->setupUi(this);
    if (pData == nullptr) return;

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    for (int i = 0; i < pData->getAmountOfChannels(); i++) {
        GraphTemplate *gTemp = new GraphTemplate(
                    this,
                    pData->getChannelName(i),
                    pData->getAmountOfSamples(),
                    pData->getChannel(i),
                    pData->maxVal(i),
                    pData->minVal(i)
                    );

        layout->addWidget(gTemp);

        if (i != pData->getAmountOfChannels() - 1) layout->setSpacing(20);
    }

    this->setLayout(layout);

}

GraphWidget::~GraphWidget()
{
    delete ui;
}

