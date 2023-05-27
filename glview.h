#ifndef GLVIEW_H
#define GLVIEW_H

#include <QOpenGLWidget>
#include <QPointer>
#include <QList>

static const QList<double> _ref{0};

class glView : public QOpenGLWidget
{
    Q_OBJECT
    const QList<double>& data;
    long long parNum;
    double maxVal;
    double minVal;
    long long lcur, rcur;

signals:
    void cursChanged(long long lcur, long long rcur);

public slots:
    void setCurs(long long lcur, long long rcur);

public:
    glView(QWidget *parent = nullptr,
           long long parNum = 0,
           const QList<double>& data = _ref,
           double maxVal = 0,
           double minVal = 0
            );

protected:
    void drawGraph();

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};

#endif // GLVIEW_H
