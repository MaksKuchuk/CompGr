#include "glOscillogram.hpp"

glOscillogram::glOscillogram(QWidget *parent, Graph2DData *data) :
    QOpenGLWidget(parent),
    data(data)
{ }

void glOscillogram::initializeGL() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);
}

void glOscillogram::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    drawGraph();
}

void glOscillogram::paintGL() {
    glClearColor(0, 0, 0, 1);
    drawGraph();
}

void glOscillogram::drawGraph() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLineWidth(1);
    glColor3f(1, 1, 1);

    double x, y;

}





