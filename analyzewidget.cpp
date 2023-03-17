#include "analyzewidget.h"
#include "ui_analyzewidget.h"
#include "Handler/AnalysisWindowHandler.hpp"
#include <QVBoxLayout>
#include <QEvent>
#include "mainwindow.h"

AnalyzeWidget::AnalyzeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnalyzeWidget),
    layout(new QVBoxLayout())
{
    ui->setupUi(this);

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    this->setLayout(layout);

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

    long long x = 0;
    if (event->key() == Qt::Key_A) {
        x = -1;
    } else if (event->key() == Qt::Key_D) {
        x = 1;
    }
    if (x == 0) return;

    AnalysisWindowHandler::getInstance()->moveGraph(x);
}

bool AnalyzeWidget::eventFilter(QObject *obj, QEvent *event) {
    QKeyEvent *keyEvent = NULL;//event data, if this is a keystroke event
    bool result = false;//return true to consume the keystroke

    if (event->type() == QEvent::KeyPress)
    {
         keyEvent = dynamic_cast<QKeyEvent*>(event);
         this->keyPressEvent(keyEvent);
         result = true;
    }//if type()

    //### Standard event processing ###
    else
        result = QObject::eventFilter(obj, event);

    return result;
}

void AnalyzeWidget::closeEvent(QCloseEvent *event)
{
    AnalysisWindowHandler::getInstance()->destroyWidget();
}

