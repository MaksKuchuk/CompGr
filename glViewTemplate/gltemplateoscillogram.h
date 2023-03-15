#ifndef GLTEMPLATEOSCILLOGRAM_H
#define GLTEMPLATEOSCILLOGRAM_H

#include <QWidget>
#include "../GraphGlData/Graph2DData.hpp"

namespace Ui {
class glTemplateOscillogram;
}

class glTemplateOscillogram : public QWidget
{
    Q_OBJECT
    Graph2DData *data;

public:
    explicit glTemplateOscillogram(QWidget *parent = nullptr, Graph2DData *data = nullptr);
    ~glTemplateOscillogram();

private:
    Ui::glTemplateOscillogram *ui;
};

#endif // GLTEMPLATEOSCILLOGRAM_H
