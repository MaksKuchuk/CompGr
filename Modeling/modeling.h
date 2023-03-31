#ifndef MODELING_H
#define MODELING_H

#include<cmath>

#include "../GraphGlData/Graph2DData.hpp"

class Modeling {
public:
    static Graph2DData *delayedSingleImpulse(const long long N, const double T, const long long n);

    static Graph2DData *delayedSingleHop(const long long N, const double T, const long long n);

    static Graph2DData *sampledDecreasingExponent
            (
                    const long long N, const double T, const int a
            );

    static Graph2DData *sampledSineWave
            (
                    const long long N, const double T,
                    const int a, const double omega, const double phi
            );

    static Graph2DData *meander(const long long N, const double T, const long long L);

    static Graph2DData *saw(const long long N, const double T, const long long L);

    static Graph2DData *exponentialEnvelope(
        const long long N, const double T, const double a,
        const double tau, const double f, const double phi
    );

    static Graph2DData *balanceEnvelope(
            const long long N, const double T, const double a,
            const double f0, const double fn, const double phi
    );

    static Graph2DData *tonalEnvelope(
            const long long N, const double T, const double a,
            const double f0, const double fn, const double phi, const double m
    );

    static Graph2DData *LFM(
            const long long N, const double T, const double a,
            const double f0, const double fk, const double phi0
    );
};

#endif // MODELING_H
