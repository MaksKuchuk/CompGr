#include "glview.h"
#include "mainwindow.h"
#include "Utility/config.h"
#include <iostream>

glView::glView(QWidget *parent, long long parNum, const QList<double>& data, double maxVal, double minVal) :
    QOpenGLWidget(parent),
    parNum(parNum),
    data(data),
    maxVal(maxVal),
    minVal(minVal),
    lcur(0),
    rcur(parNum - 1)
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
    if (Config::isDarkTheme) {
        glClearColor(0, 0, 0, 1);
        glColor3f(1, 1, 1);
    } else {
        glClearColor(1, 1, 1, 1);
        glColor3f(0, 0, 0);
    }
    glClear(GL_COLOR_BUFFER_BIT);

    glLineWidth(1);
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

    if (lcur <= 0 && rcur >= parNum - 1) return;
    glColor3f(1, 0, 0);
    glLineWidth(1);

    glBegin(GL_LINES);
        glVertex2f(((double)lcur / parNum) * 2 - 1, -1);
        glVertex2f(((double)lcur / parNum) * 2 - 1, 1);
        glVertex2f(((double)(rcur+2) / parNum) * 2 - 1, -1);
        glVertex2f(((double)(rcur+2) / parNum) * 2 - 1, 1);
    glEnd();
}

void glView::setCurs(long long lcur, long long rcur) {
    this->lcur = lcur;
    this->rcur = rcur;
    emit cursChanged(lcur, rcur);
    update();
}




