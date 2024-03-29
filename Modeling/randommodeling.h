#ifndef RANDOMMODELING_H
#define RANDOMMODELING_H

#include <cmath>
#include <memory>
#include <random>

#include "../GraphGlData/Graph2DData.hpp"


class randomModeling
{
public:
    static std::shared_ptr<Graph2DData> whiteNoise(
            const long long N, const double T, const double a, const double b
        );

    static std::shared_ptr<Graph2DData> normalDistrWhiteNoise(
            const long long N, const double T, const double a, const double dispersion
        );

    static std::shared_ptr<Graph2DData> ARMA(
            const long long N, const double T, const double dispersion,
            const QList<double> as, const QList<double> bs
        );
};

#endif // RANDOMMODELING_H
