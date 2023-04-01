#include "modeling.h"

#define _USE_MATH_DEFINES

/*
    N - amount of samples
    T - sampling step
    n - parameter
*/
Graph2DData *Modeling::delayedSingleImpulse(const long long N, const long long n, Graph2DData* data) {
    Graph2DData *data2D = new Graph2DData();
    data2D->name = "Model_J_1";
    data2D->source = "Delayed Single Impulse n = " + std::to_string(n);

    data2D->maxVal = 1;
    data2D->minVal = 0;

    data2D->maxLoc = 1;
    data2D->minLoc = 0;
    data2D->amountOfSamples = (data == nullptr) ? N : data->amountOfSamples;

    data2D->lcur = 0;
    data2D->rcur = (N - 1);

    data2D->Hz = 1;

    data2D->totalSeconds = (N - 1);

    double *samples = new double[N]();
    samples[n] = 1;
    data2D->samples = samples;

    return data2D;
}

/*
    N - amount of samples
    T - sampling step
    n - parameter
*/
Graph2DData *Modeling::delayedSingleHop(const long long N, const long long n, Graph2DData* data) {
    Graph2DData *data2D = new Graph2DData();
    data2D->name = "Model_J_2";
    data2D->source = "Delayed Single Hop n = " + std::to_string(n);

    data2D->maxVal = 1;
    data2D->minVal = 0;

    data2D->maxLoc = 1;
    data2D->minLoc = 0;
    data2D->amountOfSamples = (data == nullptr) ? N : data->amountOfSamples;

    data2D->lcur = 0;
    data2D->rcur = (N - 1);

    data2D->Hz = 1;

    data2D->totalSeconds = (N - 1);

    double *samples = new double[N]();
    for (unsigned long long i = n; i < N; ++i) {
        samples[i] = 1;
    }
    data2D->samples = samples;

    return data2D;
}


/*
    N - amount of samples
    T - sampling step
    a - parameter, n >= 0, 0 <= a <= 1
*/
Graph2DData *Modeling::sampledDecreasingExponent
        (
                const long long N, const int a, Graph2DData* data
        ) {
    Graph2DData *data2D = new Graph2DData();
    data2D->name = "Model_J_3";
    data2D->source = "Sampled Decreased Exponent a = " + std::to_string(a);

    data2D->amountOfSamples = (data == nullptr) ? N : data->amountOfSamples;

    data2D->lcur = 0;
    data2D->rcur = (N - 1);

    data2D->Hz = 1;

    data2D->totalSeconds = (N - 1);

    double min = std::numeric_limits<double>::max();
    double max = -std::numeric_limits<double>::max();

    double *samples = new double[N]();
    for (unsigned long long i = 0; i < N; ++i) {
        samples[i] = std::pow(a, i);
        if (samples[i] > max) max = samples[i];
        if (samples[i] < min) min = samples[i];
    }
    data2D->samples = samples;

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
Graph2DData *Modeling::sampledSineWave
        (
                const long long N,
                const int a, const double circFreq, const double initPhase, Graph2DData* data
        ) {
    Graph2DData *data2D = new Graph2DData();
    data2D->name = "Model_J_4";
    data2D->source = "Sampled Sine Wave amplitude = " + std::to_string(a)
                     + " circle frequency = " + std::to_string(circFreq)
                     + " initial phase = " + std::to_string(initPhase);

    data2D->amountOfSamples = (data == nullptr) ? N : data->amountOfSamples;

    data2D->lcur = 0;
    data2D->rcur = (N - 1);

    data2D->Hz = 1;

    data2D->totalSeconds = (N - 1);

    double min = std::numeric_limits<double>::max();
    double max = -std::numeric_limits<double>::max();

    double *samples = new double[N]();
    for (unsigned long long i = 0; i < N; ++i) {
        samples[i] = a * std::sin(i * circFreq + initPhase);
        if (samples[i] > max) max = samples[i];
        if (samples[i] < min) min = samples[i];
    }
    data2D->samples = samples;

    data2D->minVal = min;
    data2D->maxVal = max;
    data2D->minLoc = min;
    data2D->maxLoc = max;

    return data2D;
}

Graph2DData *Modeling::meander(const long long N, const long long L, Graph2DData* data) {
    Graph2DData *data2D = new Graph2DData();
    data2D->name = "Model_J_5";
    data2D->source = "Meander period = " + std::to_string(L);

    data2D->amountOfSamples = (data == nullptr) ? N : data->amountOfSamples;

    data2D->lcur = 0;
    data2D->rcur = (N - 1);

    data2D->Hz = 1;

    data2D->totalSeconds = (N - 1);

    double min = 1;
    double max = -1;

    double *samples = new double[N]();
    for (unsigned long long i = 0; i < N; ++i) {
        samples[i] = (i % L < L / 2) ? 1 : -1;
        if (samples[i] > max) max = samples[i];
        if (samples[i] < min) min = samples[i];
    }
    data2D->samples = samples;

    data2D->minVal = min;
    data2D->maxVal = max;
    data2D->minLoc = min;
    data2D->maxLoc = max;

    return data2D;
}

Graph2DData *Modeling::saw(const long long N, const long long L, Graph2DData* data) {
    Graph2DData *data2D = new Graph2DData();
    data2D->name = "Model_J_6";
    data2D->source = "Saw period = " + std::to_string(L);

    data2D->amountOfSamples = (data == nullptr) ? N : data->amountOfSamples;

    data2D->lcur = 0;
    data2D->rcur = (N - 1);

    data2D->Hz = 1;

    data2D->totalSeconds = (N - 1);

    double min = std::numeric_limits<double>::max();
    double max = -std::numeric_limits<double>::max();

    double *samples = new double[N]();
    for (unsigned long long i = 0; i < N; ++i) {
        samples[i] = (i % L) / L;
        if (samples[i] > max) max = samples[i];
        if (samples[i] < min) min = samples[i];
    }
    data2D->samples = samples;

    data2D->minVal = min;
    data2D->maxVal = max;
    data2D->minLoc = min;
    data2D->maxLoc = max;

    return data2D;
}

static Graph2DData *exponentialEnvelope(
        const long long N, const double T, const double a,
        const double tau, const double f, const double phi
) {
    Graph2DData *data2D = new Graph2DData();
    data2D->name = "Model_J_7";
    data2D->source = "Amplitude = " + std::to_string(a)
                     + " envelope width = " + std::to_string(tau)
                     + " carrier freq = " + std::to_string(f)
                     + " phase = " + std::to_string(phi);

    data2D->amountOfSamples = N;

    data2D->lcur = 0;
    data2D->rcur = (N - 1) * T;

    data2D->Hz = 1 / T;

    data2D->totalSeconds = (N - 1) * T;

    double min = std::numeric_limits<double>::max();
    double max = -std::numeric_limits<double>::max();

    double *samples = new double[N]();
    for (size_t i = 0; i < N; ++i) {
        double t = data2D->Hz * i;
        samples[i] = a * exp(-t / tau) * cos(2 * M_PI * f * t + phi);
        min = std::min(min, samples[i]);
        max = std::max(max, samples[i]);
    }
    data2D->samples = samples;

    data2D->minVal = min;
    data2D->maxVal = max;
    data2D->minLoc = min;
    data2D->maxLoc = max;

    return data2D;
}

static Graph2DData *balanceEnvelope(
        const long long N, const double T, const double a,
        const double f0, const double fn, const double phi
) {
    Graph2DData *data2D = new Graph2DData();
    data2D->name = "Model_J_8";
    data2D->source = "Amplitude = " + std::to_string(a)
                     + " envelope freq = " + std::to_string(f0)
                     + " carrier freq = " + std::to_string(fn)
                     + " carrier phase = " + std::to_string(phi);

    data2D->amountOfSamples = N;

    data2D->lcur = 0;
    data2D->rcur = (N - 1) * T;

    data2D->Hz = 1 / T;

    data2D->totalSeconds = (N - 1) * T;

    double min = std::numeric_limits<double>::max();
    double max = -std::numeric_limits<double>::max();

    double *samples = new double[N]();
    for (size_t i = 0; i < N; ++i) {
        double t = data2D->Hz * i;
        samples[i] = a * cos(2 * M_PI * f0 * t) * cos(2 * M_PI * fn * t + phi);
        min = std::min(min, samples[i]);
        max = std::max(max, samples[i]);
    }
    data2D->samples = samples;

    data2D->minVal = min;
    data2D->maxVal = max;
    data2D->minLoc = min;
    data2D->maxLoc = max;

    return data2D;
}

static Graph2DData *tonalEnvelope(
        const long long N, const double T, const double a,
        const double f0, const double fn, const double phi, const double m
) {
    Graph2DData *data2D = new Graph2DData();
    data2D->name = "Model_J_9";
    data2D->source = "Amplitude = " + std::to_string(a)
                     + " envelope freq = " + std::to_string(f0)
                     + " carrier freq = " + std::to_string(fn)
                     + " carrier phase = " + std::to_string(phi)
                     + " modulation depth index = " + std::to_string(m);

    data2D->amountOfSamples = N;

    data2D->lcur = 0;
    data2D->rcur = (N - 1) * T;

    data2D->Hz = 1 / T;

    data2D->totalSeconds = (N - 1) * T;

    double min = std::numeric_limits<double>::max();
    double max = -std::numeric_limits<double>::max();

    double *samples = new double[N]();
    for (size_t i = 0; i < N; ++i) {
        double t = data2D->Hz * i;
        samples[i] = a * (1 + m * cos(2 * M_PI * f0 * t)) * cos(2 * M_PI * fn * t + phi);
        min = std::min(min, samples[i]);
        max = std::max(max, samples[i]);
    }
    data2D->samples = samples;

    data2D->minVal = min;
    data2D->maxVal = max;
    data2D->minLoc = min;
    data2D->maxLoc = max;

    return data2D;
}

static Graph2DData *LFM(
        const long long N, const double T, const double a,
        const double f0, const double fk, const double phi0
) {
    Graph2DData *data2D = new Graph2DData();
    data2D->name = "Model_J_10";
    data2D->source = "Amplitude = " + std::to_string(a)
                     + " start freq = " + std::to_string(f0)
                     + " end freq = " + std::to_string(fk)
                     + " start phase = " + std::to_string(phi0);

    data2D->amountOfSamples = N;

    data2D->lcur = 0;
    data2D->rcur = (N - 1) * T;

    data2D->Hz = 1 / T;

    data2D->totalSeconds = (N - 1) * T;

    double min = std::numeric_limits<double>::max();
    double max = -std::numeric_limits<double>::max();

    double totalTime = N * T;

    double *samples = new double[N]();
    for (size_t i = 0; i < N; ++i) {
        double t = data2D->Hz * i;
        samples[i] = a * cos(2 * M_PI * (f0 + (fk - f0) * t / totalTime) * t + phi0);
        min = std::min(min, samples[i]);
        max = std::max(max, samples[i]);
    }
    data2D->samples = samples;

    data2D->minVal = min;
    data2D->maxVal = max;
    data2D->minLoc = min;
    data2D->maxLoc = max;

    return data2D;
}
