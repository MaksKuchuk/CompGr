#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QWidget>
#include <QCloseEvent>
#include <memory>
#include "GraphGlData/generaldata.h"

namespace Ui {
class GraphWidget;
}

class GraphWidget : public QWidget
{
    Q_OBJECT

public:
    std::string nm = "GraphWidget";

    explicit GraphWidget(QWidget *parent = nullptr, std::shared_ptr<GeneralData> pData = nullptr);
    ~GraphWidget();

    std::shared_ptr<GeneralData> graphData;

    void AddNewChannel(std::shared_ptr<Graph2DData> newData);
    void AddNewChannel(std::shared_ptr<Graph2DData> newData, QString name);

    void closeEvent(QCloseEvent *event);

    void setCurs(long long lcur, long long rcur);

private:
    Ui::GraphWidget *ui;

};

#endif // GRAPHWIDGET_H
