#ifndef GLOSCILLOGRAM_H
#define GLOSCILLOGRAM_H

#include <QOpenGLWidget>
#include "../GraphGlData/Graph2DData.hpp"

class glOscillogram : public QOpenGLWidget
{
    Q_OBJECT

public:
    std::shared_ptr<Graph2DData> data;

    glOscillogram(QWidget *parent = nullptr, std::shared_ptr<Graph2DData> data = nullptr);

    void updateGraph();
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void drawGraph();
};

#endif
