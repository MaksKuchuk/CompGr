#ifndef GLTEMPLATEOSCILLOGRAM_H
#define GLTEMPLATEOSCILLOGRAM_H

#include <QWidget>
#include "../GraphGlData/Graph2DData.hpp"
#include "../glViewType/glOscillogram.hpp"
#include "../graphtemplate.h"

namespace Ui {
class glTemplateOscillogram;
}

class glTemplateOscillogram : public QWidget
{
    Q_OBJECT
    std::shared_ptr<Graph2DData> data;

public:
    glOscillogram* gView = nullptr;

    QPointer<GraphTemplate> templ = nullptr;

    explicit glTemplateOscillogram(QWidget *parent = nullptr,
                                   std::shared_ptr<Graph2DData> data = nullptr,
                                   QPointer<GraphTemplate> templ_ = nullptr);
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

    void mouseReleaseEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    void paintEvent(QPaintEvent *event) override;

private:
    Ui::glTemplateOscillogram *ui;

    friend class AnalysisWindowHandler;
};

#endif // GLTEMPLATEOSCILLOGRAM_H
