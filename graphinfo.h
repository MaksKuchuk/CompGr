#ifndef GRAPHINFO_H
#define GRAPHINFO_H

#include <QWidget>
#include "GraphGlData/generaldata.h"

namespace Ui {
class GraphInfo;
}

class GraphInfo : public QWidget
{
    Q_OBJECT

public:
    explicit GraphInfo(QWidget *parent = nullptr, std::shared_ptr<GeneralData> pData = nullptr);
    ~GraphInfo();

private:
    Ui::GraphInfo *ui;
};

#endif // GRAPHINFO_H
