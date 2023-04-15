#ifndef MODELLINGWIDGET_H
#define MODELLINGWIDGET_H

#include <QWidget>
#include "GraphGlData/Graph2DData.hpp"

namespace Ui {
class GraphWidget;
}

class ModellingWidget : public QWidget
{
    Q_OBJECT
public:
    std::string nm = "GraphWidget";

    Graph2DData* data = nullptr;

    ModellingWidget(QWidget* parent);
    ~ModellingWidget() = default;

    static void test();

private:
    Ui::GraphWidget *ui;
};

#endif // MODELLINGWIDGET_H
