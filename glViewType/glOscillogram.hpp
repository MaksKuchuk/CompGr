#ifndef GLOSCILLOGRAM_H
#define GLOSCILLOGRAM_H

#include <QOpenGLWidget>
#include "../GraphGlData/Graph2DData.hpp"

class glOscillogram : public QOpenGLWidget
{
    Q_OBJECT
    Graph2DData* data;

public:
    glOscillogram(QWidget *parent = nullptr, Graph2DData *data = nullptr);

protected:
    void drawGraph();

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};

#endif
