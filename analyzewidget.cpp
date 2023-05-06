#include "analyzewidget.h"
#include "ui_analyzewidget.h"
#include "Handler/AnalysisWindowHandler.hpp"
#include <QVBoxLayout>
#include <QEvent>
#include "mainwindow.h"
#include <QScrollBar>

#include <iostream>

AnalyzeWidget::AnalyzeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnalyzeWidget),
    layout(new QVBoxLayout())
{
    ui->setupUi(this);

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    this->setLayout(layout);

    QScrollBar* scr = new QScrollBar(Qt::Horizontal, this);

    layout->addWidget(scr);

    connect(scr, &QScrollBar::valueChanged,
            AnalysisWindowHandler::getInstance(), &AnalysisWindowHandler::scrollBarHasChanged);

    MainWindow::instance->installEventFilter(this);
}

AnalyzeWidget::~AnalyzeWidget()
{
    delete ui;
    delete layout;
}

void AnalyzeWidget::wheelEvent(QWheelEvent *event) {
    AnalysisWindowHandler::getInstance()->scrollGraph(event->angleDelta().y());
}

void AnalyzeWidget::keyPressEvent(QKeyEvent* event) {
    if (AnalysisWindowHandler::getInstance()->isNullAnalyzeWidget()) return;

    auto code = event->nativeScanCode();
    //38 - a, 40 - d
    if (code == 30 || code == 32
            || code == 38 || code == 40) {
        long long x = (code == 38 || code == 30) ? -1 : 1;

        AnalysisWindowHandler::getInstance()->moveGraph(x);
    }

    if (event->key() == Qt::Key_Alt) {
        AnalysisWindowHandler::scaleMod = true;
    }
}

void AnalyzeWidget::keyReleaseEvent(QKeyEvent *event) {
    if (AnalysisWindowHandler::getInstance()->isNullAnalyzeWidget()) return;

    if (event->key() == Qt::Key_Alt) {
        AnalysisWindowHandler::scaleMod = false;
    }
}

bool AnalyzeWidget::eventFilter(QObject *obj, QEvent *event) {
    QKeyEvent *keyEvent = NULL;//event data, if this is a keystroke event
    QMouseEvent *mouseEvent = NULL;
    bool result = false;//return true to consume the keystroke

    if (event->type() == QEvent::KeyPress) {
         keyEvent = dynamic_cast<QKeyEvent*>(event);
         this->keyPressEvent(keyEvent);
         result = true;
    } else if (event->type() == QEvent::KeyRelease) {
         keyEvent = dynamic_cast<QKeyEvent*>(event);
         this->keyReleaseEvent(keyEvent);
         result = true;
    }

    //### Standard event processing ###
    else
        result = QObject::eventFilter(obj, event);

    return result;
}

void AnalyzeWidget::closeEvent(QCloseEvent *event)
{
    AnalysisWindowHandler::getInstance()->destroyWidget();
}

