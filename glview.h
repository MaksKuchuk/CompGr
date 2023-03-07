#ifndef GLVIEW_H
#define GLVIEW_H

#include <QOpenGLWidget>

class glView : public QOpenGLWidget
{
    Q_OBJECT
    double* data;
    long long parNum;
    double maxVal;
    double minVal;

public:
    glView(QWidget *parent = nullptr,
           long long parNum = 0,
           double* data = nullptr,
           double maxVal = 0,
           double minVal = 0
            );

protected:
    void drawGraph();

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};

#endif // GLVIEW_H
