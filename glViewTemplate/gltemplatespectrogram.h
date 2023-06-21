#ifndef GLTEMPLATESPECTROGRAM_H
#define GLTEMPLATESPECTROGRAM_H

#include <QObject>
#include <QWidget>
#include <QSlider>
#include <QPointer>
#include <QScrollBar>
#include <QLabel>

#include "../GraphGlData/Graph3DData.hpp"
#include "../glViewType/glspectrogram.h"

namespace Ui {
class glTemplateOscillogram;
}

class glTemplateSpectrogram : public QWidget
{
    Q_OBJECT

public:
    std::shared_ptr<Graph3DData> data;
    QPointer<glSpectrogram> gSpec;

    bool xLogScale = false;
    bool yLogScale = false;
    double brightness = 1.0;

    explicit glTemplateSpectrogram(QWidget *parent = nullptr,
                          std::shared_ptr<Graph3DData> data = nullptr);

    ~glTemplateSpectrogram();

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

    void ChangeScrollBar();
    void ScrollBarChanged();


private:
    QPointer<QScrollBar> scrollBar;
    QPointer<QLabel> infoLabel;
    QPointer<QSlider> slider;

    friend class QWidget;
    Ui::glTemplateOscillogram *ui;

    void closeEvent(QCloseEvent *event) override;

    void ChangeInfoLabel();

};

#endif // GLTEMPLATESPECTROGRAM_H
