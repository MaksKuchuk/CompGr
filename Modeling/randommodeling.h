#ifndef RANDOMMODELING_H
#define RANDOMMODELING_H

#include <cmath>
#include <memory>
#include <random>
#include <cstring>

#include "../GraphGlData/Graph2DData.hpp"


class randomModeling
{
public:
    static std::shared_ptr<Graph2DData> whiteNoise(
            const long long N, const double T, const double a, const double b
        );
};

#endif // RANDOMMODELING_H
