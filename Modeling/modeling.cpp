#define _USE_MATH_DEFINES

#include <cmath>
#include <memory>

#include "modeling.h"
#include "../GraphGlData/Graph2DData.hpp"


/*
    N - amount of samples
    T - sampling step
    n - parameter
*/
std::shared_ptr<Graph2DData> Modeling::delayedSingleImpulse(const long long N, const long long n) {
    auto data2D = std::make_shared<Graph2DData>();
    data2D->name = "Model_1_";
    data2D->source = "Delayed Single Impulse n = " + QString::number(n);

    data2D->maxVal = 1;
    data2D->minVal = 0;

    data2D->maxLoc = 1;
    data2D->minLoc = 0;
    data2D->amountOfSamples = N;

    data2D->lcur = 0;
    data2D->rcur = (N - 1);

    data2D->Hz = 1;

    data2D->totalSeconds = (N - 1);

    data2D->samples.resize(N);
    if (n < N)
        data2D->samples[n] = 1;

    return data2D;
}

/*
    N - amount of samples
    T - sampling step
    n - parameter
*/
std::shared_ptr<Graph2DData> Modeling::delayedSingleHop(const long long N, const long long n) {
    auto data2D = std::make_shared<Graph2DData>();
    data2D->name = "Model_2_";
    data2D->source = "Delayed Single Hop n = " + QString::number(n);

    data2D->maxVal = 1;
    data2D->minVal = 0;

    data2D->maxLoc = 1;
    data2D->minLoc = 0;
    data2D->amountOfSamples = N;

    data2D->lcur = 0;
    data2D->rcur = (N - 1);

    data2D->Hz = 1;

    data2D->totalSeconds = (N - 1);

    data2D->samples.resize(N);
    for (unsigned long long i = n; i < N; ++i) {
        data2D->samples[i] = 1;
    }

    return data2D;
}


/*
    N - amount of samples
    T - sampling step
    a - parameter, n >= 0, 0 <= a <= 1
*/
std::shared_ptr<Graph2DData> Modeling::sampledDecreasingExponent
        (
                const long long N, const double a
        ) {
    auto data2D = std::make_shared<Graph2DData>();
    data2D->name = "Model_3_";
    data2D->source = "Sampled Decreased Exponent a = " + QString::number(a);

    data2D->amountOfSamples = N;

    data2D->lcur = 0;
    data2D->rcur = (N - 1);

    data2D->Hz = 1;

    data2D->totalSeconds = (N - 1);

    double min = std::numeric_limits<double>::max();
    double max = -std::numeric_limits<double>::max();

    data2D->samples.resize(N);
    for (unsigned long long i = 0; i < N; ++i) {
        data2D->samples[i] = std::pow(a, i);
        if (data2D->samples[i] > max) max = data2D->samples[i];
        if (data2D->samples[i] < min) min = data2D->samples[i];
    }

    data2D->minVal = min;
    data2D->maxVal = max;
    data2D->minLoc = min;
    data2D->maxLoc = max;

    return data2D;
}

/*
    a - amplitude
    circFreq - circle frequency, [0, pi]
    initPhase - initial phase, [0, 2pi]
*/
std::shared_ptr<Graph2DData> Modeling::sampledSineWave
        (
                const long long N,
                const double a, const double circFreq, const double initPhase
        ) {
    auto data2D = std::make_shared<Graph2DData>();
    data2D->name = "Model_4_";
    data2D->source = "Sampled Sine Wave amplitude = " + QString::number(a)
                     + " circle frequency = " + QString::number(circFreq)
                     + " initial phase = " + QString::number(initPhase);

    data2D->amountOfSamples = N;

    data2D->lcur = 0;
    data2D->rcur = (N - 1);

    data2D->Hz = 1;

    data2D->totalSeconds = (N - 1);

    double min = std::numeric_limits<double>::max();
    double max = -std::numeric_limits<double>::max();

    data2D->samples.resize(N);
    for (unsigned long long i = 0; i < N; ++i) {
        data2D->samples[i] = a * std::sin(i * circFreq + initPhase);
        if (data2D->samples[i] > max) max = data2D->samples[i];
        if (data2D->samples[i] < min) min = data2D->samples[i];
    }

    data2D->minVal = min;
    data2D->maxVal = max;
    data2D->minLoc = min;
    data2D->maxLoc = max;

    return data2D;
}

std::shared_ptr<Graph2DData> Modeling::meander(const long long N, const long long L) {
    auto data2D = std::make_shared<Graph2DData>();
    data2D->name = "Model_5_";
    data2D->source = "Meander period = " + QString::number(L);

    data2D->amountOfSamples = N;

    data2D->lcur = 0;
    data2D->rcur = (N - 1);

    data2D->Hz = 1;

    data2D->totalSeconds = (N - 1);

    double min = 1;
    double max = -1;

    data2D->samples.resize(N);
    if (L > 0) {
        for (unsigned long long i = 0; i < N; ++i) {
            data2D->samples[i] = (i % L < L / 2) ? 1 : -1;
            if (data2D->samples[i] > max) max = data2D->samples[i];
            if (data2D->samples[i] < min) min = data2D->samples[i];
        }
    }

    data2D->minVal = min;
    data2D->maxVal = max;
    data2D->minLoc = min;
    data2D->maxLoc = max;

    return data2D;
}

std::shared_ptr<Graph2DData> Modeling::saw(const long long N, const long long L) {
    auto data2D = std::make_shared<Graph2DData>();
    data2D->name = "Model_6_";
    data2D->source = "Saw period = " + QString::number(L);

    data2D->amountOfSamples = N;

    data2D->lcur = 0;
    data2D->rcur = (N - 1);

    data2D->Hz = 1;

    data2D->totalSeconds = (N - 1);

    double min = std::numeric_limits<double>::max();
    double max = -std::numeric_limits<double>::max();

    data2D->samples.resize(N);
    if (L > 0) {
        for (unsigned long long i = 0; i < N; ++i) {
            data2D->samples[i] = (i % L) / (double)L;
            if (data2D->samples[i] > max) max = data2D->samples[i];
            if (data2D->samples[i] < min) min = data2D->samples[i];
        }
    }

    data2D->minVal = min;
    data2D->maxVal = max;
    data2D->minLoc = min;
    data2D->maxLoc = max;

    return data2D;
}

std::shared_ptr<Graph2DData> Modeling::exponentialEnvelope(
        const long long N, const double T, const double a,
        const double tau, const double f, const double phi
) {
    auto data2D = std::make_shared<Graph2DData>();
    data2D->name = "Model_7_";
    data2D->source = "Amplitude = " + QString::number(a)
                     + " envelope width = " + QString::number(tau)
                     + " carrier freq = " + QString::number(f)
                     + " phase = " + QString::number(phi);

    data2D->amountOfSamples = N;

    data2D->lcur = 0;
    data2D->rcur = (N - 1);

    data2D->Hz = 1 / T;

    data2D->totalSeconds = (N - 1) * T;

    double min = std::numeric_limits<double>::max();
    double max = -std::numeric_limits<double>::max();

    data2D->samples.resize(N);
    for (size_t i = 0; i < N; ++i) {
        double t = data2D->Hz * i;
        data2D->samples[i] = a * exp(-t / tau) * cos(2 * M_PI * f * t + phi);
        min = std::min(min, data2D->samples[i]);
        max = std::max(max, data2D->samples[i]);
    }

    data2D->minVal = min;
    data2D->maxVal = max;
    data2D->minLoc = min;
    data2D->maxLoc = max;

    return data2D;
}

std::shared_ptr<Graph2DData> Modeling::balanceEnvelope(
        const long long N, const double T, const double a,
        const double f0, const double fn, const double phi
) {
    auto data2D = std::make_shared<Graph2DData>();
    data2D->name = "Model_8_";
    data2D->source = "Amplitude = " + QString::number(a)
                     + " envelope freq = " + QString::number(f0)
                     + " carrier freq = " + QString::number(fn)
                     + " carrier phase = " + QString::number(phi);

    data2D->amountOfSamples = N;

    data2D->lcur = 0;
    data2D->rcur = (N - 1);

    data2D->Hz = 1 / T;

    data2D->totalSeconds = (N - 1) * T;

    double min = std::numeric_limits<double>::max();
    double max = -std::numeric_limits<double>::max();

    data2D->samples.resize(N);
    for (size_t i = 0; i < N; ++i) {
        double t = data2D->Hz * i;
        data2D->samples[i] = a * cos(2 * M_PI * f0 * t) * cos(2 * M_PI * fn * t + phi);
        min = std::min(min, data2D->samples[i]);
        max = std::max(max, data2D->samples[i]);
    }

    data2D->minVal = min;
    data2D->maxVal = max;
    data2D->minLoc = min;
    data2D->maxLoc = max;

    return data2D;
}

std::shared_ptr<Graph2DData> Modeling::tonalEnvelope(
        const long long N, const double T, const double a,
        const double f0, const double fn, const double phi, const double m
) {
    auto data2D = std::make_shared<Graph2DData>();
    data2D->name = "Model_9_";
    data2D->source = "Amplitude = " + QString::number(a)
                     + " envelope freq = " + QString::number(f0)
                     + " carrier freq = " + QString::number(fn)
                     + " carrier phase = " + QString::number(phi)
                     + " modulation depth index = " + QString::number(m);

    data2D->amountOfSamples = N;

    data2D->lcur = 0;
    data2D->rcur = (N - 1);

    data2D->Hz = 1 / T;

    data2D->totalSeconds = (N - 1) * T;

    double min = std::numeric_limits<double>::max();
    double max = -std::numeric_limits<double>::max();

    data2D->samples.resize(N);
    for (size_t i = 0; i < N; ++i) {
        double t = data2D->Hz * i;
        data2D->samples[i] = a * (1 + m * cos(2 * M_PI * f0 * t)) * cos(2 * M_PI * fn * t + phi);
        min = std::min(min, data2D->samples[i]);
        max = std::max(max, data2D->samples[i]);
    }

    data2D->minVal = min;
    data2D->maxVal = max;
    data2D->minLoc = min;
    data2D->maxLoc = max;

    return data2D;
}

std::shared_ptr<Graph2DData> Modeling::LFM(
        const long long N, const double T, const double a,
        const double f0, const double fk, const double phi0
) {
    auto data2D = std::make_shared<Graph2DData>();
    data2D->name = "Model_10_";
    data2D->source = "Amplitude = " + QString::number(a)
                     + " start freq = " + QString::number(f0)
                     + " end freq = " + QString::number(fk)
                     + " start phase = " + QString::number(phi0);

    data2D->amountOfSamples = N;

    data2D->lcur = 0;
    data2D->rcur = (N - 1);

    data2D->Hz = 1 / T;

    data2D->totalSeconds = (N - 1) * T;

    double min = std::numeric_limits<double>::max();
    double max = -std::numeric_limits<double>::max();

    double totalTime = N * T;

    data2D->samples.resize(N);
    for (size_t i = 0; i < N; ++i) {
        double t = data2D->Hz * i;
        data2D->samples[i] = a * cos(2 * M_PI * (f0 + (fk - f0) * t / totalTime) * t + phi0);
        min = std::min(min, data2D->samples[i]);
        max = std::max(max, data2D->samples[i]);
    }

    data2D->minVal = min;
    data2D->maxVal = max;
    data2D->minLoc = min;
    data2D->maxLoc = max;

    return data2D;
}
