#include "gltemplateoscillogram.h"
#include "../ui_gltemplateoscillogram.h"
#include "../glViewType/glOscillogram.hpp"
#include <QLabel>
#include <QVBoxLayout>
#include "../Handler/AnalysisWindowHandler.hpp"

#include <QMenu>

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
    layout->setContentsMargins(0, 5, 0, 5);

    layout->addWidget(gView);
    layout->setSpacing(3);
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
}

void glTemplateOscillogram::setGlobalScale() {
    if (data == nullptr) return;

    data->maxLoc = data->maxVal;
    data->minLoc = data->minVal;

    gView->updateGraph();
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
