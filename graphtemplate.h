#ifndef GRAPHTEMPLATE_H
#define GRAPHTEMPLATE_H

#include <QWidget>
#include "GraphGlData/generaldata.h"
#include "GraphGlData/Graph2DData.hpp"
#include "Statistics/statisticswidget.h"
#include "glview.h"

namespace Ui {
class GraphTemplate;
}

class GraphTemplate : public QWidget
{
    Q_OBJECT

    void drawMenu(QPoint globalPos);

    const std::shared_ptr<Graph2DData> data = nullptr;
public:

    QPointer<glView> gView = nullptr;
    QPointer<StatisticsWidget> statsWid;

    explicit GraphTemplate(QWidget *parent = nullptr, const std::shared_ptr<GeneralData> gData = nullptr, long long ind = -1);
    explicit GraphTemplate(QWidget *parent = nullptr, const std::shared_ptr<Graph2DData> gData = nullptr, bool showName = true);
    ~GraphTemplate();

    void resetCurs();

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::GraphTemplate *ui;
};

#endif // GRAPHTEMPLATE_H
