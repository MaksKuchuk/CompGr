#include "statistics.h"
#include <thread>
#include <memory>
#include <algorithm>

#include <QtMath>

#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include "../mainwindow.h"
//#include "ui_mainwindow.h"

void Statistics::calcMean(double& mean, const QList<double>& samples) {
    double sum = 0;
    for (auto x : samples)
        sum += x;
    mean = sum / samples.size();
}

void Statistics::calcPowDiff(double* value, const QList<double>& samples, const double mean, const int pow) {
    double sum = 0;
    for (auto x : samples)
        sum += qPow((x - mean), pow);
    *value = sum / samples.size();
}

void Statistics::sortSamples(Statistics* stats) {
    stats->sortedSamples = stats->slicedSamples;
    std::sort(stats->sortedSamples.begin(), stats->sortedSamples.end());
}

Statistics::Statistics(std::shared_ptr<Graph2DData> data, const size_t k) {
    CalcStatistics(data, k);
}

void Statistics::CalcStatistics(std::shared_ptr<Graph2DData> data, const size_t k) {
    size_t amountOfSamples = data->rcur - data->lcur + 1;
    slicedSamples = data->samples.sliced(data->lcur, amountOfSamples);

    auto sortT = std::make_shared<std::thread>(sortSamples, this);
    calcMean(mean, slicedSamples);

    auto dispT = std::make_shared<std::thread>(calcPowDiff, &dispersion, slicedSamples, mean, 2);
    auto asymmT = std::make_shared<std::thread>(calcPowDiff, &asymmetricCoefficient, slicedSamples, mean, 3);
    auto kurtT = std::make_shared<std::thread>(calcPowDiff, &kurtosisCoefficient, slicedSamples, mean, 4);

    dispT->join();
    standardDeviation = qSqrt(dispersion);
    variationCoefficient = standardDeviation / mean;

    asymmT->join();
    asymmetricCoefficient /= qPow(standardDeviation, 3);

    kurtT->join();
    kurtosisCoefficient = kurtosisCoefficient / qPow(dispersion, 2) - 3;

    sortT->join();
    minVal = sortedSamples.front();
    maxVal = sortedSamples.back();

    quintile05 = sortedSamples[ size_t(0.05 * amountOfSamples) ];
    quintile95 = sortedSamples[ size_t(0.95 * amountOfSamples) ];
    median = sortedSamples[ size_t(amountOfSamples / 2) ];

    double h = (maxVal - minVal) / k;

    histogram.resize(k);
    QList<qint64> histCount(k);
    for (auto x : sortedSamples) {
        auto ind = qint64((x - minVal) / h);
        histCount[ ind < k ? ind : k - 1 ]++;
    }
    for (size_t i = 0; i < k; ++i) {
        histogram[i] = (histCount[i] * 1.0) / amountOfSamples;
    }


    //to be relocated, only for testing now


    auto qw = new QWidget(MainWindow::instance);

    auto lay = new QVBoxLayout(qw);
    lay->addWidget(new QLabel("Mean: " + QString::number( mean )));
    lay->addWidget(new QLabel("Dispersion: " + QString::number( dispersion )));
    lay->addWidget(new QLabel("standardDeviation: " + QString::number( standardDeviation )));
    lay->addWidget(new QLabel("variationCoefficient: " + QString::number( variationCoefficient )));
    lay->addWidget(new QLabel("asymmetricCoefficient: " + QString::number( asymmetricCoefficient )));
    lay->addWidget(new QLabel("kurtosisCoefficient: " + QString::number( kurtosisCoefficient )));
    lay->addWidget(new QLabel("Min: " + QString::number( minVal )));
    lay->addWidget(new QLabel("Max: " + QString::number( maxVal )));
    lay->addWidget(new QLabel("quintile05: " + QString::number( quintile05 )));
    lay->addWidget(new QLabel("quintile95: " + QString::number( quintile95 )));
    lay->addWidget(new QLabel("median: " + QString::number( median )));



    QBarSet* barSet = new QBarSet("ddd", qw);
    for (auto x : histogram) {
        *barSet << x;
    }
    auto series = new QBarSeries();
    series->append(barSet);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Simple barchart example");
//    chart->setAnimationOptions(QChart::SeriesAnimations);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(640, 480);

    lay->addWidget(chartView);
    qw->setLayout(lay);
    MainWindow::instance->AddWidget(qw);
    // hist
}
