#ifndef MODELING_H
#define MODELING_H

#include<cmath>

#include "../GraphGlData/Graph2DData.hpp"
class Modeling {
public:
    static Graph2DData* delayedSingleImpulse(const long long N, const double T, const long long n);

    static Graph2DData* delayedSingleHop(const long long N, const double T, const long long n);

    static Graph2DData* sampledDecreasingExponent
            (
                const long long N, const double T, const int a
            );

    static Graph2DData* sampledSineWave
        (
            const long long N, const double T,
            const int a, const double omega, const double phi
        );

    static Graph2DData* meander(const long long N, const double T, const long long L);

    static Graph2DData* saw(const long long N, const double T, const long long L);
};

#endif // MODELING_H
