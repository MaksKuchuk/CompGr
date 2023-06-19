#include "glspectrogram.h"
#include "../glViewTemplate/gltemplatespectrogram.h"
#include "../analyzewidget.h"
#include "../Utility/config.h"
#include "../Utility/utility.h"

#include <QToolTip>
#include <QList>

glSpectrogram::glSpectrogram(QWidget *parent, std::shared_ptr<Graph3DData> data)
    : QOpenGLWidget{parent},
      data(data)
{
    setMouseTracking(true);
    setToolTipDuration(1e6);
    setAttribute(Qt::WA_Hover, true);
}

void glSpectrogram::updateGraph() {
    repaint();
}

void glSpectrogram::initializeGL() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);
}

void glSpectrogram::resizeGL(int w, int h) {
}

void glSpectrogram::paintGL() {
    if (Config::isDarkTheme) {
        glClearColor(0, 0, 0, 1);
        glColor3f(1, 1, 1);
    } else {
        glClearColor(1, 1, 1, 1);
        glColor3f(0, 0, 0);
    }
    drawGraph();
}

void glSpectrogram::drawGraph() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLineWidth(1);

    double x, y, x_1, y_1;

    long long lcur = data->lcur;
    long long rcur = data->rcur;

    long long parNum = rcur - lcur + 1;

//    double dotsNumber = parNum > 50000 ? 50000 : parNum;
    //double dotsNumber = parNum;
//    double diff = data->maxLoc - data->minLoc;

    auto gtemp = qobject_cast<glTemplateSpectrogram*>(parent());

    auto xScaler = &Utility::LinearScale;
    if (gtemp->xLogScale)
        xScaler = &Utility::LogScale;

    auto yScaler = &Utility::LinearScale;
    if (gtemp->yLogScale)
        yScaler = &Utility::LogScale;

    auto brightness = gtemp->brightness;

    // bottom -> top, left -> right
    glBegin(GL_QUADS);
        for (qint64 i = 0; i < parNum; ++i) {
            for (qint64 j = 0; j < data->height; ++j) {

                x = xScaler(i, parNum, -1, 1);
                y = yScaler(j, data->height, -1, 1);
                x_1 = xScaler(i+1, parNum, -1, 1);
                y_1 = yScaler(j+1, data->height, -1, 1);

                auto col = data->samples[qint64(lcur + i)][j]/(brightness *  data->maxVal);

                colorSchemeDarkHeat(col);

                glVertex2d(x, y);
                glVertex2d(x, y_1 );
                glVertex2d(x_1, y_1 );
                glVertex2d(x_1, y);
            }
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

void glSpectrogram::colorSchemeGrey(float t) {
    glColor3f(t, t, t);
}

void glSpectrogram::colorSchemeHeat(float t) {
    if (t < 0.5)
        glColor3f(0, 2*t, 1 - 2*t);
    else
        glColor3f(2*t - 1, 2-2*t, 0);

//    float R = t*t;
//    float G = 2*t - 2*t*t;
//    float B = 1 - 2*t + t*t;
}

void glSpectrogram::colorSchemeDarkHeat(float t) {
    float R, G, B;
    if (t > 1) {
        R = 1;
        G = 1;
        B = 1;
    } else {
        R = 2*t-t*t;
        G = 1 - 4*(t-1)*(t-1);
        B = 1 - 16 *(t-1)*(t-1);
    }

    glColor3f(R, G > 0 ? G : 0, B > 0 ? B : 0);

//    float R = t*t;
//    float G = 2*t - 2*t*t;
//    float B = 1 - 2*t + t*t;
}

void glSpectrogram::mouseMoveEvent(QMouseEvent *event) {
    long long lcur = data->lcur;
    long long rcur = data->rcur;

    long long parNum = rcur - lcur + 1;

    auto xScaler = &Utility::LinearScale;
    auto yScaler = &Utility::LinearScale;

    qint64 x = xScaler(event->position().x(), width(), lcur, rcur);
    qint64 y = yScaler(event->position().y(), height(), data->height - 1, 0);
    x = x < 0 ? 0 : x;

    auto text = QString::number( x / data->Hz ) + " s\n" +
                QString::number( y / data->yHz ) + " Hz (" +
                Utility::freqToTime( y / data->yHz ) + ")\n" +
                QString::number( data->samples[x][y] );

    QToolTip::showText(event->globalPosition().toPoint(), text);

    crossX = (double)event->position().x()/width() * 2 - 1;
    crossY = 1 - (double)event->position().y()/height() * 2;

    updateGraph();
    QWidget::mouseMoveEvent(event);
}

void glSpectrogram::leaveEvent(QEvent* event) {
    crossX = -1;
    crossY = -1;

    updateGraph();
    QWidget::leaveEvent(event);
}
