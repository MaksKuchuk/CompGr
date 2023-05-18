#ifndef STATISTICSWIDGET_H
#define STATISTICSWIDGET_H

#include <QObject>
#include <QWidget>

#include <QLabel>
#include <QPointer>
#include <QLineEdit>
#include <QtCharts/QBarSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChartView>

#include "../GraphGlData/Graph2DData.hpp"
#include "statistics.h"

class StatisticsWidget : public QWidget
{
    Q_OBJECT

    std::shared_ptr<Statistics> statistics;

    QPointer<QLabel> meanLabel;
    QPointer<QLabel> dispersionLabel;
    QPointer<QLabel> standardDeviationLabel;
    QPointer<QLabel> variationCoefficientLabel;
    QPointer<QLabel> asymmetricCoefficientLabel;
    QPointer<QLabel> kurtosisCoefficientLabel;
    QPointer<QLabel> minValLabel;
    QPointer<QLabel> maxValLabel;
    QPointer<QLabel> quintile05Label;
    QPointer<QLabel> quintile95Label;
    QPointer<QLabel> medianLabel;

    QPointer<QLineEdit> numberOfDivisions;

    struct Chart {
        QPointer<QBarSet> set;
        QPointer<QBarSeries> series;
        QPointer<QChart> chart;
        QPointer<QValueAxis> axisX, axisY;
        QPointer<QChartView> chartView;
    };

    Chart histChart;

public:
    explicit StatisticsWidget(QWidget *parent = nullptr, const std::shared_ptr<Graph2DData> graphData = nullptr);
    void recalcualte();

    bool isCorrect = false;
signals:

};

#endif // STATISTICSWIDGET_H
