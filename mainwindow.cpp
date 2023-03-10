#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "graphwidget.h"
#include "aboutwidget.h"
#include "graphinfo.h"

#include "Parser/Parser.hpp"
#include "Parser/ParseData.hpp"

#include <QMdiSubWindow>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("DSP-Kuchuk");

    setCentralWidget(ui->mdiArea);
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

    ParseData *pData = Parser::parse(str.toStdString());

    QWidget *widget = new GraphWidget(ui->mdiArea, pData);
    //QWidget *widget = new GraphWidget(ui->mdiArea, nullptr);

    ui->mdiArea->addSubWindow(widget);

    ui->mdiArea->subWindowList().last()->setFixedSize(300, 105 * pData->getAmountOfChannels());

    widget->setWindowTitle("Channels");
    widget->show();
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
    ui->mdiArea->activateNextSubWindow();
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

