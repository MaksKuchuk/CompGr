#ifndef GRAPHINFO_H
#define GRAPHINFO_H

#include <QWidget>
#include "Parser/ParseData.hpp"

namespace Ui {
class GraphInfo;
}

class GraphInfo : public QWidget
{
    Q_OBJECT

public:
    explicit GraphInfo(QWidget *parent = nullptr, ParseData *pData = nullptr);
    ~GraphInfo();

private:
    Ui::GraphInfo *ui;
};

#endif // GRAPHINFO_H
