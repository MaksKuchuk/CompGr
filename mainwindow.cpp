#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("DSP-Kuchuk");
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
        "All files (*.*);; Text files (*.txt);; DAT files (*.dat);; Sound files (*.wav);; TSF files (*.tsf);"
        );

    ui->statusbar->showMessage(str);
}

