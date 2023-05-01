#include "statistics.h"
#include <thread>
#include <memory>
#include <algorithm>

#include <QtMath>

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

Statistics::Statistics(std::shared_ptr<Graph2DData> data) {
    CalcStatistics(data);
}

void Statistics::CalcStatistics(std::shared_ptr<Graph2DData> data) {
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

    // hist
}
