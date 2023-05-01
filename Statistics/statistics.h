#ifndef STATISTICS_H
#define STATISTICS_H

#include <QList>
#include <memory>
#include "../GraphGlData/Graph2DData.hpp"

class Statistics
{
    QList<double> slicedSamples;
    QList<double> sortedSamples;
    static void sortSamples(Statistics* stats);

    static void calcMean(double& mean, const QList<double>& samples);
    static void calcPowDiff(double* value, const QList<double>& samples, const double mean, const int pow);

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


    Statistics(std::shared_ptr<Graph2DData> data);
    void CalcStatistics(std::shared_ptr<Graph2DData> data);
};

#endif // STATISTICS_H
