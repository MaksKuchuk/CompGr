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

    void analyze2DBy(std::shared_ptr<Graph2DData> data, QPointer<GraphTemplate> templ, glType t = glType::Oscillogram);
    void analyze3DBy(std::shared_ptr<Graph2DData> data);


    static inline bool scaleMod;

    static inline int xpress;
    static inline int ypress;
    static inline int xrelease;
    static inline int yrelease;

    static inline double xleft;
    static inline double xright;
    static inline double ybottom;
    static inline double ytop;

private:
    Ui::AnalyzeWidget *ui;

    friend class AnalysisWindowHandler;
};

#endif // ANALYZEWIDGET_H
