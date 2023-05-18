#ifndef STATISTICS_H
#define STATISTICS_H

#include <QList>
#include <memory>
#include "../GraphGlData/Graph2DData.hpp"

class Statistics
{
    const std::shared_ptr<Graph2DData> graphData;

    QList<double> slicedSamples;
    QList<double> sortedSamples;
    static void sortSamples(Statistics* stats);

    static void calcMean(double& mean, const QList<double>& samples);
    static void calcPowDiff(double* value, const QList<double>& samples, const double mean, const int pow);

//    static void calcHist();

public:
    double mean;
    double dispersion;
    double standardDeviation;
    double variationCoefficient;
    double asymmetricCoefficient;
    double kurtosisCoefficient;
    double minVal;
    double maxVal;
    double quintile05;
    double quintile95;
    double median;
    QList<double> histogram;

    size_t histDivs = 20;
    void calcHistogramm();


    Statistics(const std::shared_ptr<Graph2DData> data, const size_t k = 20);
    void CalcStatistics(const std::shared_ptr<Graph2DData> data, const size_t k = 20);
    void Recalc(const size_t k = 20);
};

#endif // STATISTICS_H
