#ifndef GRAPHTEMPLATE_H
#define GRAPHTEMPLATE_H

#include <QWidget>
#include "GraphGlData/generaldata.h"

namespace Ui {
class GraphTemplate;
}

class GraphTemplate : public QWidget
{
    Q_OBJECT
    std::shared_ptr<GeneralData> pData;
    long long ind;

    void drawMenu(QPoint globalPos);

public:
    explicit GraphTemplate(QWidget *parent = nullptr, std::shared_ptr<GeneralData> pData = nullptr, long long ind = -1);
    ~GraphTemplate();

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::GraphTemplate *ui;
};

#endif // GRAPHTEMPLATE_H
