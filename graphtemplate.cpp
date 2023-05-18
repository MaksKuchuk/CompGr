#include "graphtemplate.h"
#include "ui_graphtemplate.h"
#include "glview.h"
#include "GraphGlData/generaldata.h"
#include "Handler/AnalysisWindowHandler.hpp"
#include "Transformation/TransformToOscillogram.hpp"
#include "Transformation/TransformToFourierSpectrum.hpp"
#include "Statistics/statistics.h"
#include "Statistics/statisticswidget.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QMenu>

#include "mainwindow.h"

GraphTemplate::GraphTemplate(QWidget *parent, const std::shared_ptr<GeneralData> gData, long long ind) :
    GraphTemplate(parent, gData->channelTo2D(ind)) {}

GraphTemplate::GraphTemplate(QWidget *parent, const std::shared_ptr<Graph2DData> gData, bool showName) :
    QWidget(parent),
    ui(new Ui::GraphTemplate),
    data(gData)
{
    ui->setupUi(this);

    gView = new glView(this,
                       data->amountOfSamples,
                       data->samples,
                       data->maxVal,
                       data->minVal);
    gView->setFixedHeight(60);
    gView->setFixedWidth(300);
    int height = 60;

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(gView);

    if (showName) {
        QLabel *label = new QLabel(this);
        label->setText(data->name);

        QFont font = label->font();
        font.setPixelSize(20);
        label->setFont(font);

        label->setAlignment(Qt::AlignCenter);
        label->setFixedHeight(18);
        layout->setSpacing(3);
        layout->addWidget(label);
        height += 21;
    }

    this->setLayout(layout);

    this->setFixedSize(300, height);
}

void GraphTemplate::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        drawMenu(QCursor::pos());
    }
}

void GraphTemplate::drawMenu(QPoint globalPos) {
    QMenu *menu = new QMenu();

    menu->addAction(new QAction(QString::fromUtf8("Oscillogram"), this));
//    menu->addSeparator();
    menu->addAction(new QAction(QString::fromUtf8("Fourier spectrum"), this));
//    menu->addSeparator();
    menu->addAction(new QAction(QString::fromUtf8("Waveletogram"), this));
    menu->addSeparator();
    menu->addAction(new QAction(QString::fromUtf8("Statistics"), this));


    QAction* selectedItem = menu->exec(globalPos);

    delete menu;

    if (selectedItem == nullptr) return;
    if (selectedItem->text() == "Oscillogram") {
        MainWindow::openAnalysisWindow();

        AnalysisWindowHandler::getInstance()->analyze2DBy(
            data, glType::Oscillogram, this);
    } else if (selectedItem->text() == "Fourier spectrum") {
        MainWindow::openAnalysisWindow();

        AnalysisWindowHandler::getInstance()->analyze2DBy(
            data, glType::FourierSpectrum, this);
    } else if (selectedItem->text() == "Waveletogram") {
        MainWindow::openAnalysisWindow();
    } else if (selectedItem->text() == "Statistics") {
        statsWid = new StatisticsWidget(this, data);
        if (statsWid->isCorrect)
            MainWindow::AddWidget(statsWid);
    }
}

GraphTemplate::~GraphTemplate()
{
    delete ui;
}
