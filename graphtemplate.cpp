#include "graphtemplate.h"
#include "ui_graphtemplate.h"
#include "glview.h"
#include "GraphGlData/generaldata.h"
#include "Handler/AnalysisWindowHandler.hpp"
#include "Transformation/TransformToOscillogram.hpp"
#include "Transformation/TransformToFourierSpectrum.hpp"

#include <QVBoxLayout>
#include <QLabel>
#include <QMenu>

#include "mainwindow.h"

GraphTemplate::GraphTemplate(QWidget *parent, std::shared_ptr<GeneralData> pData, long long ind) :
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
    label->setText(pData->getChannelName(ind));

    QFont font = label->font();
    font.setPixelSize(24);
    label->setFont(font);

    label->setAlignment(Qt::AlignCenter);
    label->setFixedHeight(18);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(gView);
    layout->setSpacing(3);
    layout->addWidget(label);
    this->setLayout(layout);
}

void GraphTemplate::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        drawMenu(QCursor::pos());
    }
}

void GraphTemplate::drawMenu(QPoint globalPos) {
    QMenu *menu = new QMenu();

    QAction* action1 = new QAction(QString::fromUtf8("Oscillogram"), this);
    menu->addAction(action1);
    menu->addSeparator();
    QAction* action2 = new QAction(QString::fromUtf8("Fourier spectrum"), this);
    menu->addAction(action2);
    menu->addSeparator();
    QAction* action3 = new QAction(QString::fromUtf8("Waveletogram"), this);
    menu->addAction(action3);

    QAction* selectedItem = menu->exec(globalPos);

    delete menu;

    if (selectedItem == nullptr) return;
    if (selectedItem->text() == "Oscillogram") {
        MainWindow::openAnalysisWindow();

        AnalysisWindowHandler::getInstance()->analyze2DBy(
            TransformToOscillogram::transform(pData, ind), glType::Oscillogram);
    } else if (selectedItem->text() == "Fourier spectrum") {
        MainWindow::openAnalysisWindow();

        AnalysisWindowHandler::getInstance()->analyze2DBy(
            TransformToFourierSpectrum::transform(pData, ind), glType::FourierSpectrum);
    } else if (selectedItem->text() == "Waveletogram") {
        MainWindow::openAnalysisWindow();
    }
}

GraphTemplate::~GraphTemplate()
{
    delete ui;
}
