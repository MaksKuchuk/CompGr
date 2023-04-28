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

    for (int i = 0; i < graphData->amountOfChannels; i++) {
        GraphTemplate* gTemp = new GraphTemplate(this, graphData, i);

        layout->addWidget(gTemp);

        if (i != pData->getAmountOfChannels() - 1) layout->setSpacing(5);
    }

    this->setLayout(layout);

}

void GraphWidget::AddNewChannel(std::shared_ptr<Graph2DData> newData) {
    AddNewChannel(newData, newData->name);
}

void GraphWidget::AddNewChannel(std::shared_ptr<Graph2DData> newData, QString name) {
    graphData->addNewChannel(newData, name);

    size_t i = graphData->amountOfChannels - 1;
    GraphTemplate* gTemp = new GraphTemplate(this, graphData, i);
    layout()->setSpacing(5);
    layout()->addWidget(gTemp);

    if (MainWindow::grWindow != nullptr)
        MainWindow::grWindow->resize(MainWindow::grWindow->sizeHint().width(), 85 * graphData->amountOfChannels + 20);
//        MainWindow::grWindow->adjustSize();
//        MainWindow::grWindow->resize(MainWindow::grWindow->sizeHint().width(), MainWindow::grWindow->sizeHint().height());
}

void GraphWidget::closeEvent(QCloseEvent *event) {
    MainWindow::grWid->graphData.reset();

    if (MainWindow::modelWindow != nullptr)
        MainWindow::modelWid->DisableToggle();
}

GraphWidget::~GraphWidget()
{
    delete ui;
}

