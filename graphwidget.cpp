#include "graphwidget.h"
#include "ui_graphwidget.h"
#include "graphtemplate.h"
#include "GraphGlData/generaldata.h"
#include "mainwindow.h"
#include <memory>

#include <QVBoxLayout>

GraphWidget::GraphWidget(QWidget *parent, std::shared_ptr<GeneralData> pData) :
    QWidget(parent),
    ui(new Ui::GraphWidget),
    graphData(pData)
{
    ui->setupUi(this);
    if (pData == nullptr) return;

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
//    layout->setSpacing(0);

    for (int i = 0; i < pData->amountOfChannels; i++) {
        GraphTemplate* gTemp = new GraphTemplate(this, pData, i);

        layout->addWidget(gTemp);

        if (i != pData->getAmountOfChannels() - 1) layout->setSpacing(5);
    }

    this->setLayout(layout);

}

void GraphWidget::closeEvent(QCloseEvent *event) {
    MainWindow::grWid->graphData.reset();
}

GraphWidget::~GraphWidget()
{
    delete ui;
}

