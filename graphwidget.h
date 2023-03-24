#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QWidget>
#include "Parser/ParseData.hpp"

namespace Ui {
class GraphWidget;
}

class GraphWidget : public QWidget
{
    Q_OBJECT

public:
    std::string nm = "GraphWidget";

    explicit GraphWidget(QWidget *parent = nullptr, ParseData* pData = nullptr);
    ~GraphWidget();

    ParseData *pData = nullptr;


private:
    Ui::GraphWidget *ui;

};

#endif // GRAPHWIDGET_H
