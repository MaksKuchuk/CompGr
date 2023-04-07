#include "gltemplateoscillogram.h"
#include "../ui_gltemplateoscillogram.h"
#include "../glViewType/glOscillogram.hpp"
#include <QLabel>
#include <QVBoxLayout>
#include <QPainter>
#include "../Handler/AnalysisWindowHandler.hpp"
#include "../mainwindow.h"
#include "../graphtemplate.h"
#include "../glview.h"

#include <QMenu>
#include <QDialog>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QFormLayout>

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
    label->setText(data->name);

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
    QAction* action3 = new QAction(QString::fromUtf8("Global scale"), this);
    menu->addAction(action3);
    QAction* action4 = new QAction(QString::fromUtf8("Set scale"), this);
    menu->addAction(action4);
    menu->addSeparator();
    QAction* action5 = new QAction(QString::fromUtf8("Global bias"), this);
    menu->addAction(action5);
    QAction* action6 = new QAction(QString::fromUtf8("Set bias"), this);
    menu->addAction(action6);
    menu->addSeparator();
    QAction* action7 = new QAction(QString::fromUtf8("Single local scale"), this);
    menu->addAction(action7);
    QAction* action8 = new QAction(QString::fromUtf8("Single global scale"), this);
    menu->addAction(action8);
    menu->addSeparator();
    QAction* action9 = new QAction(QString::fromUtf8("Default scale for all"), this);
    menu->addAction(action9);
    QAction* action10 = new QAction(QString::fromUtf8("Default bias for all"), this);
    menu->addAction(action10);


    QAction* selectedItem = menu->exec(globalPos);

    delete menu;

    if (selectedItem == nullptr) return;
    if (selectedItem->text() == "Close") {
        if (AnalysisWindowHandler::getInstance()->getLocalRef() == nullptr) return;
        AnalysisWindowHandler::getInstance()->getLocalRef()->deleteLater();
        AnalysisWindowHandler::getInstance()->setLocalRef(nullptr);
    } else if (selectedItem->text() == "Local scale") {
        setLocalScale();
    } else if (selectedItem->text() == "Global scale") {
        setGlobalScale();
    } else if (selectedItem->text() == "Set scale") {
        selectScale();
    } else if (selectedItem->text() == "Global bias") {
        setGlobalBias();
    } else if (selectedItem->text() == "Set bias") {
        selectBias();
    } else if (selectedItem->text() == "Single local scale") {
        selectSingleLocalScale();
    } else if (selectedItem->text() == "Single global scale") {
        selectSingleGlobalScale();
    } else if (selectedItem->text() == "Default scale for all") {
        setDefaultScale();
    } else if (selectedItem->text() == "Default bias for all") {
        setDefaultBias();
    }
}

void glTemplateOscillogram::setDefaultScale(){
    AnalysisWindowHandler::setDefaultScale();
}

void glTemplateOscillogram::setDefaultBias() {
    AnalysisWindowHandler::setDefaultBias();
}

void glTemplateOscillogram::selectSingleLocalScale() {
    if (data == nullptr) return;

    double lmi, lma;

    lmi = data->samples[data->lcur];
    lma = data->samples[data->lcur];
    for (long long i = data->lcur; i <= data->rcur; i++) {
        if (lmi > data->samples[i]) lmi = data->samples[i];
        if (lma < data->samples[i]) lma = data->samples[i];
    }

    AnalysisWindowHandler::changeSingleLocalScale(lmi, lma);
}

void glTemplateOscillogram::selectSingleGlobalScale() {
    AnalysisWindowHandler::setSingleGlobalScale();
}

void glTemplateOscillogram::selectScale() {
    QDialog dlg(this);
    dlg.setWindowTitle(tr("Y scale"));

    QLineEdit *ledit1 = new QLineEdit(&dlg);
    QLineEdit *ledit2 = new QLineEdit(&dlg);

    QDialogButtonBox *btn_box = new QDialogButtonBox(&dlg);
    btn_box->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(btn_box, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(btn_box, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    QFormLayout *layout = new QFormLayout();
    layout->addRow(tr("Y maximum"), ledit1);
    layout->addRow(tr("Y minimum:"), ledit2);
    layout->addWidget(btn_box);

    dlg.setLayout(layout);

    if(dlg.exec() == QDialog::Accepted) {
        if (data == nullptr) return;

        data->maxLoc = ledit1->text().toDouble();
        data->minLoc = ledit2->text().toDouble();

        gView->updateGraph();
        this->repaint();
    }
}

void glTemplateOscillogram::selectBias() {
    QDialog dlg(this);
    dlg.setWindowTitle(tr("X bias"));

    QLineEdit *ledit1 = new QLineEdit(&dlg);
    QLineEdit *ledit2 = new QLineEdit(&dlg);

    QDialogButtonBox *btn_box = new QDialogButtonBox(&dlg);
    btn_box->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(btn_box, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(btn_box, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    QFormLayout *layout = new QFormLayout();
    layout->addRow(tr("X left seconds"), ledit1);
    layout->addRow(tr("X right seconds"), ledit2);
    layout->addWidget(btn_box);

    dlg.setLayout(layout);

    if(dlg.exec() == QDialog::Accepted) {
        if (data == nullptr || ledit1->text().toDouble() >= ledit2->text().toDouble()) return;

        long long lx = ledit1->text().toDouble() * data->Hz;
        long long rx = ledit2->text().toDouble() * data->Hz;

        if (lx < 0 || rx >= data->amountOfSamples) return;

        data->lcur = lx;
        data->rcur = rx;

        AnalysisWindowHandler::updateGraphs(this);

        gView->updateGraph();
        this->repaint();
    }
}

void glTemplateOscillogram::setGlobalBias() {
    if (data == nullptr) return;

    data->lcur = 0;
    data->rcur = data->amountOfSamples - 1;

    AnalysisWindowHandler::updateGraphs(this);

    gView->updateGraph();
    this->repaint();
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
