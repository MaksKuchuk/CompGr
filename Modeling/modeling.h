#ifndef MODELING_H
#define MODELING_H

#include <cmath>
#include <memory>

#include "../GraphGlData/Graph2DData.hpp"

class Modeling {
public:

    static std::shared_ptr<Graph2DData> delayedSingleImpulse(const long long N, const double T, const long long n);

    static std::shared_ptr<Graph2DData> delayedSingleHop(const long long N, const double T, const long long n);

    static std::shared_ptr<Graph2DData> sampledDecreasingExponent
            (
                    const long long N, const double T, const double a
            );

    static std::shared_ptr<Graph2DData> sampledSineWave
            (
                    const long long N, const double T, const double a,
                    const double omega, const double phi
            );

    static std::shared_ptr<Graph2DData> meander(const long long N, const double T, const long long L);

    static std::shared_ptr<Graph2DData> saw(const long long N, const double T, const long long L);

    static std::shared_ptr<Graph2DData> exponentialEnvelope
    (
        const long long N, const double T, const double a,
        const double tau, const double f, const double phi
    );

    static std::shared_ptr<Graph2DData> balanceEnvelope
    (
            const long long N, const double T, const double a,
            const double f0, const double fn, const double phi
    );

    static std::shared_ptr<Graph2DData> tonalEnvelope
    (
            const long long N, const double T, const double a,
            const double f0, const double fn, const double phi, const double m
    );

    static std::shared_ptr<Graph2DData> LFM
    (
            const long long N, const double T, const double a,
            const double f0, const double fk, const double phi0
    );



    enum class Type {
        SingleImpulse,
        SingleHop,
        DecreasingExponent,
        SineWave,
        Meander,
        Saw,
        ExponentialEnvelope,
        BalanceEnvelope,
        TonalEnvelope,
        LFM,

        UniformWhiteNoise,
        NormalWhiteNoise,
        AutoregressiveMovingAverage,


        __AMOUNT_OF_TYPES
    };

    static size_t AmountOfTypes() {
        return (size_t)Type::__AMOUNT_OF_TYPES;
    }

    static QList<QString> TypesList();

    static QString TypeToString(Type type);

    static QString TypeToFormula(Type type);
};

#endif // MODELING_H
