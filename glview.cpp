#include "glview.h"

glView::glView(QWidget *parent, long long parNum, double* data, double maxVal, double minVal) :
    QOpenGLWidget(parent),
    parNum(parNum),
    data(data),
    maxVal(maxVal),
    minVal(minVal)
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
    double dotsNumber = parNum > 15000 ? 15000 : parNum;
    double diff = maxVal - minVal;
    glBegin(GL_LINE_STRIP);
        for (long long i = 0; i < dotsNumber; i++) {
            double x = (2 * (double)(i) / (dotsNumber - 1)) - 1;
            double y = (2 * (data[static_cast<long long>(i * (parNum / dotsNumber))] - minVal) / diff) - 1;
            glVertex2d(x, y);
        }
    glEnd();
}





