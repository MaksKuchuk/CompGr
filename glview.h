#ifndef GLVIEW_H
#define GLVIEW_H

#include <QOpenGLWidget>

class glView : public QOpenGLWidget
{
    Q_OBJECT

public:
    glView(QWidget *parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};

#endif // GLVIEW_H
