#include "glview.h"

glView::glView(QWidget *parent) : QOpenGLWidget(parent) { }

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

    glLineWidth(1.0);
    glColor3f(1, 1, 1);
    glBegin(GL_LINE_STRIP);
        glVertex2f(-1, 0);
        glVertex2f(-0.9, 0.2);
        glVertex2f(-0.8, -0.4);
        glVertex2f(-0.7, 0.3);
        glVertex2f(-0.6, -0.8);
        glVertex2f(-0.5, 0.7);
        glVertex2f(-0.4, 0);
        glVertex2f(-0.3, 0.2);
        glVertex2f(-0.2, -0.4);
        glVertex2f(-0.1, 0.3);
        glVertex2f(0, -0.8);
        glVertex2f(0.1, 0.3);
        glVertex2f(0.2, 1);
        glVertex2f(0.3, -0.7);
        glVertex2f(0.4, 0.7);
        glVertex2f(0.5, 0.2);
        glVertex2f(0.6, -0.1);
        glVertex2f(0.7, 0.2);
        glVertex2f(0.8, -0.7);
        glVertex2f(0.9, -0.3);
        glVertex2f(1, 0);
    glEnd();
}
