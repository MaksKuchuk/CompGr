#ifndef GRAPHTEMPLATE_H
#define GRAPHTEMPLATE_H

#include <QWidget>
#include "GraphGlData/generaldata.h"
#include "GraphGlData/Graph2DData.hpp"
#include "glview.h"

namespace Ui {
class GraphTemplate;
}

class GraphTemplate : public QWidget
{
    Q_OBJECT
    const std::shared_ptr<Graph2DData> data = nullptr;

    void drawMenu(QPoint globalPos);

public:
    QPointer<glView> gView = nullptr;

    explicit GraphTemplate(QWidget *parent = nullptr, const std::shared_ptr<GeneralData> gData = nullptr, long long ind = -1);
    explicit GraphTemplate(QWidget *parent = nullptr, const std::shared_ptr<Graph2DData> gData = nullptr, bool showName = true);
    ~GraphTemplate();

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::GraphTemplate *ui;
};

#endif // GRAPHTEMPLATE_H
