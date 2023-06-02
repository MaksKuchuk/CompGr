#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "graphwidget.h"
#include "aboutwidget.h"
#include "graphinfo.h"
#include "Utility/config.h"
#include "modellingwidget.h"
#include "analyzewidget.h"

#include "Parser/Parser.hpp"
#include "Parser/ParseData.hpp"

#include "Saver/Saver.hpp"
#include "Saver/saverwindow.h"

#include "Utility/generaldialog.h"

#include "Modeling/superpositiondialog.h"

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
        "All files (*.*);; Text files (*.txt);; Sound files (*.wav);; CSV (*.csv);"
        );
    // DAT files (*.dat);; TSF files (*.tsf);;

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
//    grWindow->setFixedSize(300, grWindow->sizeHint().height()); //105 * data->getAmountOfChannels()

    widget->setWindowTitle("Channels");
    widget->show();
}

void MainWindow::on_actionSave_file_triggered() {
    if (grWid == nullptr || grWid->graphData == nullptr) return;

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
    if (AnalyzeWidget::instance != nullptr || grWid == nullptr) return;

    openAnalysisWindow();

    auto instanceAn = AnalyzeWidget::getInstance();

    if (grWid != nullptr) {
        for (size_t i = 0; i < grWid->graphData->getAmountOfChannels(); ++i) {
            instanceAn->analyze(grWid->graphData->channelTo2D(i),
                    static_cast<GraphTemplate*>(MainWindow::grWid->layout()->itemAt(i)->widget()));
        }
    }
}

void MainWindow::openAnalysisWindow() {
    if (AnalyzeWidget::instance != nullptr) return;

    AnalyzeWidget *instanceAn = AnalyzeWidget::getInstance();

    instance->ui->mdiArea->addSubWindow(instanceAn);
    instanceAn->setWindowTitle("Analyze");

    instance->ui->mdiArea->subWindowList().last()->resize(600, 450);

    instanceAn->show();
}

void MainWindow::on_actionInformation_triggered() {

    if (grWid == nullptr) return;

    QWidget *widget = new GraphInfo(ui->mdiArea, grWid->graphData);

    ui->mdiArea->addSubWindow(widget);
//    ui->mdiArea->subWindowList().last()->setFixedSize(400, 150 + 25 * grWi->graphData->getAmountOfChannels());

    widget->setWindowTitle("Graph information");
    widget->show();
}


void MainWindow::on_actionDarkTheme_triggered()
{
    Config::isDarkTheme = ui->actionDarkTheme->isChecked();
}

void MainWindow::on_actionWaveletFill_triggered()
{
    Config::waveletFill = ui->actionWaveletFill->isChecked();
}

void MainWindow::on_actionCreate_new_model_triggered() {

    auto type = GeneralDialog::ButtonDialog("Choose model", Modeling::TypesList());
    if (type < 0)
        return;

    if (modelWid != nullptr) {
        modelWindow->close();
    }

    modelWid = new ModellingWidget(ui->mdiArea, grWid == nullptr ? nullptr : grWid->graphData, (Modeling::Type)type);

    ui->mdiArea->addSubWindow(modelWid);

    modelWindow = ui->mdiArea->subWindowList().last();
    modelWindow->resize(modelWindow->sizeHint());
//    wid->resize(wid->sizeHint().width(), 500);

    modelWid->setWindowTitle("Modelling");
    modelWid->show();
}

void MainWindow::on_actionCreate_superposition_triggered() {
    if (grWid == nullptr || grWid->graphData == nullptr) {
        GeneralDialog::InfoDialog("No graphs are opened");
        return;
    }

    auto type = GeneralDialog::ButtonDialog("Choose type of Superposition", {"Linear", "Multiplicative"});

    switch (type) {
    case 0:
        SuperpositionDialog::LinSupDialog(grWid->graphData);
        break;
    case 1:
        SuperpositionDialog::MultSupDialog(grWid->graphData);
        break;
    default:
        break;
    }
}

void MainWindow::AddWidget(QWidget* qwid) {
    MainWindow::instance->ui->mdiArea->addSubWindow(qwid);
    qwid->show();
}
