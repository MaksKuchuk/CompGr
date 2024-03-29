#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <memory>

#include "graphwidget.h"
#include "modellingwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static inline QPointer<MainWindow> instance = nullptr;

    static void openAnalysisWindow();

    static inline QPointer<GraphWidget> grWid = nullptr;
    static inline QPointer<QMdiSubWindow> grWindow = nullptr;

    static inline QPointer<ModellingWidget> modelWid = nullptr;
    static inline QPointer<QMdiSubWindow> modelWindow = nullptr;

    void ShowGraphWidget(std::shared_ptr<GeneralData>);

    static void AddWidget(QWidget* qwid);

private slots:
    void on_actionNew_file_triggered();

    void on_actionSave_file_triggered();

    void on_actionAbout_triggered();

    void on_actionAnalysis_triggered();

    void on_actionInformation_triggered();

    void on_actionDarkTheme_triggered();

    void on_actionFourierFill_triggered();

    void on_actionCreate_new_model_triggered();

    void on_actionCreate_superposition_triggered();

//protected:

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
