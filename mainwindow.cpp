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

    if (grWid != nullptr && grWid->graphData != nullptr) {
        auto agree = GeneralDialog::AgreeDialog("Close old graph and open new?");
        if (!agree)
            return;
        for (auto subWin : ui->mdiArea->subWindowList())
            subWin->close();
    }

    auto pData = static_cast<std::shared_ptr<GeneralData>>(Parser::parse(str));

    ShowGraphWidget(pData);
}

void MainWindow::ShowGraphWidget(std::shared_ptr<GeneralData> data) {
    if (grWid != nullptr){
        grWid->close();
        grWindow->close();
    }

    grWid = new GraphWidget(ui->mdiArea, data);

    QWidget *widget = grWid;

    ui->mdiArea->addSubWindow(widget);

    grWindow = ui->mdiArea->subWindowList().last();
    grWindow->setFixedSize(300, grWindow->sizeHint().height()); //105 * data->getAmountOfChannels()

    widget->setWindowTitle("Channels");
    widget->show();
}

void MainWindow::on_actionSave_file_triggered() {
    if (grWid->graphData == nullptr) return;

    auto data = SaverWindow::openWindow(grWid->graphData, grWid->graphData->lcur, grWid->graphData->rcur);

    if (!data.success)
        return;

    QString path = QFileDialog::getSaveFileName(
                this,
                "Save File",
                "",
                "Text files (*.txt);; All files (*.*)");
    if (path == nullptr || path == "") return;

    if (data.success)
        Saver::TxtSaver(grWid->graphData, data.first_sample, data.last_sample, data.channels, path);
}

void MainWindow::on_actionAbout_triggered()
{
    QWidget *widget = new AboutWidget(ui->mdiArea);
    ui->mdiArea->addSubWindow(widget);
//    ui->mdiArea->subWindowList().last()->setFixedSize(350, 110);

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
            instanceAn->addWidget(grWi->graphData);
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

    QWidget *widget = new GraphInfo(ui->mdiArea, grWi->graphData);

    ui->mdiArea->addSubWindow(widget);
//    ui->mdiArea->subWindowList().last()->setFixedSize(400, 150 + 25 * grWi->graphData->getAmountOfChannels());

    widget->setWindowTitle("Graph information");
    widget->show();
}


void MainWindow::on_actionTheme_triggered()
{
    isDarkTheme = !isDarkTheme;
}

void MainWindow::on_actionCreate_new_model_triggered() {

    modelWid = new ModellingWidget(ui->mdiArea, grWid == nullptr ? nullptr : grWid->graphData);

    ui->mdiArea->addSubWindow(modelWid);

    modelWindow = ui->mdiArea->subWindowList().last();
    modelWindow->resize(modelWindow->sizeHint());
//    wid->resize(wid->sizeHint().width(), 500);

    modelWid->setWindowTitle("Modelling");
    modelWid->show();
}

