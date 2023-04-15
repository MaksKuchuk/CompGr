#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMdiArea>

#include "graphwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static inline MainWindow* instance = nullptr;

    static inline void openAnalysisWindow();

    static inline bool isDarkTheme = true;

    static inline GraphWidget* grWid = nullptr;

    static inline ParseData* grapthData = nullptr;

private slots:
    void on_actionNew_file_triggered();

    void on_actionSave_file_triggered();

    void on_actionAbout_triggered();

    void on_actionAnalysis_triggered();

    void on_actionInformation_triggered();

    void on_actionTheme_triggered();

    void on_actionCreate_new_model_triggered();

protected:

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
