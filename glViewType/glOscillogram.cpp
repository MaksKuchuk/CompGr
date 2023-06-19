#include "glOscillogram.hpp"
#include "../glViewTemplate/gltemplateoscillogram.h"
#include "../Utility/config.h"
#include "../Utility/utility.h"
#include "../analyzewidget.h"
#include <iostream>
#include <cmath>
#include <QToolTip>

glOscillogram::glOscillogram(QWidget *parent, std::shared_ptr<Graph2DData> data) :
    QOpenGLWidget(parent),
    data(data) {
    setMinimumSize(50, 25);

    setMouseTracking(true);
    setToolTipDuration(1e6);
    setAttribute(Qt::WA_Hover, true);
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

}

void glOscillogram::paintGL() {
    if (Config::isDarkTheme) {
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

    double dotsNumber = parNum > 50000 ? 50000 : parNum;
    //double dotsNumber = parNum;
    double diff = data->maxLoc - data->minLoc;

    auto gtemp = qobject_cast<glTemplateOscillogram*>(parent());

    auto xScaler = &Utility::LinearScale;
    if (gtemp->xLogScale)
        xScaler = &Utility::LogScale;

    auto yScaler = &Utility::LinearScale;
    if (gtemp->yLogScale)
        yScaler = &Utility::LogScale;

    glBegin(GL_LINE_STRIP);
        for (long long i = 0; i < dotsNumber; i++) {
            x = xScaler(i, dotsNumber - 1, -1, 1);
            y = yScaler(data->samples[static_cast<long long>(lcur + i * (parNum / dotsNumber))] - data->minLoc, diff, -1, 1);
            glVertex2d(x, y);
        }
    glEnd();

    glColor3f(0.2, 0.2, 0.2);
    glLineWidth(1);
    glBegin(GL_LINES);
        glVertex2d(crossX, -1);
        glVertex2d(crossX, 1);
        glVertex2d(-1, crossY);
        glVertex2d(1, crossY);
    glEnd();

    if (AnalyzeWidget::xpress == -1 ||
        AnalyzeWidget::ypress == -1 ||
        AnalyzeWidget::xrelease == -1 ||
        AnalyzeWidget::yrelease == -1) return;

//    std::cout << AnalysisWindowHandler::xleft << ' ' << AnalysisWindowHandler::xright
//              << ' ' << AnalysisWindowHandler::ybottom << ' ' << AnalysisWindowHandler::ytop << std::endl;

    glColor3f(0, 0, 1);
    glLineWidth(1);
    glBegin(GL_LINES);
        glVertex2d(((AnalyzeWidget::xleft * 2) - 1), -1);
        glVertex2d(((AnalyzeWidget::xleft * 2) - 1), 1);
        glVertex2d(((AnalyzeWidget::xright * 2) - 1), -1);
        glVertex2d(((AnalyzeWidget::xright * 2) - 1), 1);
    glEnd();
}

void glOscillogram::mouseMoveEvent(QMouseEvent *event) {
    auto gtemp = qobject_cast<glTemplateOscillogram*>(parent());
    auto xScaler = &Utility::LinearScale;
    if (gtemp->xLogScale)
        xScaler = &Utility::ExpScale;

    auto yScaler = &Utility::LinearScale;
    if (gtemp->xLogScale)
        yScaler = &Utility::ExpScale;


    if (event->buttons() & Qt::LeftButton) {
        crossX = -1;
        crossY = -1;
    } else {

        qint64 x = xScaler(event->position().x(), width(), data->lcur, data->rcur);
        QString text;
        if (gtemp->type == glType::Oscillogram) {
            text = QString::number( x / data->Hz ) + " s\n" +
                    QString::number( data->samples[x] );
        } else {
            text = Utility::freqToTime( x / data->Hz ) + "\n" +
                    QString::number( x / data->Hz ) + " Hz\n" +
                    QString::number( data->samples[x] );
        }

        QToolTip::showText(event->globalPosition().toPoint(), text);

        crossX = (double)event->position().x()/width() * 2 - 1;
        crossY = 1 - (double)event->position().y()/height() * 2;
    }

    updateGraph();
    QWidget::mouseMoveEvent(event);
}

void glOscillogram::leaveEvent(QEvent* event) {
    crossX = -1;
    crossY = -1;

    updateGraph();
    QWidget::leaveEvent(event);
}













