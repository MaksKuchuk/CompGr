#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "graphwidget.h"
#include "aboutwidget.h"
#include "graphinfo.h"
#include "glview.h"
#include "graphtemplate.h"
#include "modellingwidget.h"
#include "Handler/AnalysisWindowHandler.hpp"

#include "Parser/Parser.hpp"
#include "Parser/ParseData.hpp"

#include "Saver/Saver.hpp"
#include "Saver/saverwindow.h"

#include "Utility/generaldialog.h"

#include <QDebug>

#include <QMdiSubWindow>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("DSP-Kuchuk");

    setCentralWidget(ui->mdiArea);
    instance = this;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_file_triggered()
{
    QString str;
    str = QFileDialog::getOpenFileName(
        this,
        "Select file",
        "",
        "Text files (*.txt);; DAT files (*.dat);; Sound files (*.wav);; TSF files (*.tsf);; All files (*.*);"
        );

    if (str == nullptr || str == "") return;

    if (grapthData != nullptr) {
        auto agree = GeneralDialog::AgreeDialog("Close old graph and open new?");
        if (!agree)
            return;
        for (auto subWin : ui->mdiArea->subWindowList())
            subWin->close();
    }

    ParseData *pData = Parser::parse(str);

    grWid = new GraphWidget(ui->mdiArea, pData);
    grapthData = pData;

    QWidget *widget = grWid;

    ui->mdiArea->addSubWindow(widget);

    ui->mdiArea->subWindowList().last()->setFixedSize(300, 105 * pData->getAmountOfChannels());

    widget->setWindowTitle("Channels");
    widget->show();
}

void MainWindow::on_actionSave_file_triggered() {
    QMdiSubWindow *activeWidget = ui->mdiArea->currentSubWindow(); //ui->mdiArea->activeSubWindow();

    if (activeWidget == nullptr) return;

    GraphWidget* grWi = static_cast<GraphWidget*>(activeWidget->widget());

    if (grWi->nm != "GraphWidget" || grWi->pData == nullptr) return;

    auto temp = static_cast<GraphTemplate*>(grWi->layout()->itemAt(0)->widget());
    if (temp == nullptr) return;
    auto view = static_cast<glView*>(temp->layout()->itemAt(0)->widget());

    QString path = QFileDialog::getSaveFileName(
                this,
                "Save File",
                "",
                "Text files (*.txt);; All files (*.*)");
//    qDebug() << path;

    if (path == nullptr || path == "") return;

    auto data = SaverWindow::openWindow(grWi->pData, view->getLCur(), view->getRCur());
    Saver::TxtSaver(grWi->pData, data.first_sample, data.last_sample, data.channels, path);
}

void MainWindow::on_actionAbout_triggered()
{
    QWidget *widget = new AboutWidget(ui->mdiArea);
    ui->mdiArea->addSubWindow(widget);
    ui->mdiArea->subWindowList().last()->setFixedSize(350, 110);

    widget->setWindowTitle("About");
    widget->show();
}

void MainWindow::on_actionAnalysis_triggered() {
    if (!AnalysisWindowHandler::getInstance()->isNullAnalyzeWidget()) return;

    QMdiSubWindow *activeWidget = ui->mdiArea->activeSubWindow();

    openAnalysisWindow();

    AnalysisWindowHandler *instanceAn = AnalysisWindowHandler::getInstance();

    if (activeWidget != nullptr) {
        GraphWidget* grWi = static_cast<GraphWidget*>(activeWidget->widget());

        if (grWi->nm == "GraphWidget") {
            instanceAn->addWidget(grWi->pData);
        }
    }
}

void MainWindow::openAnalysisWindow() {
    if (!AnalysisWindowHandler::getInstance()->isNullAnalyzeWidget()) return;

    AnalysisWindowHandler *instanceAn = AnalysisWindowHandler::getInstance();
    AnalyzeWidget* w = instanceAn->getAnalyzeWidget();

    instance->ui->mdiArea->addSubWindow(w);
    instanceAn->getAnalyzeWidget()->setWindowTitle("Analyze");

    instance->ui->mdiArea->subWindowList().last()->resize(600, 450);

    instanceAn->getAnalyzeWidget()->show();
}

void MainWindow::on_actionInformation_triggered() {

    QMdiSubWindow *activeWidget = ui->mdiArea->activeSubWindow();

    if (activeWidget == nullptr) return;

    GraphWidget* grWi = static_cast<GraphWidget*>(activeWidget->widget());

    if (grWi->nm != "GraphWidget") return;

    QWidget *widget = new GraphInfo(ui->mdiArea, grWi->pData);

    ui->mdiArea->addSubWindow(widget);
    ui->mdiArea->subWindowList().last()->setFixedSize(400, 150 + 25 * grWi->pData->getAmountOfChannels());

    widget->setWindowTitle("Graph information");
    widget->show();
}


void MainWindow::on_actionTheme_triggered()
{
    isDarkTheme = !isDarkTheme;
}

void MainWindow::on_actionCreate_new_model_triggered() {
    qDebug() << "HEY!";

    QWidget *widget = new ModellingWidget(ui->mdiArea);

    ui->mdiArea->addSubWindow(widget);

    ui->mdiArea->subWindowList().last()->resize(300, 105);

    widget->setWindowTitle("Modelling");
    widget->show();
}

