#include "glOscillogram.hpp"
#include "../mainwindow.h"
#include "../Handler/AnalysisWindowHandler.hpp"
#include <iostream>

glOscillogram::glOscillogram(QWidget *parent, std::shared_ptr<Graph2DData> data) :
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
    if (MainWindow::isDarkTheme) {
        glClearColor(0, 0, 0, 1);
        glColor3f(1, 1, 1);
    } else {
        glClearColor(1, 1, 1, 1);
        glColor3f(0, 0, 0);
    }
    drawGraph();
}

void glOscillogram::drawGraph() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLineWidth(1);

    double x, y;
    long long lcur = data->lcur;
    long long rcur = data->rcur;

    double parNum = rcur - lcur + 1;

    double dotsNumber = parNum > 2000 ? 2000 : parNum;
    //double dotsNumber = parNum;
    double diff = data->maxLoc - data->minLoc;

    glBegin(GL_LINE_STRIP);
        for (long long i = 0; i < dotsNumber; i++) {
            x = (2 * (double)(i) / (dotsNumber - 1)) - 1;
            y = (2 * (data->samples[static_cast<long long>(lcur + i * (parNum / dotsNumber))] - data->minLoc) / diff) - 1;
            glVertex2d(x, y);
        }
    glEnd();

    if (AnalysisWindowHandler::xpress == -1 ||
        AnalysisWindowHandler::ypress == -1 ||
        AnalysisWindowHandler::xrelease == -1 ||
        AnalysisWindowHandler::yrelease == -1) return;

//    std::cout << AnalysisWindowHandler::xleft << ' ' << AnalysisWindowHandler::xright
//              << ' ' << AnalysisWindowHandler::ybottom << ' ' << AnalysisWindowHandler::ytop << std::endl;

    glColor3f(0, 0, 1);
    glLineWidth(1);
    glBegin(GL_LINES);
        glVertex2d(((AnalysisWindowHandler::xleft * 2) - 1), -1);
        glVertex2d(((AnalysisWindowHandler::xleft * 2) - 1), 1);
        glVertex2d(((AnalysisWindowHandler::xright * 2) - 1), -1);
        glVertex2d(((AnalysisWindowHandler::xright * 2) - 1), 1);
    glEnd();
}















