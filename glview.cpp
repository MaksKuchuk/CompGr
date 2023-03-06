#include "glview.h"

glView::glView(QWidget *parent, long long parNum, double* data, double maxMod) :
    QOpenGLWidget(parent),
    parNum(parNum),
    data(data),
    maxMod(maxMod)
{ }

void glView::initializeGL() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);
}

void glView::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void glView::paintGL() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glLineWidth(1);
    glColor3f(1, 1, 1);
    drawGraph();
}

void glView::drawGraph() {
    glBegin(GL_LINE_STRIP);
        for (long long i = 0; i < parNum; i++) {
            double x = (2 * (double)(i) / (parNum - 1)) - 1;
            double y = (data[i] / maxMod);
            glVertex2d(x, y);
        }
    glEnd();
}
