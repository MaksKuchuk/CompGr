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

    void updateGraph();
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void drawGraph();

private:

};

#endif // GLSPECTROGRAM_H
