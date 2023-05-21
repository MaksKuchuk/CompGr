#include "statistics.h"
#include <thread>
#include <memory>
#include <algorithm>
#include <ctime>

#include <QtMath>

#include <QDebug>

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

void Statistics::calcHistogramm() {
    size_t k = histDivs;
    size_t amountOfSamples = sortedSamples.size();
    double h = (maxVal - minVal) / k;

    histogram.resize(k);
    QList<qint64> histCount(k);
    for (auto x : sortedSamples) {
        auto ind = qint64((x - minVal) / h);
        histCount[ ind < k ? ind : k - 1 ]++;
    }
    for (size_t i = 0; i < k; ++i) {
        histogram[i] = double(histCount[i]) / amountOfSamples;
    }
}

Statistics::Statistics(std::shared_ptr<Graph2DData> data, const size_t k) : graphData(data) {
    CalcStatistics(data, k);
}

void Statistics::Recalc(const size_t k) {
    CalcStatistics(graphData, k);
}

void Statistics::CalcStatistics(std::shared_ptr<Graph2DData> data, const size_t k) {

    auto s = std::chrono::high_resolution_clock().now();

    histDivs = k;
    size_t amountOfSamples = data->rcur - data->lcur + 1;
    slicedSamples = data->samples.sliced(data->lcur, amountOfSamples);

    auto sortT = std::make_unique<std::thread>(sortSamples, this);
    calcMean(mean, slicedSamples);

    auto dispT = std::make_unique<std::thread>(calcPowDiff, &dispersion, slicedSamples, mean, 2);
    auto asymmT = std::make_unique<std::thread>(calcPowDiff, &asymmetricCoefficient, slicedSamples, mean, 3);
    auto kurtT = std::make_unique<std::thread>(calcPowDiff, &kurtosisCoefficient, slicedSamples, mean, 4);

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

    calcHistogramm();

    qDebug() << duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock().now() - s).count() << " ms: Statistics calc";
}
