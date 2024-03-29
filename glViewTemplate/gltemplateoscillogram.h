#ifndef GLTEMPLATEOSCILLOGRAM_H
#define GLTEMPLATEOSCILLOGRAM_H

#include <QWidget>
#include <QScrollBar>
#include <QLabel>
#include <QPointer>
#include <QString>
#include "../GraphGlData/Graph2DData.hpp"
#include "../glViewType/glOscillogram.hpp"
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

    glType type = glType::Oscillogram;

    bool xLogScale = false;
    bool yLogScale = false;
    bool isTimeStep = false;

    explicit glTemplateOscillogram(QWidget *parent = nullptr,
                                   std::shared_ptr<Graph2DData> data = nullptr,
                                   glType oscType = glType::Oscillogram);
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
    void ResetBias();
    void ResetScale();

signals:
    void BiasChanged(long long lcur, long long rcur);
    void ScaleChanged(double top, double bottom);

protected:

    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    void paintEvent(QPaintEvent *event) override;


    void wheelEvent(QWheelEvent *event) override;

    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent *event) override;


    double scrollF(long long x);


    void ChangeInfoLabel();

    void ChangeScrollBar();
    void ScrollBarChanged();

    QPointer<QScrollBar> scrollBar;
    QPointer<QLabel> infoLabel;
private:

    Ui::glTemplateOscillogram *ui;

    friend class QWidget;

    void closeEvent(QCloseEvent *event) override;
};

#endif // GLTEMPLATEOSCILLOGRAM_H
