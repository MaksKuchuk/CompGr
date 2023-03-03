#include "glview.h"

glView::glView(QWidget *parent) : QOpenGLWidget(parent) { }

void glView::initializeGL() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 800, 600, 0, 0, 1);
}

void glView::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void glView::paintGL() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3f(1, 1, 1);
    glBegin(GL_POLYGON);
        glVertex2d(0, 0);
        glVertex2d(300, 300);
        glVertex2d(0, 300);
    glEnd();
}
