#include "glOscillogram.hpp"

glOscillogram::glOscillogram(QWidget *parent, Graph2DData *data) :
    QOpenGLWidget(parent),
    data(data) {

}

void glOscillogram::updateGraph() {
    repaint();
}

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
    long long lcur = data->lcur;
    long long rcur = data->rcur;

    double parNum = rcur - lcur + 1;

    double dotsNumber = parNum > 2000 ? 2000 : parNum;
    double diff = data->maxVal - data->minVal;

    glBegin(GL_LINE_STRIP);
        for (long long i = 0; i < dotsNumber; i++) {
            x = (2 * (double)(i) / (dotsNumber - 1)) - 1;
            y = (2 * (data->samples[static_cast<long long>(lcur + i * (parNum / dotsNumber))] - data->minVal) / diff) - 1;
            glVertex2d(x, y);
        }
    glEnd();
}















