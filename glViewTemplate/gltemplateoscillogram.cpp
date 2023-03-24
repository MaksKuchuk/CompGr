#include "gltemplateoscillogram.h"
#include "../ui_gltemplateoscillogram.h"
#include "../glViewType/glOscillogram.hpp"
#include <QLabel>
#include <QVBoxLayout>
#include <QPainter>
#include "../Handler/AnalysisWindowHandler.hpp"

#include <QMenu>

void glTemplateOscillogram::paintEvent(QPaintEvent *event) {
    int margin = 65; // adjust as needed
    int xAxisY = gView->height() + gView->y();
    int yAxisX = margin;
    QPainter painter(this);
    painter.drawLine(yAxisX, 5, yAxisX, xAxisY); // draw the y-axis
    painter.drawLine(yAxisX, xAxisY, gView->width() - margin, xAxisY); // draw the x-axis

    int tickLength = 5; // adjust as needed
    int numTicks = 10; // adjust as needed
    int xTickSpacing = 100;
    int yTickSpacing = 60;
    int numTicksX = gView->width() / xTickSpacing + 1;
    int numTicksY = gView->height() / yTickSpacing + 1;

    for (int i = 0; i < numTicksX; i++) {
        // draw tick on x-axis
        int x = (yAxisX + i * gView->width() / numTicksX);
        painter.drawLine(x, xAxisY, x, xAxisY + tickLength);

        // add tick labels
        QString xTickLabel = QString::number(
                    (long long)(100*((data->lcur + i*(data->rcur - data->lcur) / numTicksX) / data->Hz)) / 100.0
                    );
        painter.drawText(x - tickLength / 2, xAxisY + tickLength + 10, xTickLabel);
    }

    for (int i = 0; i < numTicksY; i++) {
        // draw tick on y-axis
        int y = xAxisY - i * gView->height() / numTicksY;
        painter.drawLine(yAxisX - tickLength, y, yAxisX, y);

        // add tick labels
        QString yTickLabel = QString::number(
                    (long long)(100*(data->minLoc + i * (data->maxLoc - data->minLoc) / numTicksY)) / 100.0
                    );
        painter.drawText(yAxisX - tickLength - 5 - painter.fontMetrics()
                         .horizontalAdvance(yTickLabel),
                         y + painter.fontMetrics().height() / 2, yTickLabel);
    }
}

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
    label->setFixedHeight(22);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(65, 5, 35, 5);

    layout->addWidget(gView);
    layout->setSpacing(15);
    layout->addWidget(label);

    this->setLayout(layout);
}

void glTemplateOscillogram::drawMenu(QPoint globalPos) {
    QMenu *menu = new QMenu();

    QAction* action1 = new QAction(QString::fromUtf8("Close"), this);
    menu->addAction(action1);
    menu->addSeparator();
    QAction* action2 = new QAction(QString::fromUtf8("Local scale"), this);
    menu->addAction(action2);
    menu->addSeparator();
    QAction* action3 = new QAction(QString::fromUtf8("Global scale"), this);
    menu->addAction(action3);

    QAction* selectedItem = menu->exec(globalPos);

    delete menu;

    if (selectedItem == nullptr) return;
    if (selectedItem->text() == "Close") {
        if (AnalysisWindowHandler::getInstance()->getLocalRef() == nullptr) return;
        AnalysisWindowHandler::getInstance()->getLocalRef()->deleteLater();
    } else if (selectedItem->text() == "Local scale") {
        setLocalScale();
    } else if (selectedItem->text() == "Global scale") {
        setGlobalScale();
    }
}

void glTemplateOscillogram::setLocalScale() {
    if (data == nullptr) return;

    double lmi, lma;

    lmi = data->samples[data->lcur];
    lma = data->samples[data->lcur];
    for (long long i = data->lcur; i <= data->rcur; i++) {
        if (lmi > data->samples[i]) lmi = data->samples[i];
        if (lma < data->samples[i]) lma = data->samples[i];
    }

    data->minLoc = lmi;
    data->maxLoc = lma;

    gView->updateGraph();
    this->repaint();
}

void glTemplateOscillogram::setGlobalScale() {
    if (data == nullptr) return;

    data->maxLoc = data->maxVal;
    data->minLoc = data->minVal;

    gView->updateGraph();
    this->repaint();
}

void glTemplateOscillogram::mousePressEvent(QMouseEvent *event) {
    AnalysisWindowHandler::getInstance()->setLocalRef(this);

    if (event->button() == Qt::RightButton) {
        drawMenu(QCursor::pos());
    }
}

glTemplateOscillogram::~glTemplateOscillogram()
{
    delete ui;
}
