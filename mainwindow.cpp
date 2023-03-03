#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "graphwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("DSP-Kuchuk");

    setCentralWidget(ui->mdiArea);

    on_actionNew_file_triggered();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_file_triggered()
{
//    QString str;
//    str = QFileDialog::getOpenFileName(
//        this,
//        "Select file",
//        "",
//        "Text files (*.txt);; DAT files (*.dat);; Sound files (*.wav);; TSF files (*.tsf);; All files (*.*);"
//        );


    QWidget *widget = new GraphWidget(ui->mdiArea);

    ui->mdiArea->addSubWindow(widget);
    widget->setWindowTitle("Sub Window");
    widget->show();
}

