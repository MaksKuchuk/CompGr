#ifndef GLTEMPLATEOSCILLOGRAM_H
#define GLTEMPLATEOSCILLOGRAM_H

#include <QWidget>
#include "../GraphGlData/Graph2DData.hpp"
#include "../glViewType/glOscillogram.hpp"
#include "../graphtemplate.h"
#include "../glViewType/glType.hpp"


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
    QPointer<QLabel> infoLabel;
    glType type = glType::Oscillogram;

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

    void scrollGraph(long long delta_y);
    void moveGraph(long long y);


public slots:
    void SetBias(long long lcur, long long rcur);
    void SetScale(double bottom, double top);

signals:
    void BiasChanged(long long lcur, long long rcur);
    void ScaleChanged(double top, double bottom);

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    void paintEvent(QPaintEvent *event) override;


    void wheelEvent(QWheelEvent *event) override;

    bool eventFilter(QObject *obj, QEvent *event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent *event) override;


    double scrollF(long long x);


    void ChangeInfoLabel();

private:
    Ui::glTemplateOscillogram *ui;

    friend class AnalysisWindowHandler;
};

#endif // GLTEMPLATEOSCILLOGRAM_H
