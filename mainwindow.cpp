#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "graphwidget.h"
#include "aboutwidget.h"

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

//    on_actionNew_file_triggered();
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

    ui->mdiArea->addSubWindow(widget);

    ui->mdiArea->subWindowList().last()->setFixedSize(200, 100 * pData->getAmountOfChannels());

    widget->setWindowTitle("Sub Window");
    widget->show();
}


void MainWindow::on_actionAbout_triggered()
{
    QWidget *widget = new AboutWidget(ui->mdiArea);
    ui->mdiArea->addSubWindow(widget);
    ui->mdiArea->subWindowList().last()->setFixedSize(350, 100);

    widget->setWindowTitle("About");
    widget->show();
}

