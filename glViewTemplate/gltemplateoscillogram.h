#ifndef GLTEMPLATEOSCILLOGRAM_H
#define GLTEMPLATEOSCILLOGRAM_H

#include <QWidget>
#include "../GraphGlData/Graph2DData.hpp"
#include "../glViewType/glOscillogram.hpp"

namespace Ui {
class glTemplateOscillogram;
}

class glTemplateOscillogram : public QWidget
{
    Q_OBJECT
    Graph2DData *data;
    long long ind;
    glOscillogram* gView = nullptr;

public:
    explicit glTemplateOscillogram(QWidget *parent = nullptr,
                                   Graph2DData *data = nullptr);
    ~glTemplateOscillogram();

    void drawMenu(QPoint globalPos);

    void setLocalScale();

    void setGlobalScale();

    void selectScale();

    void selectBias();

    void setGlobalBias();

    void selectSingleLocalScale();

    void selectSingleGlobalScale();

    void setDefaultScale();

    void setDefaultBias();

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void paintEvent(QPaintEvent *event) override;

private:
    Ui::glTemplateOscillogram *ui;

    friend class AnalysisWindowHandler;
};

#endif // GLTEMPLATEOSCILLOGRAM_H
