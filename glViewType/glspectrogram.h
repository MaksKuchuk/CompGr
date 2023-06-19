#ifndef GLSPECTROGRAM_H
#define GLSPECTROGRAM_H

#include <QOpenGLWidget>
#include "../GraphGlData/Graph3DData.hpp"

class glSpectrogram : public QOpenGLWidget
{
    Q_OBJECT

public:
    std::shared_ptr<Graph3DData> data;

    glSpectrogram(QWidget *parent = nullptr, std::shared_ptr<Graph3DData> data = nullptr);

    static void colorSchemeGrey(float percent);
    static void colorSchemeHeat(float percent);
    static void colorSchemeDarkHeat(float percent);

    void updateGraph();
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void drawGraph();

private:

    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent* event) override;

    double crossX = -1, crossY = -1;

};

#endif // GLSPECTROGRAM_H
