#ifndef ANALYZEWIDGET_H
#define ANALYZEWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QWheelEvent>
#include <QKeyEvent>

#include "GraphGlData/Graph2DData.hpp"
#include "graphtemplate.h"
#include "glViewType/glType.hpp"


namespace Ui {
class AnalyzeWidget;
}

class AnalyzeWidget : public QWidget
{
    Q_OBJECT

public:
    static inline QPointer<AnalyzeWidget> instance = nullptr;
    static QPointer<AnalyzeWidget> getInstance();

    QVBoxLayout *layout;

    explicit AnalyzeWidget(QWidget *parent = nullptr);
    ~AnalyzeWidget();

    void closeEvent(QCloseEvent *event) override;

    void analyze(std::shared_ptr<Graph2DData> data, QPointer<GraphTemplate> templ, glType t = glType::Oscillogram);


    static inline bool isMultipleBiasStarted = false;

    static inline bool scaleMod;

    static inline int xpress;
    static inline int ypress;
    static inline int xrelease;
    static inline int yrelease;

    static inline double xleft;
    static inline double xright;
    static inline double ybottom;
    static inline double ytop;

public slots:
    void multipleBiasStart(qint64 l, qint64 r);

signals:
    void multipleBiasStartSignal(qint64 l, qint64 r);

private:
    Ui::AnalyzeWidget *ui;

private slots:

    void on_actionSimultaneous_moving_triggered();
    void on_actionLog_Scale_X_triggered();
    void on_actionLog_Scale_Y_triggered();
};

#endif // ANALYZEWIDGET_H
